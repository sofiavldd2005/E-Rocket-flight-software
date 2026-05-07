#include <rclcpp/rclcpp.hpp>
#include <px4_msgs/msg/vehicle_attitude.hpp>
#include <px4_msgs/msg/vehicle_angular_velocity.hpp>
#include <px4_msgs/msg/vehicle_local_position.hpp>
#include <px4_msgs/msg/vehicle_odometry.hpp>
#include <px4_msgs/msg/actuator_motors.hpp>
#include <px4_msgs/msg/actuator_servos.hpp>
#include <erocket/msg/flight_mode.hpp>
#include <std_msgs/msg/string.hpp>

#include <chrono>
#include <deque>
#include <map>
#include <cmath>
#include <iomanip>
#include <sstream>

using namespace std::chrono;

/**
 * @brief Tracks health metrics for a single topic
 * 
 * Monitors:
 * - Message frequency (Hz)
 * - Message latency (age of data)
 * - Dropped messages (gaps in sequence)
 * - Timestamp monotonicity (out-of-order detection)
 */
struct TopicHealth {
    std::string topic_name;
    uint64_t message_count = 0;
    uint64_t dropped_messages = 0;
    
    // Timestamp tracking (microseconds)
    uint64_t last_timestamp_us = 0;
    uint64_t last_receive_time_us = 0;
    
    // Frequency tracking (ring buffer of inter-message times)
    static constexpr size_t FREQ_WINDOW_SIZE = 20;
    std::deque<double> inter_message_times_ms;
    
    // Latency tracking
    static constexpr size_t LATENCY_WINDOW_SIZE = 20;
    std::deque<double> message_latencies_ms;
    
    // Thresholds (configurable)
    double max_message_age_ms = 100.0;    // Warn if older than this
    double min_expected_frequency_hz = 50.0;
    double max_frequency_variance_pct = 50.0;
    
    // State
    bool is_healthy = false;
    std::string last_error_message;
    system_clock::time_point last_update_time;
    
    /**
     * @brief Update health metrics when a new message arrives
     * @param timestamp_us The timestamp from the message (microseconds)
     */
    void update(uint64_t timestamp_us) {
        auto now = system_clock::now();
        auto now_us = duration_cast<microseconds>(now.time_since_epoch()).count();
        
        // Check for out-of-order / timestamp anomalies
        if (last_timestamp_us > 0 && timestamp_us < last_timestamp_us) {
            dropped_messages++;
            last_error_message = "Timestamp went backwards (out-of-order message)";
            is_healthy = false;
            return;
        }
        
        // Record inter-message time
        if (last_receive_time_us > 0) {
            double inter_msg_time_ms = (now_us - last_receive_time_us) / 1000.0;
            inter_message_times_ms.push_back(inter_msg_time_ms);
            if (inter_message_times_ms.size() > FREQ_WINDOW_SIZE) {
                inter_message_times_ms.pop_front();
            }
        }
        
        // Record message latency (age of data)
        double latency_ms = (now_us - timestamp_us) / 1000.0;
        message_latencies_ms.push_back(latency_ms);
        if (message_latencies_ms.size() > LATENCY_WINDOW_SIZE) {
            message_latencies_ms.pop_front();
        }
        
        // Check if data is too old
        if (latency_ms > max_message_age_ms) {
            std::ostringstream oss;
            oss << "Message latency too high: " << std::fixed << std::setprecision(2) 
                << latency_ms << " ms (threshold: " << max_message_age_ms << " ms)";
            last_error_message = oss.str();
            is_healthy = false;
            return;
        }
        
        // Check frequency
        if (inter_message_times_ms.size() >= 5) {  // Need at least 5 samples
            double avg_inter_msg_time = 0.0;
            for (double t : inter_message_times_ms) {
                avg_inter_msg_time += t;
            }
            avg_inter_msg_time /= inter_message_times_ms.size();
            
            double actual_frequency_hz = 1000.0 / avg_inter_msg_time;
            
            if (actual_frequency_hz < min_expected_frequency_hz * 0.9) {  // 90% of expected
                std::ostringstream oss;
                oss << "Frequency too low: " << std::fixed << std::setprecision(1)
                    << actual_frequency_hz << " Hz (expected >= " 
                    << min_expected_frequency_hz << " Hz)";
                last_error_message = oss.str();
                is_healthy = false;
                return;
            }
        }
        
        last_timestamp_us = timestamp_us;
        last_receive_time_us = now_us;
        message_count++;
        is_healthy = true;
        last_update_time = now;
    }
    
    /**
     * @brief Check if this topic has gone silent
     */
    bool is_stale(std::chrono::milliseconds timeout) const {
        auto now = system_clock::now();
        auto elapsed = duration_cast<milliseconds>(now - last_update_time);
        return elapsed > timeout;
    }
    
    /**
     * @brief Get current statistics
     */
    std::string get_status_string() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        
        double avg_frequency = 0.0;
        if (!inter_message_times_ms.empty()) {
            double avg_inter_msg = 0.0;
            for (double t : inter_message_times_ms) {
                avg_inter_msg += t;
            }
            avg_inter_msg /= inter_message_times_ms.size();
            avg_frequency = 1000.0 / avg_inter_msg;
        }
        
        double avg_latency = 0.0;
        if (!message_latencies_ms.empty()) {
            for (double t : message_latencies_ms) {
                avg_latency += t;
            }
            avg_latency /= message_latencies_ms.size();
        }
        
        oss << "[" << topic_name << "] "
            << "Count: " << message_count << " | "
            << "Freq: " << avg_frequency << " Hz | "
            << "Latency: " << avg_latency << " ms | "
            << "Health: " << (is_healthy ? "OK" : "WARN");
        
        if (!is_healthy) {
            oss << " | Error: " << last_error_message;
        }
        
        return oss.str();
    }
};

/**
 * @brief System Monitor Node
 * 
 * Subscribes to all critical topics and tracks their health.
 * Publishes periodic status reports.
 * Triggers alarms when issues are detected.
 */
class SystemMonitor : public rclcpp::Node {
public:
    explicit SystemMonitor() : Node("system_monitor") {
        // Get parameters
        declare_parameter<int>("max_message_age_ms", 100);
        declare_parameter<int>("min_expected_frequency_hz", 50);
        declare_parameter<int>("check_interval_ms", 500);
        declare_parameter<int>("stale_topic_timeout_ms", 2000);
        
        max_message_age_ms_ = get_parameter("max_message_age_ms").as_int();
        min_expected_frequency_hz_ = get_parameter("min_expected_frequency_hz").as_int();
        int check_interval_ms = get_parameter("check_interval_ms").as_int();
        stale_topic_timeout_ms_ = get_parameter("stale_topic_timeout_ms").as_int();
        
        RCLCPP_INFO(get_logger(), "SystemMonitor initialized with:");
        RCLCPP_INFO(get_logger(), "  max_message_age_ms: %d", max_message_age_ms_);
        RCLCPP_INFO(get_logger(), "  min_expected_frequency_hz: %d", min_expected_frequency_hz_);
        RCLCPP_INFO(get_logger(), "  check_interval_ms: %d", check_interval_ms);
        
        // Subscribe to critical topics
        setup_subscriptions();
        
        // Timer to periodically check health
        health_check_timer_ = create_wall_timer(
            std::chrono::milliseconds(check_interval_ms),
            [this] { check_health_and_report(); });
    }

private:
    std::map<std::string, TopicHealth> topic_health_;
    
    // Thresholds
    int max_message_age_ms_;
    int min_expected_frequency_hz_;
    int stale_topic_timeout_ms_;
    
    rclcpp::TimerBase::SharedPtr health_check_timer_;
    
    // Subscription members
    rclcpp::Subscription<px4_msgs::msg::VehicleAttitude>::SharedPtr attitude_sub_;
    rclcpp::Subscription<px4_msgs::msg::VehicleAngularVelocity>::SharedPtr angular_velocity_sub_;
    rclcpp::Subscription<px4_msgs::msg::VehicleLocalPosition>::SharedPtr local_position_sub_;
    rclcpp::Subscription<px4_msgs::msg::VehicleOdometry>::SharedPtr odometry_sub_;
    rclcpp::Subscription<px4_msgs::msg::ActuatorMotors>::SharedPtr actuator_motors_sub_;
    rclcpp::Subscription<px4_msgs::msg::ActuatorServos>::SharedPtr actuator_servos_sub_;
    rclcpp::Subscription<erocket::msg::FlightMode>::SharedPtr flight_mode_sub_;
    
    void setup_subscriptions() {
        auto qos = rclcpp::QoS(rclcpp::SensorDataQoSProfile());
        
        // PX4 input topics
        attitude_sub_ = create_subscription<px4_msgs::msg::VehicleAttitude>(
            "/fmu/out/vehicle_attitude", qos,
            [this](const px4_msgs::msg::VehicleAttitude::SharedPtr msg) {
                topic_health_["/fmu/out/vehicle_attitude"].update(msg->timestamp);
            });
        
        angular_velocity_sub_ = create_subscription<px4_msgs::msg::VehicleAngularVelocity>(
            "/fmu/out/vehicle_angular_velocity", qos,
            [this](const px4_msgs::msg::VehicleAngularVelocity::SharedPtr msg) {
                topic_health_["/fmu/out/vehicle_angular_velocity"].update(msg->timestamp);
            });
        
        local_position_sub_ = create_subscription<px4_msgs::msg::VehicleLocalPosition>(
            "/fmu/out/vehicle_local_position", qos,
            [this](const px4_msgs::msg::VehicleLocalPosition::SharedPtr msg) {
                topic_health_["/fmu/out/vehicle_local_position"].update(msg->timestamp);
            });
        
        odometry_sub_ = create_subscription<px4_msgs::msg::VehicleOdometry>(
            "/fmu/out/vehicle_odometry", qos,
            [this](const px4_msgs::msg::VehicleOdometry::SharedPtr msg) {
                topic_health_["/fmu/out/vehicle_odometry"].update(msg->timestamp);
            });
        
        // ROS2 output topics
        actuator_motors_sub_ = create_subscription<px4_msgs::msg::ActuatorMotors>(
            "/fmu/in/actuator_motors", qos,
            [this](const px4_msgs::msg::ActuatorMotors::SharedPtr msg) {
                topic_health_["/fmu/in/actuator_motors"].update(msg->timestamp);
            });
        
        actuator_servos_sub_ = create_subscription<px4_msgs::msg::ActuatorServos>(
            "/fmu/in/actuator_servos", qos,
            [this](const px4_msgs::msg::ActuatorServos::SharedPtr msg) {
                topic_health_["/fmu/in/actuator_servos"].update(msg->timestamp);
            });
        
        flight_mode_sub_ = create_subscription<erocket::msg::FlightMode>(
            "offboard/flight_mode/get", qos,
            [this](const erocket::msg::FlightMode::SharedPtr msg) {
                topic_health_["offboard/flight_mode/get"].update(
                    msg->stamp.sec * 1000000ULL + msg->stamp.nanosec / 1000);
            });
        
        // Initialize topic health objects
        topic_health_["/fmu/out/vehicle_attitude"].topic_name = "/fmu/out/vehicle_attitude";
        topic_health_["/fmu/out/vehicle_attitude"].max_message_age_ms = max_message_age_ms_;
        topic_health_["/fmu/out/vehicle_attitude"].min_expected_frequency_hz = 100;  // PX4 publishes at ~100 Hz
        
        topic_health_["/fmu/out/vehicle_angular_velocity"].topic_name = "/fmu/out/vehicle_angular_velocity";
        topic_health_["/fmu/out/vehicle_angular_velocity"].max_message_age_ms = max_message_age_ms_;
        topic_health_["/fmu/out/vehicle_angular_velocity"].min_expected_frequency_hz = 100;
        
        topic_health_["/fmu/out/vehicle_local_position"].topic_name = "/fmu/out/vehicle_local_position";
        topic_health_["/fmu/out/vehicle_local_position"].max_message_age_ms = max_message_age_ms_;
        topic_health_["/fmu/out/vehicle_local_position"].min_expected_frequency_hz = 50;
        
        topic_health_["/fmu/out/vehicle_odometry"].topic_name = "/fmu/out/vehicle_odometry";
        topic_health_["/fmu/out/vehicle_odometry"].max_message_age_ms = max_message_age_ms_;
        topic_health_["/fmu/out/vehicle_odometry"].min_expected_frequency_hz = 50;
        
        topic_health_["/fmu/in/actuator_motors"].topic_name = "/fmu/in/actuator_motors";
        topic_health_["/fmu/in/actuator_motors"].max_message_age_ms = max_message_age_ms_;
        topic_health_["/fmu/in/actuator_motors"].min_expected_frequency_hz = 50;
        
        topic_health_["/fmu/in/actuator_servos"].topic_name = "/fmu/in/actuator_servos";
        topic_health_["/fmu/in/actuator_servos"].max_message_age_ms = max_message_age_ms_;
        topic_health_["/fmu/in/actuator_servos"].min_expected_frequency_hz = 50;
        
        topic_health_["offboard/flight_mode/get"].topic_name = "offboard/flight_mode/get";
        topic_health_["offboard/flight_mode/get"].max_message_age_ms = 1000;  // Less critical
        topic_health_["offboard/flight_mode/get"].min_expected_frequency_hz = 1;
    }
    
    void check_health_and_report() {
        bool any_unhealthy = false;
        
        for (auto& [topic_name, health] : topic_health_) {
            // Check if topic has gone silent
            if (health.message_count > 0 && health.is_stale(std::chrono::milliseconds(stale_topic_timeout_ms_))) {
                RCLCPP_ERROR(get_logger(),
                    "TOPIC STALE: %s (last message %.1f seconds ago)",
                    topic_name.c_str(),
                    std::chrono::duration<double>(
                        std::chrono::system_clock::now() - health.last_update_time).count());
                any_unhealthy = true;
            }
            
            // Log current status
            if (!health.is_healthy) {
                RCLCPP_WARN(get_logger(), "%s", health.get_status_string().c_str());
                any_unhealthy = true;
            } else if (health.message_count > 0) {
                RCLCPP_DEBUG(get_logger(), "%s", health.get_status_string().c_str());
            }
        }
        
        if (any_unhealthy) {
            RCLCPP_WARN(get_logger(), "=== SYSTEM HEALTH CHECK: ISSUES DETECTED ===");
        }
    }
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SystemMonitor>());
    rclcpp::shutdown();
    return 0;
}
