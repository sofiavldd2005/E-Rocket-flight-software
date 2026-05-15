#pragma once
#include <erocket/constants.hpp>
#include <erocket/frame_transforms.h>
#include <erocket/msg/allocator_debug.hpp>
#include <erocket/vehicle_constants.hpp>
#include <px4_msgs/msg/actuator_motors.hpp>
#include <px4_msgs/msg/actuator_servos.hpp>
#include <rclcpp/rclcpp.hpp>

using namespace std::chrono;
using namespace std::chrono_literals;
using namespace px4_msgs::msg;
using namespace erocket::msg;
using namespace erocket::constants::controller;

namespace frame_transforms = erocket::frame_transforms;

/**
 * @struct AllocatorInput
 * @brief Input variables containing the mathematical commands from the controllers.
 */
struct AllocatorInput {

  // TODO: see this Eigen stuff
  Eigen::Vector3d thrust_vector;
  double tau_delta_bar;
};

/**
 * @struct AllocatorOutput
 * @brief Output PWM values to be sent directly to the PX4 actuators.
 */
struct AllocatorOutput {
  double inner_servo_pwm;
  double outer_servo_pwm;
  double upwards_motor_pwm;
  double downwards_motor_pwm;
};

/**
 * @class Allocator
 * @brief Maps mathematical control vectors (thrust, torque) to physical actuator PWM signals.
 */
class Allocator {
public:
  /**
   * @brief Construct a new Allocator object
   * 
   * @param node The parent ROS 2 Node.
   * @param qos The QoS profile used for publishers.
   * @param vehicle_constants Shared pointer to the vehicle's physical constants.
   */
  Allocator(rclcpp::Node *node, rclcpp::QoS qos,
            std::shared_ptr<VehicleConstants> vehicle_constants)
      : vehicle_constants_(vehicle_constants),
        servo_tilt_angle_publisher_{node->create_publisher<ActuatorServos>(
            CONTROLLER_OUTPUT_SERVO_PWM_TOPIC, qos)},
        motor_thrust_publisher_{node->create_publisher<ActuatorMotors>(
            CONTROLLER_OUTPUT_MOTOR_PWM_TOPIC, qos)},
        debug_publisher_{
            node->create_publisher<AllocatorDebug>(ALLOCATOR_DEBUG_TOPIC, qos)},
        clock_(std::make_shared<rclcpp::Clock>(RCL_ROS_TIME)) {
    output_ = AllocatorOutput{};

    auto t0 = node->get_clock()->now();
    while (node->get_clock()->now() - t0 < 1s) {
      publish_servo_pwm();
      publish_motor_pwm();
      rclcpp::sleep_for(100ms);
    }
  }

  /**
   * @brief Computes actuator allocation given the requested mathematical thrust and torque.
   * 
   * @param input The requested AllocatorInput containing thrust vector and torque commands.
   */
  void compute_allocation(AllocatorInput input) {
    input_ = input;
    // thrust vector magnitude computation
    thrust_ = input.thrust_vector.norm();
    // convert that physical force (Newtons) into a base PWM signal (M_bar_)
    // using the physical characterization curve of the specific motors
    // (thrust_curve_m and thrust_curve_b)
    M_bar_ = motor_thrust_curve_newtons_to_pwm(thrust_);
    M_delta_ = delta_torque_curve(M_bar_, input.tau_delta_bar);

    // induce roll torque (tau_delta_bar), he calculates a differential PWM
    // (M_delta_) using an experimentally derived curve (delta_torque_curve()).
    // One motor speeds up (upwards_motor_pwm = M_bar_ + M_delta_ / 2.0f), and
    // the other slows down
    auto upwards_motor_pwm = M_bar_ + M_delta_ / 2.0f;
    auto downwards_motor_pwm = M_bar_ - M_delta_ / 2.0f;

    // point the thrust vector, he calculates the required angles using arcsine
    // (std::asin). He calculates gamma_inner_ using the Y-axis thrust and
    // gamma_outer_ using the X-axis thrust, accounting for the inner tilt
    gamma_inner_ = std::asin(input.thrust_vector[1] / thrust_);
    gamma_outer_ =
        std::asin(-input.thrust_vector[0] /
                  (sqrt(thrust_ * thrust_ -
                        input.thrust_vector[1] * input.thrust_vector[1])));
    auto inner_servo_pwm = servo_curve_tilt_radians_to_pwm(gamma_inner_);
    auto outer_servo_pwm = servo_curve_tilt_radians_to_pwm(gamma_outer_);

    output_.inner_servo_pwm = limit_range_servo_pwm(inner_servo_pwm);
    output_.outer_servo_pwm = limit_range_servo_pwm(outer_servo_pwm);
    output_.upwards_motor_pwm = limit_range_motor_pwm(upwards_motor_pwm);
    output_.downwards_motor_pwm = limit_range_motor_pwm(downwards_motor_pwm);

    publish_servo_pwm();
    publish_motor_pwm();
    publish_allocator_debug();
  }

  /**
   * @brief Sets actuators to a neutral, safe state (zero thrust/angles).
   */
  void compute_allocation_neutral() {
    output_.inner_servo_pwm = 0.0f;
    output_.outer_servo_pwm = 0.0f;
    output_.upwards_motor_pwm = NAN;
    output_.downwards_motor_pwm = NAN;

    publish_servo_pwm();
    publish_motor_pwm();
    publish_allocator_debug();
  }

  /**
   * @brief Allows for direct manual control of the actuators, bypassing the PID loops.
   * 
   * @param inner_servo_tilt_angle_radians Inner servo tilt angle (radians).
   * @param outer_servo_tilt_angle_radians Outer servo tilt angle (radians).
   * @param upwards_motor_thrust_pwm Upwards motor thrust PWM signal.
   * @param downwards_motor_thrust_pwm Downwards motor thrust PWM signal.
   */
  void indirect_actuation(double inner_servo_tilt_angle_radians = 0.0f,
                          double outer_servo_tilt_angle_radians = 0.0f,
                          double upwards_motor_thrust_pwm = NAN,
                          double downwards_motor_thrust_pwm = NAN) {
    output_.inner_servo_pwm = limit_range_servo_pwm(
        servo_curve_tilt_radians_to_pwm(inner_servo_tilt_angle_radians));
    output_.outer_servo_pwm = limit_range_servo_pwm(
        servo_curve_tilt_radians_to_pwm(outer_servo_tilt_angle_radians));
    output_.upwards_motor_pwm = limit_range_motor_pwm(upwards_motor_thrust_pwm);
    output_.downwards_motor_pwm =
        limit_range_motor_pwm(downwards_motor_thrust_pwm);

    publish_servo_pwm();
    publish_motor_pwm();
    publish_allocator_debug();
  }

  double motor_thrust_curve_pwm_to_newtons(double motor_pwm) {
    auto g = vehicle_constants_->gravitational_acceleration_;
    auto thrust_curve_m = vehicle_constants_->motor_thrust_curve_m_;
    auto thrust_curve_b = vehicle_constants_->motor_thrust_curve_b_;

    return (motor_pwm * thrust_curve_m + thrust_curve_b) / 1000.0f * g;
  }

  double motor_thrust_curve_newtons_to_pwm(double thrust_newtons) {
    auto g = vehicle_constants_->gravitational_acceleration_;
    auto thrust_curve_m = vehicle_constants_->motor_thrust_curve_m_;
    auto thrust_curve_b = vehicle_constants_->motor_thrust_curve_b_;

    return ((thrust_newtons * 1000.0f) / g - thrust_curve_b) / thrust_curve_m;
  }

private:
  std::shared_ptr<VehicleConstants> vehicle_constants_;

  AllocatorInput input_;
  AllocatorOutput output_;

  double thrust_;
  double M_bar_;
  double M_delta_;
  double gamma_inner_ = 0.0f;
  double gamma_outer_ = 0.0f;

  rclcpp::Publisher<ActuatorServos>::SharedPtr servo_tilt_angle_publisher_;
  rclcpp::Publisher<ActuatorMotors>::SharedPtr motor_thrust_publisher_;
  rclcpp::Publisher<AllocatorDebug>::SharedPtr debug_publisher_;

  rclcpp::Clock::SharedPtr clock_;
  rclcpp::Logger logger_ = rclcpp::get_logger("allocator");

  // clamp the servo
  double limit_range_servo_pwm(double servo_pwm) {
    servo_pwm = (servo_pwm > 1.0f) ? 1.0f : servo_pwm;
    servo_pwm = (servo_pwm < -1.0f) ? -1.0f : servo_pwm;

    return servo_pwm;
  }
  // clamps the motor signal, prevents the motor to rotate backwards
  double limit_range_motor_pwm(double motor_pwm) {
    motor_pwm = (motor_pwm > vehicle_constants_->max_motor_pwm_)
                    ? vehicle_constants_->max_motor_pwm_
                    : motor_pwm;
    motor_pwm = (motor_pwm < 0.0f) ? 0.0f : motor_pwm;

    return motor_pwm;
  }

  double servo_curve_tilt_radians_to_pwm(double servo_tilt_angle_radians) {
    double servo_tilt_angle_degrees =
        frame_transforms::radians_to_degrees(servo_tilt_angle_radians);
    return servo_tilt_angle_degrees /
           vehicle_constants_->servo_max_tilt_angle_degrees_;
  }

  double delta_torque_curve(double M_bar, double tau_delta_bar) {
    auto a = vehicle_constants_->delta_torque_a_;
    auto b = vehicle_constants_->delta_torque_b_;
    auto c = vehicle_constants_->delta_torque_c_;
    return tau_delta_bar / a - (b / a) * M_bar - c / a;
  }

  void publish_servo_pwm() {
    ActuatorServos msg{};
    msg.timestamp = clock_->now().nanoseconds() / 1000;
    if (vehicle_constants_->servo_active_) {
      msg.control[0] = output_.outer_servo_pwm;
      msg.control[1] = output_.inner_servo_pwm;
    } else {
      // If servos are not active, disable them
      msg.control[0] = 0.;
      msg.control[1] = 0.;
    }
    servo_tilt_angle_publisher_->publish(msg);
  }

  /**
   * @brief Publish the computed actuator motor PWM values.
   */
  void publish_motor_pwm() {
    ActuatorMotors msg{};
    msg.timestamp = clock_->now().nanoseconds() / 1000;
    if (vehicle_constants_->motor_active_) {
      msg.control[0] = output_.upwards_motor_pwm;
      msg.control[1] = output_.downwards_motor_pwm;
    } else {
      // If motors are not active, disable them
      // Zeros out the servos and sets the motors to NAN In PX4 logic, sending
      // NAN to a motor immediately disarms it
      msg.control[0] = NAN;
      msg.control[1] = NAN;
    }
    motor_thrust_publisher_->publish(msg);
  }

  // publishes the internal variables (m_bar, m_delta, gamma_inner_degrees)
  // over the ROS 2 network using the custom AllocatorDebug messag
  // open PlotJuggler and watch the calculated inner and
  // outer servo angles in real-time, perfectly synced with the actual physical
  // output PWMs
  void publish_allocator_debug() {
    AllocatorDebug msg{};
    msg.stamp = clock_->now();

    Eigen::Map<Eigen::Vector3d>(msg.thrust_vector.data()) =
        input_.thrust_vector;
    msg.tau_delta_bar = input_.tau_delta_bar;
    msg.m_bar = M_bar_;
    msg.m_delta = M_delta_;
    msg.thrust = thrust_;
    msg.gamma_inner_degrees =
        frame_transforms::radians_to_degrees(gamma_inner_);
    msg.gamma_outer_degrees =
        frame_transforms::radians_to_degrees(gamma_outer_);

    msg.motor_upwards_pwm = output_.upwards_motor_pwm;
    msg.motor_downwards_pwm = output_.downwards_motor_pwm;
    msg.outer_servo_pwm = output_.inner_servo_pwm;
    msg.inner_servo_pwm = output_.outer_servo_pwm;

    debug_publisher_->publish(msg);
  }
};
