//
// Created by Victor on 17-10-2016.
//

#ifndef KOOKABURRA_COUNTER_H
#define KOOKABURRA_COUNTER_H

#include <stm32f0xx_tim.h>
#include <stdint.h>


class Time {
public:
    Time(int p);
    void Raisetime();

private:
    int period;
    int32_t Value;
};


#endif //KOOKABURRA_COUNTER_H
