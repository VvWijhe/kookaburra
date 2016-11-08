//
// Created by Victor on 17-10-2016.
//

#ifndef KOOKABURRA_COUNTER_H
#define KOOKABURRA_COUNTER_H

#include <stm32f0xx_tim.h>
#include <stdint.h>

class Time {
public:
    Time();
    void init(int p);
    void Raisetime(void);
    uint32_t GetValue(void);

private:
    int Freq;
    uint32_t Value;
};


#endif //KOOKABURRA_COUNTER_H
