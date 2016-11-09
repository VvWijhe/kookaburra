//
// Created by Victor on 9-11-2016.
//

#ifndef PWM_PWM_H
#define PWM_PWM_H

#endif //PWM_PWM_H


#include "usart.h"
#include "../../../../basisopbouw/mainproject/h/includes.h"

class pwm {
private:

public:
    void pwm_cicle  (uint8_t timer, uint16_t duty_cicle);
    void init_servo (void);
    void init_motor (void);
};