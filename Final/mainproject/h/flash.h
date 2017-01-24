//
// Created by Martijn on 12-1-2017.
//

#ifndef MAINPROJECT_FLASH_H
#define MAINPROJECT_FLASH_H

/* Private define ------------------------------------------------------------*/
#ifdef STM32F072
#define FLASH_PAGE_SIZE         ((uint32_t)0x00000800)   /* FLASH Page Size */
#define FLASH_USER_START_ADDR   ((uint32_t)0x08009000)   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ((uint32_t)0x08020000)   /* End @ of user Flash area */
#elif defined (STM32F091)
#define FLASH_PAGE_SIZE         ((uint32_t)0x00000800)   /* FLASH Page Size */
#define FLASH_USER_START_ADDR   ((uint32_t)0x08009000)   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ((uint32_t)0x08040000)   /* End @ of user Flash area */
#else
#define FLASH_PAGE_SIZE         ((uint32_t)0x00000400)   /* FLASH Page Size */
#define FLASH_USER_START_ADDR   ((uint32_t)0x08006000)   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ((uint32_t)0x08007000)   /* End @ of user Flash area */
#endif /* STM32F072 */

/* Private typedef -----------------------------------------------------------*/
__IO typedef enum {
    FAILED = false, PASSED = true
} TestStatus;

uint8_t writeFlash(uint16_t HeightOne, uint16_t HeightTwo);

int readFlash(uint8_t H1orH2);

#endif //MAINPROJECT_FLASH_H
