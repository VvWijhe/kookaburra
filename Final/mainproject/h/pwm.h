//
// Created by Jamie on 10-11-2016.
//

#ifndef PWM_PWM_H
#define PWM_PWM_H

class AirplaneControl {
public:
    typedef enum {
        PWM_MOTOR,
        PWM_SERVO_ELEVATOR,
        PWM_SERVO_RUDDER
    } pwmFunction_t;

    AirplaneControl();
    void setOnTime(pwmFunction_t function, uint32_t time);
    void initServo();
    void initMotor();

private:
    bool motorEnabled, servoEnbled;
};


#endif //PWM_PWM_H
