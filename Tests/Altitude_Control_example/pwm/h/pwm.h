//
// Created by Jamie on 10-11-2016.
//
#include <stm32f0xx_conf.h>
#include <stm32f0_discovery.h>

#ifndef PWM_PWM_H
#define PWM_PWM_H

class PWM {
private:

public:
    void cycle(uint8_t timer, uint32_t duty_cicle);
    void initServo(void);
    void initMotor(void);
};


#endif //PWM_PWM_H
