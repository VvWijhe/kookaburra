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
    void init(int p, int f);
    void Raisetime(void);
    uint32_t GetValue(void);
    void incrementTime(void);
    int  GetSeconds(void);
    uint32_t GetMinutes(void);

private:
    int Freq;
    uint32_t Value;
    int TimeDelay;
    int Seconds;
    int Minutes;
};


#endif //KOOKABURRA_COUNTER_H
