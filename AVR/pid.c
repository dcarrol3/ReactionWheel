#include "pid.h"

pid_controller_t *pid_controller_init(pid_controller_t *pid, float p, float i, float d)
{
    pid->integral = 0.0f;

    pid->p = p;
    pid->i = i;
    pid->d = d;
    pid->error_prev = 0.0f;
    pid->p_prime = 0.0f;
    pid->i_prime = 0.0f;

    return pid;
}

pid_controller_t *pid_controller_reset(pid_controller_t *pid)
{
    pid->integral = 0.0f;
    pid->error_prev = 0.0f;
    pid->p_prime = 0.0f;
    pid->i_prime = 0.0f;

    return pid;
}

float pi_update(pid_controller_t *pid,
		float error,
        float dt, float min, float max,
        float int_min, float int_max,
        float feed_forward)
{
    pid->integral += error * dt;
    pid->integral = pid->integral > int_max ? int_max : pid->integral;
    pid->integral = pid->integral < int_min ? int_min : pid->integral;

    // Calculate P term using the given P value and its compensation term
    float p = pid->p + pid->p_prime;
    p = p < 0.0 ? 0.0 : p;

    float i = pid->i + pid->i_prime;
    i = i < 0.0 ? 0.0 : i;

    float result = error * p + pid->integral * i + feed_forward;

    result = result > max ? max : result;
    result = result < min ? min : result;

    return result;
}

float pid_update(pid_controller_t *pid,
		float error,
		float diff,
        float dt, float min, float max,
        float int_min, float int_max,
        float feed_forward)
{
    // First calculate PI update value
	float pi_value = pi_update(
            pid, error, dt,
            min, max,
            int_min, int_max,
            feed_forward - pid->d * diff
    );

	// Then clamp output
	if(pi_value > max) pi_value = max;
	if(pi_value < min) pi_value = min;

    return pi_value;
}
