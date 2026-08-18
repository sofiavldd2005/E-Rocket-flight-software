#include <rclcpp/rclcpp.hpp>

<<<<<<< HEAD
#include <erocket/constants.hpp>
#include <erocket/msg/flight_mode.hpp>

=======
>>>>>>> dbe42f2 (1. Documentation explaining Simulink Codegen and troubleshooting)
#include <chrono>
#include <erocket/constants.hpp>
#include <erocket/msg/flight_mode.hpp>
#include <iostream>
<<<<<<< HEAD
=======
#include <px4_msgs/msg/actuator_armed.hpp>
#include <px4_msgs/msg/vehicle_status.hpp>
>>>>>>> dbe42f2 (1. Documentation explaining Simulink Codegen and troubleshooting)
#include <stdint.h>
#include <string>

using namespace std::chrono;
using namespace std::chrono_literals;
using namespace erocket::msg;
using namespace erocket::constants::flight_mode;

/**
 * @brief PX4 ROS2 Communication Node is responsible for sending and receiving
 * commands to and from the PX4.
 */
class MockFlightMode : public rclcpp::Node {
public:
  MockFlightMode()
      : Node("mock_flight_mode"), qos_profile_{rmw_qos_profile_sensor_data},
        qos_{rclcpp::QoS(rclcpp::QoSInitialization(qos_profile_.history, 5),
                         qos_profile_)},
        flight_mode_set_subscriber_{
            this->create_subscription<erocket::msg::FlightMode>(
                FLIGHT_MODE_SET_TOPIC, qos_,
                std::bind(&MockFlightMode::handle_flight_mode_set, this,
                          std::placeholders::_1))},
        flight_mode_get_publisher_{
            this->create_publisher<erocket::msg::FlightMode>(
<<<<<<< HEAD
                FLIGHT_MODE_GET_TOPIC, qos_)} {}

private:
  rmw_qos_profile_t qos_profile_;
  rclcpp::QoS qos_;

=======
                FLIGHT_MODE_GET_TOPIC, qos_)},
        vehicle_status_sub_{
            this->create_subscription<px4_msgs::msg::VehicleStatus>(
                "/fmu/out/vehicle_status", qos_,
                std::bind(&MockFlightMode::handle_vehicle_status, this,
                          std::placeholders::_1))},
        actuator_armed_sub_{
            this->create_subscription<px4_msgs::msg::ActuatorArmed>(
                "/fmu/out/actuator_armed", qos_,
                std::bind(&MockFlightMode::handle_actuator_armed, this,
                          std::placeholders::_1))} {}

private:
  std::atomic<uint8_t> nav_state_{0};
  std::atomic<uint8_t> arming_state_{0};
  std::atomic<bool> armed_{false};
  uint8_t last_nav_state_{0xFF};
  bool last_armed_{false};

  rmw_qos_profile_t qos_profile_;
  rclcpp::QoS qos_;
  const char *nav_state_to_string(uint8_t nav_state);
>>>>>>> dbe42f2 (1. Documentation explaining Simulink Codegen and troubleshooting)
  rclcpp::Subscription<erocket::msg::FlightMode>::SharedPtr
      flight_mode_set_subscriber_;
  rclcpp::Publisher<erocket::msg::FlightMode>::SharedPtr
      flight_mode_get_publisher_;
  void handle_flight_mode_set(
      const std::shared_ptr<erocket::msg::FlightMode> flight_mode_set_message);
<<<<<<< HEAD
};

// mission node publishes a request to the FLIGHT_MODE_SET_TOPIC,
// this node echoes that exact same mode back onto the FLIGHT_MODE_GET_TOPIC.
// It confirms every single command, allowing the mission.cpp state machine to
// progress through TAKE_OFF, IN_MISSION, and LANDING
=======
  rclcpp::Subscription<px4_msgs::msg::VehicleStatus>::SharedPtr
      vehicle_status_sub_;
  void handle_vehicle_status(const px4_msgs::msg::VehicleStatus::SharedPtr msg);

  rclcpp::Subscription<px4_msgs::msg::ActuatorArmed>::SharedPtr
      actuator_armed_sub_;
  void handle_actuator_armed(const px4_msgs::msg::ActuatorArmed::SharedPtr msg);
};
rclcpp::Subscription<px4_msgs::msg::VehicleStatus>::SharedPtr
    vehicle_status_sub_;
>>>>>>> dbe42f2 (1. Documentation explaining Simulink Codegen and troubleshooting)
void MockFlightMode::handle_flight_mode_set(
    const std::shared_ptr<erocket::msg::FlightMode> flight_mode_set) {
  erocket::msg::FlightMode msg{};

  msg.flight_mode = flight_mode_set->flight_mode;
  msg.stamp = this->get_clock()->now();

  flight_mode_get_publisher_->publish(msg);
}

<<<<<<< HEAD
int main(int argc, char *argv[]) {
  std::cout << "Starting Mock Flight Mode node..." << std::endl;
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);

=======
void MockFlightMode::handle_vehicle_status(
    const px4_msgs::msg::VehicleStatus::SharedPtr msg) {
  nav_state_.store(msg->nav_state);
  arming_state_.store(msg->arming_state);

  if (msg->nav_state != last_nav_state_) {
    RCLCPP_INFO(this->get_logger(),
                "PX4 nav_state -> %u (%s), arming_state -> %u", msg->nav_state,
                nav_state_to_string(msg->nav_state), msg->arming_state);
    last_nav_state_ = msg->nav_state;
  }
}

void MockFlightMode::handle_actuator_armed(
    const px4_msgs::msg::ActuatorArmed::SharedPtr msg) {
  armed_.store(msg->armed);

  if (msg->armed != last_armed_) {
    RCLCPP_INFO(this->get_logger(), "PX4 armed -> %s",
                msg->armed ? "true" : "false");
    last_armed_ = msg->armed;
  }
}

const char *MockFlightMode::nav_state_to_string(uint8_t nav_state) {
  switch (nav_state) {
  case px4_msgs::msg::VehicleStatus::NAVIGATION_STATE_MANUAL:
    return "MANUAL";
  case px4_msgs::msg::VehicleStatus::NAVIGATION_STATE_ALTCTL:
    return "ALTCTL";
  case px4_msgs::msg::VehicleStatus::NAVIGATION_STATE_POSCTL:
    return "POSCTL";
  case px4_msgs::msg::VehicleStatus::NAVIGATION_STATE_AUTO_MISSION:
    return "AUTO_MISSION";
  case px4_msgs::msg::VehicleStatus::NAVIGATION_STATE_AUTO_LOITER:
    return "AUTO_LOITER";
  case px4_msgs::msg::VehicleStatus::NAVIGATION_STATE_AUTO_RTL:
    return "AUTO_RTL";
  case px4_msgs::msg::VehicleStatus::NAVIGATION_STATE_ACRO:
    return "ACRO";
  case px4_msgs::msg::VehicleStatus::NAVIGATION_STATE_DESCEND:
    return "DESCEND";
  case px4_msgs::msg::VehicleStatus::NAVIGATION_STATE_TERMINATION:
    return "TERMINATION";
  case px4_msgs::msg::VehicleStatus::NAVIGATION_STATE_OFFBOARD:
    return "OFFBOARD";
  case px4_msgs::msg::VehicleStatus::NAVIGATION_STATE_STAB:
    return "STAB";
  case px4_msgs::msg::VehicleStatus::NAVIGATION_STATE_AUTO_TAKEOFF:
    return "AUTO_TAKEOFF";
  case px4_msgs::msg::VehicleStatus::NAVIGATION_STATE_AUTO_LAND:
    return "AUTO_LAND";
  default:
    return "UNKNOWN";
  }
}

int main(int argc, char *argv[]) {
  std::cout << "Starting Mock Flight Mode node..." << std::endl;
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);

>>>>>>> dbe42f2 (1. Documentation explaining Simulink Codegen and troubleshooting)
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MockFlightMode>());

  rclcpp::shutdown();
  return 0;
}
