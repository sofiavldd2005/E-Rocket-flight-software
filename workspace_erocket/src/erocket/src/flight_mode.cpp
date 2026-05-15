#include <rclcpp/rclcpp.hpp>

#include <erocket/constants.hpp>
#include <erocket/msg/flight_mode.hpp>
#include <px4_msgs/msg/offboard_control_mode.hpp>
#include <px4_msgs/msg/vehicle_control_mode.hpp>
#include <px4_msgs/srv/vehicle_command.hpp>

#include <chrono>
#include <iostream>
#include <stdint.h>
#include <string>

using namespace std::chrono;
using namespace std::chrono_literals;
using namespace erocket::constants::flight_mode;
using namespace erocket::constants::flight_mode;

/**
 * @class FlightMode
 * @brief PX4 ROS 2 Communication Node responsible for sending and receiving
 * commands to and from the PX4.
 *
 * Manages flight state transitions (e.g., ARM, TAKE_OFF, ABORT) by interfacing
 * with the PX4 autopilot through offboard control and vehicle commands.
 */
class FlightMode : public rclcpp::Node {
public:
  /**
   * @brief Construct a new Flight Mode node
   *
   * Initializes publishers, subscribers, clients, and timers required for
   * communicating with PX4. Also requests an initial transition to manual mode.
   */
  FlightMode()
      : Node("flight_mode"),
        flight_mode_current_{erocket::msg::FlightMode::INIT},
        flight_mode_requested_{erocket::msg::FlightMode::INIT},
        qos_profile_{rmw_qos_profile_sensor_data},
        qos_{rclcpp::QoS(rclcpp::QoSInitialization(qos_profile_.history, 5),
                         qos_profile_)},
        vehicle_command_client_{
            this->create_client<px4_msgs::srv::VehicleCommand>(
                "/fmu/vehicle_command")},
        flight_mode_set_subscriber_{
            this->create_subscription<erocket::msg::FlightMode>(
                FLIGHT_MODE_SET_TOPIC, qos_,
                std::bind(&FlightMode::handle_flight_mode_set, this,
                          std::placeholders::_1))},
        flight_mode_get_publisher_{
            this->create_publisher<erocket::msg::FlightMode>(
                FLIGHT_MODE_GET_TOPIC, qos_)},
        offboard_control_mode_publisher_{
            this->create_publisher<px4_msgs::msg::OffboardControlMode>(
                "/fmu/in/offboard_control_mode", 10)},
        mantain_offboard_mode_timer_{this->create_wall_timer(
            std::chrono::duration<double>(
                MANTAIN_OFFBOARD_MODE_TIMER_PERIOD_SECONDS),
            std::bind(&FlightMode::publish_offboard_control_mode, this))},
        vehicle_control_mode_publisher_{
            this->create_publisher<px4_msgs::msg::VehicleControlMode>(
                "/fmu/in/vehicle_control_mode", 10)} {
    while (!vehicle_command_client_->wait_for_service(1s)) {
      RCLCPP_WARN(this->get_logger(),
                  "Vehicle Command Service (PX4) is unavailable");
    }

    switch_to_manual_mode();
    RCLCPP_INFO(this->get_logger(), "Switching to manual mode");
  }

private:
  std::atomic<uint8_t> flight_mode_current_; ///< Currently active flight mode
  std::atomic<uint8_t>
      flight_mode_requested_; ///< The flight mode requested to transition into

  rmw_qos_profile_t qos_profile_; ///< QoS profile for PX4 communications
  rclcpp::QoS qos_;               ///< ROS 2 QoS object

  void switch_to_offboard_mode();
  void switch_to_manual_mode();
  void arm();
  void disarm();
  void terminate_flight();

  rclcpp::Client<px4_msgs::srv::VehicleCommand>::SharedPtr
      vehicle_command_client_;
  void send_vehicle_command_request(const uint16_t command,
                                    const double param1 = 0.0,
                                    const double param2 = 0.0);
  void handle_vehicle_command_response(
      rclcpp::Client<px4_msgs::srv::VehicleCommand>::SharedFuture future);

  rclcpp::Subscription<erocket::msg::FlightMode>::SharedPtr
      flight_mode_set_subscriber_;
  void handle_flight_mode_set(
      const std::shared_ptr<erocket::msg::FlightMode> flight_mode_set_message);

  rclcpp::Publisher<erocket::msg::FlightMode>::SharedPtr
      flight_mode_get_publisher_;
  void publish_flight_mode();

  rclcpp::Publisher<px4_msgs::msg::OffboardControlMode>::SharedPtr
      offboard_control_mode_publisher_;
  rclcpp::TimerBase::SharedPtr mantain_offboard_mode_timer_;
  void publish_offboard_control_mode();

  rclcpp::Publisher<px4_msgs::msg::VehicleControlMode>::SharedPtr
      vehicle_control_mode_publisher_;
  void publish_vehicle_control_mode();
};

/**
 * @brief Send a vehicle command to switch PX4 to offboard control mode.
 */
void FlightMode::switch_to_offboard_mode() {
  send_vehicle_command_request(
      px4_msgs::msg::VehicleCommand::VEHICLE_CMD_DO_SET_MODE, 1, 6);
  RCLCPP_INFO(this->get_logger(), "Offboard mode command sent");
}

/**
 * @brief Send a vehicle command to switch PX4 to manual control mode.
 */
void FlightMode::switch_to_manual_mode() {
  send_vehicle_command_request(
      px4_msgs::msg::VehicleCommand::VEHICLE_CMD_DO_SET_MODE, 1, 1);
  RCLCPP_INFO(this->get_logger(), "Manual mode command sent");
}

/**
 * @brief Send a vehicle command to arm the vehicle.
 */
void FlightMode::arm() {
  send_vehicle_command_request(
      px4_msgs::msg::VehicleCommand::VEHICLE_CMD_COMPONENT_ARM_DISARM, 1.0);
  RCLCPP_INFO(this->get_logger(), "Arm command sent");
}

/**
 * @brief Send a vehicle command to disarm the vehicle.
 */
void FlightMode::disarm() {
  send_vehicle_command_request(
      px4_msgs::msg::VehicleCommand::VEHICLE_CMD_COMPONENT_ARM_DISARM, 0.0,
      21196);
  RCLCPP_INFO(this->get_logger(), "Disarm command sent");
}

/**
 * @brief Send a vehicle command to terminate the flight immediately.
 */
void FlightMode::terminate_flight() {
  send_vehicle_command_request(
      px4_msgs::msg::VehicleCommand::VEHICLE_CMD_DO_FLIGHTTERMINATION, 1);
  RCLCPP_INFO(this->get_logger(), "Terminate Flight command sent");
}

/**
 * @brief Callback handling requests to change the current flight mode.
 *
 * @param flight_mode_set The received message containing the requested flight
 * mode.
 */
void FlightMode::handle_flight_mode_set(
    const std::shared_ptr<erocket::msg::FlightMode> flight_mode_set) {
  if (!vehicle_command_client_->wait_for_service(1s)) {
    RCLCPP_WARN(this->get_logger(),
                "Vehicle Command Service (PX4) is unavailable");
    return;
  }

  auto flight_mode_current = flight_mode_current_.load();
  auto flight_mode_requested = flight_mode_set->flight_mode;
  flight_mode_requested_.store(flight_mode_requested);

  if (flight_mode_current == erocket::msg::FlightMode::INIT &&
      flight_mode_requested == erocket::msg::FlightMode::PRE_ARM) {
    RCLCPP_INFO(this->get_logger(),
                "Received request to change flight mode to PRE_ARM");
    switch_to_offboard_mode();
  } else if (flight_mode_current == erocket::msg::FlightMode::PRE_ARM &&
             flight_mode_requested == erocket::msg::FlightMode::ARM) {
    RCLCPP_INFO(this->get_logger(),
                "Received request to change flight mode to ARM");
    arm();
  } else if (flight_mode_current == erocket::msg::FlightMode::ARM &&
             flight_mode_requested == erocket::msg::FlightMode::TAKE_OFF) {
    RCLCPP_INFO(this->get_logger(),
                "Received request to change flight mode to TAKE_OFF");

    // no need to change PX4 internal state
    publish_vehicle_control_mode();
    flight_mode_current_.store(erocket::msg::FlightMode::TAKE_OFF);
    publish_flight_mode();
  } else if (flight_mode_current == erocket::msg::FlightMode::TAKE_OFF &&
             flight_mode_requested == erocket::msg::FlightMode::IN_MISSION) {
    RCLCPP_INFO(this->get_logger(),
                "Received request to change flight mode to IN_MISSION");

    // no need to change PX4 internal state
    flight_mode_current_.store(erocket::msg::FlightMode::IN_MISSION);
    publish_flight_mode();
  } else if ((flight_mode_current == erocket::msg::FlightMode::TAKE_OFF ||
              flight_mode_current == erocket::msg::FlightMode::IN_MISSION) &&
             flight_mode_requested == erocket::msg::FlightMode::LANDING) {
    RCLCPP_INFO(this->get_logger(),
                "Received request to change flight mode to LANDING");

    // no need to change PX4 internal state
    flight_mode_current_.store(erocket::msg::FlightMode::LANDING);
    publish_flight_mode();
  } else if (flight_mode_current == erocket::msg::FlightMode::LANDING &&
             flight_mode_requested ==
                 erocket::msg::FlightMode::MISSION_COMPLETE) {
    RCLCPP_INFO(this->get_logger(),
                "Received request to change flight mode to MISSION_COMPLETE");
    disarm();
  } else if (flight_mode_requested == erocket::msg::FlightMode::ABORT) {
    RCLCPP_INFO(this->get_logger(),
                "Received request to change flight mode to ABORT");
    flight_mode_current_.store(erocket::msg::FlightMode::ABORT);
    publish_flight_mode();
    disarm();
  } else if (flight_mode_current == erocket::msg::FlightMode::ABORT) {
    rclcpp::shutdown();
  } else {
    RCLCPP_ERROR(this->get_logger(),
                 "Received invalid request to change flight mode");
  }
}

/**
 * @brief Handles the response from a vehicle command request to PX4.
 *
 * Validates whether the command was accepted or rejected, and updates the
 * state.
 *
 * @param future The shared future containing the vehicle command response.
 */
void FlightMode::handle_vehicle_command_response(
    rclcpp::Client<px4_msgs::srv::VehicleCommand>::SharedFuture future) {
  auto vehicle_command_response = future.get();
  auto reply = vehicle_command_response->reply;

  // if request is success
  if (reply.result == reply.VEHICLE_CMD_RESULT_ACCEPTED) {
    flight_mode_current_.store(flight_mode_requested_.load());
    RCLCPP_INFO(this->get_logger(), "Flight Mode change successful!");
  } else {
    flight_mode_requested_.store(flight_mode_current_.load());
    RCLCPP_WARN(this->get_logger(),
                "Flight Mode change unsuccessful! Error: %d", reply.result);
  }

  publish_flight_mode();
}

/**
 * @brief Publishes the current flight mode to other ROS 2 nodes.
 */
void FlightMode::publish_flight_mode() {
  erocket::msg::FlightMode msg{};

  msg.flight_mode = flight_mode_current_.load();
  msg.stamp = this->get_clock()->now();

  flight_mode_get_publisher_->publish(msg);
}

/**
 * @brief Sends a generic vehicle command request to PX4.
 *
 * @param command The MAVLink vehicle command ID.
 * @param param1 Command parameter 1 (default 0.0).
 * @param param2 Command parameter 2 (default 0.0).
 */
void FlightMode::send_vehicle_command_request(const uint16_t command,
                                              const double param1,
                                              const double param2) {
  auto request = std::make_shared<px4_msgs::srv::VehicleCommand::Request>();

  px4_msgs::msg::VehicleCommand msg{};
  msg.param1 = param1;
  msg.param2 = param2;
  msg.command = command;
  msg.target_system = 1;
  msg.target_component = 1;
  msg.source_system = 1;
  msg.source_component = 1;
  msg.from_external = true;
  msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;

  request->request = msg;

  // Send the request asynchronously
  vehicle_command_client_->async_send_request(
      request, std::bind(&FlightMode::handle_vehicle_command_response, this,
                         std::placeholders::_1));
}

/**
 * @brief Publish the offboard control mode.
 *        For this example, only direct actuator is active.
 */
void FlightMode::publish_offboard_control_mode() {
  px4_msgs::msg::OffboardControlMode msg{};
  msg.position = false;
  msg.velocity = false;
  msg.acceleration = false;
  msg.attitude = false;
  msg.body_rate = false;
  msg.thrust_and_torque = false;
  msg.direct_actuator = true;
  msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
  offboard_control_mode_publisher_->publish(msg);
}

/**
 * @brief Publish the vehicle control mode.
 *        For this example, we are setting the vehicle to offboard mode.
 */
void FlightMode::publish_vehicle_control_mode() {
  px4_msgs::msg::VehicleControlMode msg{};
  msg.timestamp = this->get_clock()->now().nanoseconds() / 1000;
  msg.flag_armed = true;

  msg.flag_multicopter_position_control_enabled = false;

  msg.flag_control_manual_enabled = false;
  msg.flag_control_auto_enabled = false;
  msg.flag_control_offboard_enabled = true;
  msg.flag_control_position_enabled = false;
  msg.flag_control_velocity_enabled = false;
  msg.flag_control_altitude_enabled = false;
  msg.flag_control_climb_rate_enabled = false;
  msg.flag_control_acceleration_enabled = false;
  msg.flag_control_attitude_enabled = false;
  msg.flag_control_rates_enabled = false;
  msg.flag_control_allocation_enabled = false;
  msg.flag_control_termination_enabled = false;

  msg.source_id = 1;

  vehicle_control_mode_publisher_->publish(msg);
  RCLCPP_INFO(this->get_logger(), "Vehicle control mode command sent");
}

int main(int argc, char *argv[]) {
  std::cout << "Starting PX4 ROS2 Flight Mode node..." << std::endl;
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);

  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<FlightMode>());

  rclcpp::shutdown();
  return 0;
}
