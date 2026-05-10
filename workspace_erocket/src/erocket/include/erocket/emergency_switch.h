#pragma once
#include <erocket/constants.hpp>
#include <px4_msgs/msg/actuator_armed.hpp>
#include <rclcpp/rclcpp.hpp>

using namespace erocket::constants::emergency;
using namespace px4_msgs::msg;

class EmergencySwitch {
public:
  EmergencySwitch(rclcpp::Node *node, rclcpp::QoS qos)
      : // // It creates a ROS 2 subscription to the PX4 flight controller's
        // ActuatorArmed topic
        actuator_armed_sub_{node->create_subscription<ActuatorArmed>(
            EMERGENCY_ACTUATOR_ARMED, qos,
            [this](const ActuatorArmed::SharedPtr msg) {
              if (msg->manual_lockdown) { //"Kill" toggle on their RC radio, the
                                          //PX4 flight controller sets the
                // manual_lockdown flag, on it its internal state. This ROS node
                // listens for that flag:
                emergency_switch_on_ = true;
                RCLCPP_WARN(logger_, "Emergency switch is ON!");
              }
            })} {}

  bool emergency_switch_on() { return emergency_switch_on_; }
  // If mission.cpp (which includes this header) sees emergency_switch_on()
  // return true, it requests FlightMode::ABORT.
private:
  rclcpp::Logger logger_{rclcpp::get_logger("EmergencySwitch")};
  rclcpp::Subscription<ActuatorArmed>::SharedPtr actuator_armed_sub_;
  bool emergency_switch_on_{false};
};
