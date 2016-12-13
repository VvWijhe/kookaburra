//
// Created by Jamie on 12-12-2016.
//

#include <stm32f0xx_conf.h>
#include "flash.h"

Flash::Flash() {

}

void Flash::init() {
    // Set latency
    FLASH_SetLatency(FLASH_Latency_0);

    // Enable prefetch buffer
    FLASH_PrefetchBufferCmd(ENABLE);
}

void Flash::write32(uint32_t adress, uint32_t data) {
    FLASH_Unlock();

    //FLASH_ErasePage(adress);

    FLASH_ProgramWord(adress, data);

    FLASH_Lock();
}

void Flash::write16(uint32_t adress, uint16_t data) {
    FLASH_Unlock();

    FLASH_ErasePage(adress);

    FLASH_ProgramHalfWord(adress, data);

    FLASH_Lock();
}

uint32_t Flash::read32(uint32_t adress) {
    return *(uint32_t *) adress;
}

uint16_t Flash::read16(uint32_t adress) {
    return *(uint16_t *) adress;
}


