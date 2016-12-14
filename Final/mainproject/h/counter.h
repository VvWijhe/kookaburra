//
// Created by Victor on 17-10-2016.
//

#ifndef KOOKABURRA_COUNTER_H
#define KOOKABURRA_COUNTER_H

#include <stm32f0xx_tim.h>
#include <stdint.h>

namespace Time {
    extern int hours;
    extern int minutes;
    extern int seconds;
}

class Timer {
public:
    Timer();

    static void setTim3(uint16_t p);

    static void setTim14(uint16_t p);

    static void setTim16(uint16_t p);

    static void incrementTime(int &hours, int &minutes, int &seconds);

private:

};


#endif //KOOKABURRA_COUNTER_H
