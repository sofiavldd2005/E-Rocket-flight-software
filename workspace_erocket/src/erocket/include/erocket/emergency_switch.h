#pragma once
#include <erocket/constants.hpp>
#include <px4_msgs/msg/actuator_armed.hpp>
#include <rclcpp/rclcpp.hpp>

using namespace erocket::constants::emergency;
using namespace px4_msgs::msg;

/**
 * @class EmergencySwitch
 * @brief Subscribes to the actuator armed status to monitor for a manual lockdown (kill switch).
 */
class EmergencySwitch {
public:
<<<<<<< HEAD
  /**
   * @brief Construct a new Emergency Switch
   * 
   * @param node The parent ROS 2 node.
   * @param qos The QoS profile used for the subscription.
   */
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
=======
  EmergencySwitch(rclcpp::Node *node, rclcpp::QoS qos)
      : actuator_armed_sub_{node->create_subscription<ActuatorArmed>(
            EMERGENCY_ACTUATOR_ARMED, qos,
            [this](const ActuatorArmed::SharedPtr msg) {
              if (msg->manual_lockdown || msg->force_failsafe ||
                  (msg->lockdown && msg->armed)) {
                emergency_switch_on_.store(true);
                RCLCPP_WARN(logger_,
                            "Emergency switch ON: manual_lockdown=%d "
                            "force_failsafe=%d lockdown=%d armed=%d",
                            msg->manual_lockdown, msg->force_failsafe,
                            msg->lockdown, msg->armed);
              }
            })} {}

  bool emergency_switch_on() { return emergency_switch_on_; }
>>>>>>> dbe42f2 (1. Documentation explaining Simulink Codegen and troubleshooting)

  /**
   * @brief Check if the emergency switch has been triggered.
   * @return true if manual lockdown is active, false otherwise.
   */
  bool emergency_switch_on() { return emergency_switch_on_; }
  // If mission.cpp (which includes this header) sees emergency_switch_on()
  // return true, it requests FlightMode::ABORT.
private:
  rclcpp::Logger logger_{rclcpp::get_logger("EmergencySwitch")};
  rclcpp::Subscription<ActuatorArmed>::SharedPtr actuator_armed_sub_;
<<<<<<< HEAD
  bool emergency_switch_on_{false};
=======
  std::atomic<bool> emergency_switch_on_{false};
>>>>>>> dbe42f2 (1. Documentation explaining Simulink Codegen and troubleshooting)
};
