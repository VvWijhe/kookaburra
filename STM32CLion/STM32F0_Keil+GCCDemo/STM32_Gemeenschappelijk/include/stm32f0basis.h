/**
  ******************************************************************************
  * @file    Driver voor STM32F0
  * @author  Ewout Boks
  * @version V1.1.1
  * @date    15-12-2015
  * @brief   Aangepaste driver voor de STM32F0
  *            
  *                 
  * $Id: stm32f0basis.h 2039 2016-01-05 16:26:34Z ewout $ */

#ifndef STM32F0Basis_H
#define STM32F0Basis_H

#include <stdint.h>
#include <stm32f0xx.h>

typedef unsigned int Teller;

void wachtklokInit(void);

void wachtMilliSecs(const uint32_t msecs);

class LEDDriver {
public:
    /*! @brief initialiseer de LEDs op het discovery bord*/
    LEDDriver();

    typedef enum {
        LED3 = 0,
        LED4,
        LEDAantalLEDs
    } LEDid;

    /*! @brief Schakel een LED aan/uit.
        *  @param LED Id.
        *  @param aan (true) of uit (false) */
    void schakel(const LEDid,
                 const bool);

    /*! @brief Schakel een LED om.
        *  @param LED Id. */
    void om(const LEDid);

private:
    static const uint16_t pinSources[LEDAantalLEDs];
};

/*! @brief initialiseer de uart */
void USARTinit(void);

/*! @brief stuur een tekststring de uart uit */
void USARTsend(char const *);

#endif 
