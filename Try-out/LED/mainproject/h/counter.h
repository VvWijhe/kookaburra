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
    void init_Tim3(double p);
    void init_Tim16(double p);
    void SetFreq16(float freq);
    void Raisetime(void);
    uint32_t GetValue(void);
    void incrementTime(void);
    int  GetSeconds(void);
    uint32_t GetMinutes(void);
    int GetHours(void);

private:
    int Freq;
    int Freq16;
    uint32_t Value;
    int TimeDelay;
    int Seconds;
    int Minutes;
    int Hours;
};


#endif //KOOKABURRA_COUNTER_H
