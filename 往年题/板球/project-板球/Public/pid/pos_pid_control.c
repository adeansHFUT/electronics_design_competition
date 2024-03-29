#include "pos_pid_control.h"

#define DBG_SECTION_NAME  "pos_pid_control"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

static rt_err_t pos_pid_control_reset(void *pid)
{
    rt_memset(pid, 0, sizeof(struct pos_pid_control));
    return RT_EOK;
}

static rt_err_t pos_pid_control_destroy(void *pid)
{
    rt_free(pid);
    return RT_EOK;
}

static rt_err_t pos_pid_control_update(void *pid, float current_point)
{
    pos_pid_control_t pos_pid = (pos_pid_control_t)pid;

    if((rt_tick_get() - pos_pid->last_time) < rt_tick_from_millisecond(pos_pid->control.sample_time))
    {
        LOG_D("PID waiting ... ");
        return RT_EBUSY;
    }
    pos_pid->last_time = rt_tick_get();

    pos_pid->error = pos_pid->control.target - current_point;

    pos_pid->integral += pos_pid->error;

    //Perform integral value capping to avoid internal PID state to blows up
    //when controltuators saturate:
    if(pos_pid->integral > pos_pid->anti_windup_value) {
        pos_pid->integral = pos_pid->anti_windup_value;
    } else if (pos_pid->integral < -pos_pid->anti_windup_value) {
        pos_pid->integral = -pos_pid->anti_windup_value;
    }

    pos_pid->p_error = pos_pid->kp * pos_pid->error;
    pos_pid->i_error = pos_pid->ki * pos_pid->integral;
    pos_pid->d_error = pos_pid->kd * (pos_pid->error - pos_pid->error_l);

    pos_pid->last_out = pos_pid->p_error + pos_pid->i_error + pos_pid->d_error;
	pos_pid->test_out = pos_pid->last_out;
    if (pos_pid->last_out > pos_pid->maximum)
    {
        pos_pid->last_out = pos_pid->maximum;
    }
    if (pos_pid->last_out < pos_pid->minimum)
    {
        pos_pid->last_out = pos_pid->minimum;
    }

    pos_pid->error_l = pos_pid->error;

    pos_pid->control.output = pos_pid->last_out;

    // rt_kprintf("%d - %d\n", current_point, pid->set_point);
    // LOG_D("PID current: %d : setpoint %d - P%d I%d D%d - [%d]", current_point, pid->set_point, (int)(pid->p_error + 0.5f), (int)(pid->i_error + 0.5f), (int)(pid->d_error + 0.5f), (int)(pid->out + 0.5f));
    // LOG_D("PID P Error: %d", (int)(pid->p_error + 0.5f));
    // LOG_D("PID I Error: %d", (int)(pid->i_error + 0.5f));
    // LOG_D("PID D Error: %d", (int)(pid->d_error + 0.5f));
    // LOG_D("PID Last Out: %d", (int)(pid->last_out + 0.5f));
    // LOG_D("PID Out: %d", (int)(pid->out + 0.5f));

    return RT_EOK;
}

pos_pid_control_t pos_pid_control_create(float kp, float ki, float kd)
{
    pos_pid_control_t new_pid = (pos_pid_control_t)auto_control_create(sizeof(struct pos_pid_control));
    if(new_pid == RT_NULL)
    {
        return RT_NULL;
    }

    new_pid->kp = kp;
    new_pid->ki = ki;
    new_pid->kd = kd;

    new_pid->maximum = +1000;
    new_pid->minimum = -1000;
    new_pid->anti_windup_value = 0.0f;

    new_pid->integral = 0.0f;
    new_pid->p_error = 0.0f;
    new_pid->i_error = 0.0f;
    new_pid->d_error = 0.0f;

    new_pid->error = 0.0f;
    new_pid->error_l = 0.0f;

    new_pid->last_out = 0.0f;

    new_pid->control.reset = pos_pid_control_reset;
    new_pid->control.destroy = pos_pid_control_destroy;
    new_pid->control.update = pos_pid_control_update;

    return new_pid;
}

rt_err_t pos_pid_control_set_kp(pos_pid_control_t pid, float kp)
{
    pid->kp = kp;
    return RT_EOK;
}

rt_err_t pos_pid_control_set_ki(pos_pid_control_t pid, float ki)
{
    pid->ki = ki;
    return RT_EOK;
}

rt_err_t pos_pid_control_set_kd(pos_pid_control_t pid, float kd)
{
    pid->kd = kd;
    return RT_EOK;
}