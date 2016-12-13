//
// Created by Jamie on 12-12-2016.
//

#ifndef MAINPROJECT_FLASH_H
#define MAINPROJECT_FLASH_H

#define EEPROM_START_ADDRESS  (uint32_t)(0x08010000)

#ifdef __cplusplus
extern "C" {
#endif

class Flash {
public:
    Flash();

    void init();

    void write32(uint32_t adress, uint32_t data);

    void write16(uint32_t adress, uint16_t data);

    uint32_t read32(uint32_t adress);

    uint16_t read16(uint32_t adress);

};

#ifdef __cplusplus
}
#endif

#endif //MAINPROJECT_FLASH_H
