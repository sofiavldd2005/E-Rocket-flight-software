#pragma once
#include <rclcpp/rclcpp.hpp>
#include <Eigen/Dense>
#include <erocket/msg/setpoint_c5.hpp>
#include <geometry_msgs/msg/vector3_stamped.hpp>
#include <erocket/constants.hpp>

using namespace geometry_msgs::msg;
using namespace erocket::msg;
using namespace erocket::constants::setpoint;

/**
 * @struct PositionSetpoint
 * @brief Represents a mathematical trajectory setpoint for position control.
 */
struct PositionSetpoint {
    // derived states
    Eigen::Vector3d position;
    Eigen::Vector3d velocity;
    Eigen::Vector3d acceleration;
    Eigen::Vector3d jerk;
    Eigen::Vector3d snap;
    double yaw;
    double yaw_rate;
    double yaw_acceleration;
};

/**
 * @struct AttitudeSetpoint
 * @brief Represents a mathematical setpoint for attitude control.
 */
struct AttitudeSetpoint {
    // derived states
    Eigen::Vector3d attitude; // roll, pitch, yaw in radians
};

/**
 * @class SetpointAggregator
 * @brief Aggregates trajectory and static setpoint messages into unified structs for the controllers.
 */
class SetpointAggregator
{
public:
    /**
     * @brief Construct a new Setpoint Aggregator
     * 
     * @param node The parent ROS 2 node.
     * @param qos The QoS profile for the setpoint subscriptions.
     */
    SetpointAggregator(rclcpp::Node* node, rclcpp::QoS qos) :
    position_setpoint_sub_{node->create_subscription<SetpointC5>(
        CONTROLLER_INPUT_SETPOINT_C5_TOPIC, qos,
        [this](const SetpointC5::SharedPtr msg) {
            this->pos_setpoint_.position = Eigen::Map<Eigen::Vector3d>(msg->position.data());
            this->pos_setpoint_.velocity = Eigen::Map<Eigen::Vector3d>(msg->velocity.data());
            this->pos_setpoint_.acceleration = Eigen::Map<Eigen::Vector3d>(msg->acceleration.data());
            this->pos_setpoint_.jerk = Eigen::Map<Eigen::Vector3d>(msg->jerk.data());
            this->pos_setpoint_.snap = Eigen::Map<Eigen::Vector3d>(msg->snap.data());
            this->pos_setpoint_.yaw_rate = msg->yaw_rate;
            this->pos_setpoint_.yaw_acceleration = msg->yaw_acceleration;

            // if yaw setpoint is a number, save it
            if (!std::isnan(msg->yaw)) {
                this->pos_setpoint_.yaw = msg->yaw;
                this->att_setpoint_.attitude[2] = msg->yaw;
            }
        }
    )},
    attitude_setpoint_sub_{node->create_subscription<Vector3Stamped>(
        CONTROLLER_INPUT_ATTITUDE_SETPOINT_TOPIC, qos,
        [this](const Vector3Stamped::SharedPtr msg) {
            if (std::isnan(msg->vector.x) || std::isnan(msg->vector.y) || std::isnan(msg->vector.z)) {
                RCLCPP_ERROR(logger_, "Received NaN in setpoint message.");
                return;
            }
            if (msg->vector.x < -M_PI_2 || msg->vector.y < -M_PI_2 || msg->vector.z < -M_PI ||
                msg->vector.x > M_PI_2  || msg->vector.y > M_PI_2  || msg->vector.z > M_PI
            ) {
                RCLCPP_ERROR(logger_, "Received out of range setpoint message.");
                return;
            }

            this->att_setpoint_.attitude[0] = msg->vector.x;
            this->att_setpoint_.attitude[1] = msg->vector.y;
            this->att_setpoint_.attitude[2] = msg->vector.z;
        }
    )},
    translation_position_setpoint_sub_{node->create_subscription<Vector3Stamped>(
        CONTROLLER_INPUT_TRANSLATION_POSITION_SETPOINT_TOPIC, qos,
        [this](const Vector3Stamped::SharedPtr msg) {
            if (std::isnan(msg->vector.x) || std::isnan(msg->vector.y) || std::isnan(msg->vector.z)) {
                RCLCPP_ERROR(logger_, "Received NaN in setpoint message.");
                return;
            }

            this->pos_setpoint_.position += Eigen::Vector3d(msg->vector.x, msg->vector.y, msg->vector.z);
            this->pos_setpoint_.velocity.setZero();
            this->pos_setpoint_.acceleration.setZero();
            this->pos_setpoint_.jerk.setZero();
            this->pos_setpoint_.snap.setZero();
            this->pos_setpoint_.yaw = std::nan("");
            this->pos_setpoint_.yaw_rate = 0.0;
            this->pos_setpoint_.yaw_acceleration = 0.0;
        }
    )}
    {}

    /**
     * @brief Get the current position setpoint
     * @return The active PositionSetpoint.
     */
    PositionSetpoint get_position_setpoint() const { return pos_setpoint_; }

    /**
     * @brief Get the current attitude setpoint
     * @return The active AttitudeSetpoint.
     */
    AttitudeSetpoint get_attitude_setpoint() const { return att_setpoint_; }

    /**
     * @brief Manually override the current attitude setpoint
     * @param att_setpoint The new AttitudeSetpoint to apply.
     */
    void set_attitude_setpoint(const AttitudeSetpoint& att_setpoint) { att_setpoint_ = att_setpoint; }

private:
    PositionSetpoint pos_setpoint_;
    AttitudeSetpoint att_setpoint_;

    rclcpp::Subscription<SetpointC5>::SharedPtr position_setpoint_sub_;
    rclcpp::Subscription<Vector3Stamped>::SharedPtr attitude_setpoint_sub_;
    rclcpp::Subscription<Vector3Stamped>::SharedPtr translation_position_setpoint_sub_;

    rclcpp::Logger logger_ = rclcpp::get_logger("SetpointAggregator");
};
