/*
 * Copyright (c) 2015, 2016 Yuchong Li <93@liyc.me>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR AND CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * 
 * Accessed at https://bitbucket.org/liyuchong/akarinservo/raw/9de36c1f8f53305062f0a7a71b6c669277829a57/src/motor_control.c
 * 
 * Edited and tuned for personal use by Douglas Carroll.
 * dcarrol3@asu.edu
 * 
 * PID controller for reaction wheel.
 */


#include "pid.h"

pid_controller_t *pid_controller_init(pid_controller_t *pid, float p, float i, float d){
    pid->integral = 0.0f;

    pid->p = p;
    pid->i = i;
    pid->d = d;
    pid->error_prev = 0.0f;
    pid->p_prime = 0.0f;
    pid->i_prime = 0.0f;

    return pid;
}

pid_controller_t *pid_controller_reset(pid_controller_t *pid){
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
        float feed_forward){
			
	// Adjust integral value - keep within bounds
    pid->integral += error * dt;
    pid->integral = pid->integral > int_max ? int_max : pid->integral;
    pid->integral = pid->integral < int_min ? int_min : pid->integral;

    // Calculate P term using the given P value and its offset term
    float p = pid->p + pid->p_prime;
    p = p < 0.0 ? 0.0 : p;
	// Calculate I term using the given I value and its offset term
    float i = pid->i + pid->i_prime;
    i = i < 0.0 ? 0.0 : i;
	// (KP * error) + (KI * integral) + (KD * derivative)
    float result = error * p + pid->integral * i + feed_forward;
	// Keep within bounds
    result = result > max ? max : result;
    result = result < min ? min : result;

    return result;
}

float pid_update(pid_controller_t *pid,
		float error,
		float diff,
        float dt, float min, float max,
        float int_min, float int_max,
        float feed_forward){
			
    // Calculate PID update value
	float pi_value = pi_update(
            pid, error, dt,
            min, max,
            int_min, int_max,
            feed_forward - pid->d * diff
    );

	// Keep output within bounds
	if(pi_value > max) pi_value = max;
	if(pi_value < min) pi_value = min;

    return pi_value;
}
