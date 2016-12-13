//
// Created by Jamie on 10-11-2016.
//

#ifndef PWM_PWM_H
#define PWM_PWM_H

typedef enum {
    PWM_MOTOR,
    PWM_SERVO_ELEVATOR,
    PWM_SERVO_RUDDER
} pwmFunction_t;

class AirplaneControl {
public:

    AirplaneControl();

    void setOnTime(pwmFunction_t function, uint32_t time);

    void init();

private:
    bool isEnabled;
};


#endif //PWM_PWM_H
