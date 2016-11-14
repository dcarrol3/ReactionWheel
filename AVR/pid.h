// PID controller

#ifndef pid_h_
#define pid_h_

typedef struct
{
    float p, i, d;
    float p_prime, i_prime;

    float error_prev;
    float integral;
} pid_controller_t;

// Initalize PID controller
pid_controller_t *pid_controller_init(pid_controller_t *pid, float p, float i, float d);

// Reset the PID controller values
pid_controller_t *pid_controller_reset(pid_controller_t *pid);

// Update PID value
float pid_update(pid_controller_t *pid,
        float error, float diff,
        float dt, float min, float max,
        float int_min, float int_max,
        float feed_forward);


#endif
