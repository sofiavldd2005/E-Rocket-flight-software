// constants.hpp
#pragma once

/**
 * @namespace erocket
 * @brief Main namespace for the E-Rocket flight software.
 */
namespace erocket
{
/**
 * @namespace constants
 * @brief Global parameter strings and configuration constants for the E-Rocket system.
 */
namespace constants
{

    constexpr char MASS_OF_SYSTEM[]             = "offboard.mass_of_system";             ///< Parameter name for the total mass of the system
    constexpr char LEVER_ARM[]                  = "offboard.lever_arm";                  ///< Parameter name for the thrust lever arm length
    constexpr char GRAVITATIONAL_ACCELERATION[] = "offboard.gravitational_acceleration"; ///< Parameter name for gravitational acceleration
    constexpr char MOMENT_OF_INERTIA[]          = "offboard.moment_of_inertia";          ///< Parameter name for the vehicle's moment of inertia

/**
 * @namespace vehicle
 * @brief Parameters related to physical vehicle actuators and curves.
 */
namespace vehicle
{
    constexpr char SERVO_ACTIVE_PARAM[] = "offboard.vehicle.servo_active";                         ///< Parameter to enable/disable servos
    constexpr char MOTOR_ACTIVE_PARAM[] = "offboard.vehicle.motor_active";                         ///< Parameter to enable/disable motor

    constexpr char SERVO_MAX_TILT_ANGLE_DEGREES_PARAM[] = "offboard.vehicle.servo_max_tilt_angle_degrees"; ///< Max servo tilt angle (degrees)
    constexpr char CONTROLLER_DEFAULT_MOTOR_PWM[] = "offboard.vehicle.default_motor_pwm";          ///< Default motor PWM value
    constexpr char MAX_MOTOR_PWM_PARAM[] = "offboard.vehicle.motor_max_pwm";                       ///< Maximum allowed motor PWM
    constexpr char MOTOR_THRUST_CURVE_M_PARAM[] = "offboard.vehicle.motor_thrust_curve.m";         ///< Motor thrust curve slope (m)
    constexpr char MOTOR_THRUST_CURVE_B_PARAM[] = "offboard.vehicle.motor_thrust_curve.b";         ///< Motor thrust curve intercept (b)

    constexpr char DELTA_TORQUE_A_PARAM[] = "offboard.vehicle.delta_torque.a";                     ///< Torque curve parameter A
    constexpr char DELTA_TORQUE_B_PARAM[] = "offboard.vehicle.delta_torque.b";                     ///< Torque curve parameter B
    constexpr char DELTA_TORQUE_C_PARAM[] = "offboard.vehicle.delta_torque.c";                     ///< Torque curve parameter C

} // namespace vehicle

/**
 * @namespace controller
 * @brief Topic names and parameters for the baseline PID controller.
 */
namespace controller
{

    //<! Topics for the 1-degree-of-freedom system
    constexpr char CONTROLLER_INPUT_ATTITUDE_TOPIC[] =      "/fmu/out/vehicle_attitude";         ///< PX4 output: vehicle attitude
    constexpr char CONTROLLER_INPUT_ANGULAR_RATE_TOPIC[] =  "/fmu/out/vehicle_angular_velocity"; ///< PX4 output: angular velocity
    constexpr char CONTROLLER_INPUT_LOCAL_POSITION_TOPIC[] =  "/fmu/out/vehicle_local_position"; ///< PX4 output: local position
    constexpr char CONTROLLER_INPUT_ODOMETRY_TOPIC[] =      "/fmu/out/vehicle_odometry";         ///< PX4 output: odometry
    constexpr char CONTROLLER_OUTPUT_MOTOR_PWM_TOPIC[] =    "/fmu/in/actuator_motors";           ///< PX4 input: motor commands
    constexpr char CONTROLLER_OUTPUT_SERVO_PWM_TOPIC[] =    "/fmu/in/actuator_servos";           ///< PX4 input: servo commands

    constexpr char CONTROLLER_ATTITUDE_DEBUG_TOPIC[] = "/offboard/attitude_controller/debug";    ///< Debug topic: attitude controller
    constexpr char CONTROLLER_POSITION_DEBUG_TOPIC[] = "/offboard/position_controller/debug";    ///< Debug topic: position controller
    constexpr char ALLOCATOR_DEBUG_TOPIC[] = "/offboard/allocator/debug";                        ///< Debug topic: allocator

    constexpr char CONTROLLER_ATTITUDE_ACTIVE_PARAM[] = "offboard.controller.attitude.active";   ///< Enable attitude control
    constexpr char CONTROLLER_ATTITUDE_K_P_PARAM[] = "offboard.controller.attitude.gains.k_p";   ///< Attitude Proportional gain
    constexpr char CONTROLLER_ATTITUDE_K_D_PARAM[] = "offboard.controller.attitude.gains.k_d";   ///< Attitude Derivative gain
    constexpr char CONTROLLER_ATTITUDE_K_I_PARAM[] = "offboard.controller.attitude.gains.k_i";   ///< Attitude Integral gain
    constexpr char CONTROLLER_ATTITUDE_FREQUENCY_HERTZ_PARAM[] = "offboard.controller.attitude.frequency_hertz"; ///< Attitude control frequency

    constexpr char CONTROLLER_POSITION_ACTIVE_PARAM[] = "offboard.controller.position.active";   ///< Enable position control
    constexpr char CONTROLLER_POSITION_K_P_PARAM[] = "offboard.controller.position.gains.k_p";   ///< Position Proportional gain
    constexpr char CONTROLLER_POSITION_K_D_PARAM[] = "offboard.controller.position.gains.k_d";   ///< Position Derivative gain
    constexpr char CONTROLLER_POSITION_K_I_PARAM[] = "offboard.controller.position.gains.k_i";   ///< Position Integral gain
    constexpr char CONTROLLER_POSITION_K_FF_PARAM[] = "offboard.controller.position.gains.k_ff"; ///< Position Feed-forward gain
    constexpr char CONTROLLER_POSITION_MIN_OUTPUT_PARAM[] = "offboard.controller.position.gains.min_output"; ///< Minimum output for position loop
    constexpr char CONTROLLER_POSITION_MAX_OUTPUT_PARAM[] = "offboard.controller.position.gains.max_output"; ///< Maximum output for position loop
    constexpr char CONTROLLER_POSITION_FREQUENCY_HERTZ_PARAM[] = "offboard.controller.position.frequency_hertz"; ///< Position control frequency
} // namespace controller

/**
 * @namespace controller_generic
 * @brief Parameters for the generic template controller.
 */
namespace controller_generic
{
    constexpr char CONTROLLER_GENERIC_FREQUENCY_HERTZ_PARAM[] = "offboard.controller.generic.frequency_hertz"; ///< Frequency of generic controller
} // namespace controller_generic

/**
 * @namespace flight_mode
 * @brief Topic names and parameters for the flight mode node.
 */
namespace flight_mode
{

    //<! Topics for the 1-degree-of-freedom system
    constexpr char FLIGHT_MODE_GET_TOPIC[] = "offboard/flight_mode/get"; ///< Topic to read current flight mode
    constexpr char FLIGHT_MODE_SET_TOPIC[] = "offboard/flight_mode/set"; ///< Topic to request flight mode change

} // namespace flight_mode

namespace flight_mode
{

    constexpr float MANTAIN_OFFBOARD_MODE_TIMER_PERIOD_SECONDS = 0.1f; // 10 Hz

    //<! Parameters for the flight mode node
    constexpr char FLIGHT_MODE_PARAM[] = "offboard.flight_mode"; ///< Parameter for flight mode

} // namespace flight_mode

/**
 * @namespace mocap_forwarder
 * @brief Topic names and parameters for the motion capture forwarder.
 */
namespace mocap_forwarder
{

    constexpr char MOCAP_TOPIC[]         = "/mocap/pose_enu/erocket"; ///< Incoming mocap pose topic
    constexpr char MOCAP_ACTIVE_PARAM[]  = "offboard.mocap.active";   ///< Enable/disable mocap forwarder

} // namespace mocap_forwarder

/**
 * @namespace setpoint
 * @brief Topic names and parameters for trajectory setpoints.
 */
namespace setpoint
{

    constexpr char CONTROLLER_INPUT_ATTITUDE_SETPOINT_TOPIC[] = "offboard/attitude_setpoint_degrees"; ///< Topic for attitude setpoints
    constexpr char MISSION_ATTITUDE_SETPOINT_PARAM[] = "offboard.mission.attitude_setpoint.degrees";    ///< Parameter for static attitude setpoint

    constexpr char CONTROLLER_INPUT_TRANSLATION_POSITION_SETPOINT_TOPIC[] = "offboard/translation_position_setpoint_meters"; ///< Topic for position setpoints
    constexpr char MISSION_TRANSLATION_POSITION_SETPOINT_PARAM[] = "offboard.mission.translation_position_setpoint.meters";    ///< Parameter for static position setpoint

    constexpr char CONTROLLER_INPUT_SETPOINT_C5_TOPIC[] = "offboard/setpoint_c5_meters"; ///< Topic for C5 continuous setpoints
    constexpr char MISSION_TRAJECTORY_SETPOINT_ACTIVE_PARAM[] = "offboard.mission.trajectory_setpoint.active"; ///< Parameter to enable C5 trajectory

} // namespace setpoint

/**
 * @namespace takeoff_landing
 * @brief Parameters governing takeoff and landing sequences.
 */
namespace takeoff_landing
{

    constexpr char MISSION_TAKEOFF_CLIMB_HEIGHT_PARAM[] = "offboard.mission.takeoff_climb_height_meters";         ///< Target altitude for takeoff
    constexpr char MISSION_TAKEOFF_CLIMB_DURATION_PARAM[] = "offboard.mission.takeoff_climb_duration_seconds";      ///< Duration to complete takeoff
    constexpr char MISSION_LANDING_DESCENT_DURATION_PARAM[] = "offboard.mission.landing_descent_duration_seconds";  ///< Duration to complete landing

} // namespace takeoff_landing

/**
 * @namespace emergency
 * @brief Constants related to emergency abort states.
 */
namespace emergency
{

    constexpr char EMERGENCY_ACTUATOR_ARMED[] = "fmu/out/actuator_armed"; ///< Topic indicating physical actuator armed state

} // namespace emergency
} // namespace constants
} // namespace erocket
