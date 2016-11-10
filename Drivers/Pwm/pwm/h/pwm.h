//
// Created by Jamie on 10-11-2016.
//

#ifndef PWM_PWM_H
#define PWM_PWM_H

class PWM {
private:

public:
    void cycle(uint8_t timer, uint16_t duty_cicle);
    void initServo(void);
    void initMotor(void);
};


#endif //PWM_PWM_H
