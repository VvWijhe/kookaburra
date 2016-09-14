/**
  ******************************************************************************
  * @file    cpalDriver.c
  * @author  Ewout Boks, HAN ESE
  * @version V1.1.1
  * @date    15-12-2015
  * @brief   Aangepaste driver voor de CPAL bibliotheek
  *            
  *          @note This file is a modified version of stm320518_eval_i2c_tsensor.c driver;
  *                I2C CPAL library drivers are used instead of the Standard Peripherals
  *                I2C driver.
  *                 
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CPALDriver_H
#define __CPALDriver_H

/* Includes ------------------------------------------------------------------*/
#include <stm32f0xx_i2c_cpal.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief initialiseer de cpal bibliotheek.
 *  @note roep deze aan voor alle andere funkties. */
void cpalInit(void);


/*! @brief lees een register
 *  @param adres van de slaaf
 *  @param adres van het register
 * @return waarde van het register */
uint8_t cpalLeesReg(const uint8_t,
                    const uint8_t);

/*! @brief lees twee registers alsl een unsigned halfword
 *  @param adres van de slaaf
 *  @param adres van het register
 * @return waarde van het register */
uint16_t cpalLees2Regs(const uint8_t,
                       const uint8_t);


/*! @brief lees registers
 *  @param adres van de slaaf
 *  @param adres van het eerste register
 *  @param ptr naar leeslokatie (bestemming van de data)
 *  @param Het aantal bytes */
void cpalLeesRegs(const uint8_t,
                  const uint8_t,
                  uint8_t *const,
                  const uint32_t);


/*! @brief schrijf een register
 *  @param adres van de slaaf
 *  @param adres van het register 
 * @param waarde die geschreven wordt */
void cpalSchrijfReg(const uint8_t,
                    const uint8_t,
                    const uint8_t);

/*! @brief schrijf twee registers met een unsigned halfword
 *  @param adres van de slaaf
 *  @param adres van het eerste register 
 * @param waarde die geschreven wordt */
void cpalSchrijf2Regs(const uint8_t,
                      const uint8_t,
                      const uint16_t);

/*! @brief schrijf registers
 *  @param adres van de slaaf
 *  @param adres van het eerste register
 *  @param ptr naar schrijflokatie (bron van de data)
 *  @param Het aantal bytes */
void cpalSchrijfRegs(const uint8_t,
                     const uint8_t,
                     uint8_t const *const,
                     const uint32_t);


#ifdef __cplusplus
}
#endif

#endif /* #define __CPALDriver_H */


