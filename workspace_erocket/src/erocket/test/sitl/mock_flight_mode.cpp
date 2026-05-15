#include <rclcpp/rclcpp.hpp>

#include <erocket/constants.hpp>
#include <erocket/msg/flight_mode.hpp>

#include <iostream>
#include <stdint.h>
#include <string>

using namespace std::chrono;
using namespace std::chrono_literals;
using namespace erocket::msg;
using namespace erocket::constants::flight_mode;

/**
 * @brief SITL simulation node that bypasses physical hardware arming checks.
 * It immediately echoes back requested flight modes to simulate successful
 * transitions.
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
                FLIGHT_MODE_GET_TOPIC, qos_)} {}

private:
  rmw_qos_profile_t qos_profile_;
  rclcpp::QoS qos_;

  rclcpp::Subscription<erocket::msg::FlightMode>::SharedPtr
      flight_mode_set_subscriber_;
  rclcpp::Publisher<erocket::msg::FlightMode>::SharedPtr
      flight_mode_get_publisher_;
  void handle_flight_mode_set(
      const std::shared_ptr<erocket::msg::FlightMode> flight_mode_set_message);
};

// mission node publishes a request to the FLIGHT_MODE_SET_TOPIC,
// this node echoes that exact same mode back onto the FLIGHT_MODE_GET_TOPIC.
// It confirms every single command, allowing the mission.cpp state machine to
// progress through TAKE_OFF, IN_MISSION, and LANDING
void MockFlightMode::handle_flight_mode_set(
    const std::shared_ptr<erocket::msg::FlightMode> flight_mode_set) {
  erocket::msg::FlightMode msg{};

  msg.flight_mode = flight_mode_set->flight_mode;
  msg.stamp = this->get_clock()->now();

  flight_mode_get_publisher_->publish(msg);
}

int main(int argc, char *argv[]) {
  std::cout << "Starting Mock Flight Mode node..." << std::endl;
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);

  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MockFlightMode>());

  rclcpp::shutdown();
  return 0;
}
