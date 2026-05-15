#include <rclcpp/rclcpp.hpp>
#include <erocket/msg/flight_mode.hpp>
#include <erocket/constants.hpp>
#include <erocket/frame_transforms.h>

#include <erocket/vehicle_constants.hpp>
#include <erocket/controller/allocator.hpp>
#include <erocket/controller/state.hpp>
#include <erocket/controller/setpoint.hpp>
#include <erocket/controller/impls/generic_controller.hpp>

#include <chrono>

using namespace std::chrono;
using namespace px4_msgs::msg;
using namespace geometry_msgs::msg;
using namespace erocket::msg;
using namespace erocket::constants;
using namespace erocket::constants::setpoint;
using namespace erocket::constants::controller_generic;
using namespace erocket::constants::flight_mode;
using namespace erocket::frame_transforms;

/**
 * @class GenericControllerNode
 * @brief ROS 2 node that runs a generic, user-defined control system.
 * 
 * This node provides a template for implementing custom control algorithms. 
 * It interfaces with the state and setpoint aggregators, and allocates thrust/actuation commands
 * based on the current flight mode, similar to the baseline PID controller.
 */
class GenericControllerNode : public rclcpp::Node
{
public:
    /**
     * @brief Construct a new Generic Controller Node
     * 
     * Initializes the publishers, subscribers, timers, and the generic controller.
     * The control loop frequency is loaded from ROS parameters.
     */
	explicit GenericControllerNode() : Node("generic_controller"),
    qos_profile_{rmw_qos_profile_sensor_data},
    qos_{rclcpp::QoS(rclcpp::QoSInitialization(qos_profile_.history, 5), qos_profile_)},

    vehicle_constants_{std::make_shared<VehicleConstants>(this)},
    state_aggregator_{std::make_unique<StateAggregator>(this, qos_)},
    setpoint_aggregator_{std::make_unique<SetpointAggregator>(this, qos_)},
    allocator_{std::make_unique<Allocator>(this, qos_, vehicle_constants_)},
    generic_controller_{this, qos_, state_aggregator_, setpoint_aggregator_, vehicle_constants_},

    flight_mode_{FlightMode::INIT},
    flight_mode_get_subscriber_{this->create_subscription<FlightMode>(
        FLIGHT_MODE_GET_TOPIC, qos_, 
        [this](const FlightMode::SharedPtr msg) {
            flight_mode_ = msg->flight_mode;
        }
    )}
    {
        double controller_freq = this->get_parameter(CONTROLLER_GENERIC_FREQUENCY_HERTZ_PARAM).as_double();
        double controller_period = 1.0 / controller_freq;

        // Controller timer at fixed frequency of controller
        controller_timer_ = this->create_wall_timer(
            std::chrono::duration<double>(controller_period), 
            std::bind(&GenericControllerNode::controller_callback, this)
        );
	}

private:
    rmw_qos_profile_t qos_profile_; ///< Quality of Service profile for sensor data
    rclcpp::QoS qos_;               ///< ROS 2 QoS object based on qos_profile_

    std::shared_ptr<VehicleConstants> vehicle_constants_;     ///< Shared pointer to vehicle-specific physical constants
    std::shared_ptr<StateAggregator> state_aggregator_;       ///< Aggregates vehicle state data from sensors/estimators
    std::shared_ptr<SetpointAggregator> setpoint_aggregator_; ///< Aggregates target setpoints from mission logic
    std::unique_ptr<Allocator> allocator_;                    ///< Allocates calculated torques/thrusts to physical actuators
    GenericController generic_controller_;                    ///< Instance of the generic custom controller algorithm

    //!< Time variables
    rclcpp::TimerBase::SharedPtr controller_timer_; ///< Timer triggering the main controller loop

	//!< Auxiliary functions
    void controller_callback();

    std::atomic<uint8_t> flight_mode_; ///< Thread-safe atomic variable storing the current flight mode
    rclcpp::Subscription<FlightMode>::SharedPtr flight_mode_get_subscriber_; ///< Subscription to flight mode updates
};

/**
 * @brief Main control loop callback executed periodically by controller_timer_.
 * 
 * Computes necessary actuation commands based on the current flight mode (e.g., ARM, 
 * TAKE_OFF, IN_MISSION, LANDING) using the generic controller and sends commands to the allocator.
 */
void GenericControllerNode::controller_callback()
{
    if (flight_mode_ < FlightMode::ARM) {
        allocator_->compute_allocation_neutral();
        return;
    }

    else if (flight_mode_ == FlightMode::ARM) {
        static rclcpp::Time t0 = this->get_clock()->now();
        rclcpp::Time now = this->get_clock()->now();

        auto inner_servo_tilt_angle_radians = 0.0f;
        auto outer_servo_tilt_angle_radians = 0.0f;
        auto upwards_motor_thrust_pwm = NAN;
        auto downwards_motor_thrust_pwm = NAN;

        if (now - t0 < 3.0s) {
            auto max_servo_tilt_angle_radians = degrees_to_radians(vehicle_constants_->servo_max_tilt_angle_degrees_);
            inner_servo_tilt_angle_radians = sin(2.0 * M_PI * (now - t0).seconds()) * max_servo_tilt_angle_radians;
            outer_servo_tilt_angle_radians = sin(2.0 * M_PI * (now - t0).seconds() + M_PI_2) * max_servo_tilt_angle_radians;

        }

        if (now - t0 > 4.0s) {
            // Allocate motor thrust based on the computed torque
            upwards_motor_thrust_pwm = 0.0f;
            downwards_motor_thrust_pwm = 0.0f;
        }

        allocator_->indirect_actuation(
            inner_servo_tilt_angle_radians,
            outer_servo_tilt_angle_radians,
            upwards_motor_thrust_pwm,
            downwards_motor_thrust_pwm
        );
        return;
    }

    // only run controller when in mission
    else if (flight_mode_ == FlightMode::TAKE_OFF || flight_mode_ == FlightMode::IN_MISSION || flight_mode_ == FlightMode::LANDING) {
        auto allocator_input = generic_controller_.compute();
        allocator_->compute_allocation(allocator_input);
        return;
    }

    else if (flight_mode_ == FlightMode::ABORT) {
        allocator_->compute_allocation_neutral();

        rclcpp::shutdown();
        return;
    }
}

int main(int argc, char *argv[])
{
	std::cout << "Starting offboard baseline pid controller node..." << std::endl;
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);
	rclcpp::init(argc, argv);
	rclcpp::spin(std::make_shared<GenericControllerNode>());

	rclcpp::shutdown();
	return 0;
}
