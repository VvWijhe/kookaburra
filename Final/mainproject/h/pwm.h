//
// Created by Jamie on 10-11-2016.
//

#ifndef PWM_PWM_H
#define PWM_PWM_H

extern __IO int32_t PrevDutyCycle; //logging duty cycle in case of bizarre values
extern float currentDutyCycle; // the duty cycle in %
extern uint8_t Attempts; // to make the autopilot switch time-based
extern uint8_t StepCount; // to log every step of said switch

typedef enum {
    PWM_MOTOR,
    PWM_SERVO_ELEVATOR,
    PWM_SERVO_RUDDER
} pwmFunction_t;

class AirplaneControl {
public:

    AirplaneControl();

    void setOnTime(pwmFunction_t function, uint32_t time);

    void InitCapComp();

    void init();

private:
    bool isEnabled;
};


#endif //PWM_PWM_H
