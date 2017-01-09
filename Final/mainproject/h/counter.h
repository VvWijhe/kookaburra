//
// Created by Victor on 17-10-2016.
//

#ifndef KOOKABURRA_COUNTER_H
#define KOOKABURRA_COUNTER_H

#include <stm32f0xx_tim.h>
#include <stdint.h>

void setTim3(uint16_t p);

void setTim14(uint16_t p);

void setTim16(uint16_t p);

void setTim17(int p);

namespace Time {
    extern int seconds;
}


#endif //KOOKABURRA_COUNTER_H
