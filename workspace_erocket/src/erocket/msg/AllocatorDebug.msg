# AllocatorDebug.msg
# Publishes internal allocator calculations to PlotJuggler for debugging.

builtin_interfaces/Time stamp

float64[3] thrust_vector  # Mathematical target thrust vector (N)
float64 tau_delta_bar     # Mathematical required differential torque

float64 m_bar             # Computed base motor PWM
float64 m_delta           # Computed differential motor PWM
float64 thrust            # Total thrust magnitude
float64 gamma_inner_degrees # Calculated inner servo tilt angle
float64 gamma_outer_degrees # Calculated outer servo tilt angle

float64 motor_upwards_pwm   # Final limited PWM output for the top motor
float64 motor_downwards_pwm # Final limited PWM output for the bottom motor
float64 outer_servo_pwm     # Final limited PWM output for the outer servo
float64 inner_servo_pwm     # Final limited PWM output for the inner servo
