//
// Created by Martijn on 12-1-2017.
//
#include <stm32f0_discovery.h>
#include "flash.h"

uint32_t EraseCounter = 0x00, Address = 0x00;
uint32_t Data = 0x3210ABCD;
uint32_t NbrOfPage = 0x00;
__IO FLASH_Status FLASHStatus = FLASH_COMPLETE;
__IO TestStatus MemoryProgramStatus = PASSED;


//function that writes the heights to flash
uint8_t writeFlash(uint16_t HeightOne, uint16_t HeightTwo) {
    uint8_t SuccessCode = 0;
    uint32_t DATA_32;

    DATA_32 = HeightOne;
    DATA_32 = DATA_32 << 16;
    DATA_32 = DATA_32 | HeightTwo; //combine both height to get a 32 bit word, more efficient.


    /* Unlock the Flash to enable the flash control register access *************/
    FLASH_Unlock();

    /* Erase the user Flash area
      (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

    /* Clear pending flags (if any) */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);

    /* Define the number of page to be erased */
    NbrOfPage = (FLASH_USER_END_ADDR - FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE;

    /* Erase the FLASH pages */
    for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++) {
        if (FLASH_ErasePage(FLASH_USER_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter)) != FLASH_COMPLETE) {
            SuccessCode = 1; /* Error occurred while sector erase. */
        }
    }
    /* Program the user Flash area word by word
      (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

    Address = FLASH_USER_START_ADDR;

    while (Address < FLASH_USER_END_ADDR) {
        if (FLASH_ProgramWord(Address, DATA_32) == FLASH_COMPLETE) {
            Address = Address + 4;
        } else {
            SuccessCode = 2; /* Error occurred while writing data in Flash memory. */
        }
    }

    /* Lock the Flash to disable the flash control register access (recommended
       to protect the FLASH memory against possible unwanted operation) *********/
    FLASH_Lock();

    /* Check if the programmed data is OK
    MemoryProgramStatus = 0: data programmed correctly
    MemoryProgramStatus != 0: number of words not programmed correctly ******/
    Address = FLASH_USER_START_ADDR;
    MemoryProgramStatus = PASSED;

    while (Address < FLASH_USER_END_ADDR) {
        Data = *(__IO uint32_t *) Address;

        if (Data != DATA_32) {
            SuccessCode = 3;
        }

        Address = Address + 4;
    }

    return SuccessCode; //value 0: all well. value 1: problem during clearing flash.
}                                            //value 2: problem during writing. value 3: flash wasnt written with the right value

//function to read from flash
int readFlash(uint8_t H1orH2) {
    uint16_t Height;
    uint32_t Value;

    Address = FLASH_USER_START_ADDR;
    Value = *(__IO uint32_t *) Address;

    if (H1orH2 == 1) //return Height One
    {
        Value = Value >> 16;
        Height = Value;
    } else //return Height Two
    {
        Value = Value & 0x0000FFFF;
        Height = Value;
    }

    return Height;
}