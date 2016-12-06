/*
MS5611 driver code is placed under the BSD license.
Copyright (c) 2014, Emlid Limited, www.emlid.com
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Emlid Limited nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL EMLID LIMITED BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "MS5611.h"


#define MS5611_I2C_SCL_Pin GPIO_Pin_6
#define MS5611_I2C_SDA_Pin GPIO_Pin_7
#define MS5611_I2C I2C1
#define MS5611_TIMEOUT ((uint32_t)(0x00001000))

/** MS5611 constructor.
 * @param address I2C address
 * @see MS5611_DEFAULT_ADDRESS
 */
MS5611::MS5611() {
}

/** Power on and prepare for general usage.
 * This method reads coefficients stored in PROM.
 */
void MS5611::initialize() {
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;

    RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    // Set pin B6 and B7 AF
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_1);

    // Set pin B6 and B7
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = MS5611_I2C_SCL_Pin | MS5611_I2C_SDA_Pin;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_StructInit(&I2C_InitStructure);
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
    I2C_InitStructure.I2C_DigitalFilter = 0;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_OwnAddress1 = 0;
    I2C_InitStructure.I2C_Timing = 0x2033030A; // =400 kHz. @ 48 MHz (SYSCLK) measured with LA

    I2C_Init(MS5611_I2C, &I2C_InitStructure);
    I2C_Cmd(MS5611_I2C, ENABLE);

    readPROM();
}

/** Read Cx(1-6) from the IC's PROM
 */
void MS5611::readPROM() {
    uint16_t tempC[6];

    // -------------------------- PROM read command
    for(int i = 0; i < 6; i++){
        waitForI2CFlag(I2C_ISR_BUSY);

        I2C_TransferHandling(MS5611_I2C, MS5611_ADDRESS_CSB_LOW, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);

        waitForI2CFlag(I2C_ISR_TXIS);

        I2C_SendData(MS5611_I2C, (uint8_t) (MS5611_RA_C0 + (i * 2)));

        waitForI2CFlag(I2C_ISR_STOPF);

        I2C_ClearFlag(MS5611_I2C, I2C_ICR_STOPCF);

        // Read sequence
        waitForI2CFlag(I2C_ISR_BUSY);

        I2C_TransferHandling(MS5611_I2C, MS5611_ADDRESS_CSB_LOW, 3, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

        waitForI2CFlag(I2C_ISR_RXNE);
        tempC[i] = I2C_ReceiveData(MS5611_I2C) << 8;

        waitForI2CFlag(I2C_ISR_STOPF);
        I2C_ClearFlag(MS5611_I2C, I2C_ICR_STOPCF);

        waitForI2CFlag(I2C_ISR_RXNE);
        tempC[i] = tempC[i] | I2C_ReceiveData(MS5611_I2C);
    }

    C1 = tempC[0];
    C2 = tempC[1];
    C3 = tempC[2];
    C4 = tempC[3];
    C5 = tempC[4];
    C6 = tempC[5];
}

/** Read temperature ADC from the IC.
 */
void MS5611::readTemperature() {
    // -------------------------- Convert command
    waitForI2CFlag(I2C_ISR_BUSY);

    I2C_TransferHandling(MS5611_I2C, MS5611_ADDRESS_CSB_LOW, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);

    waitForI2CFlag(I2C_ISR_TXIS);

    I2C_SendData(MS5611_I2C, MS5611_RA_D2_OSR_4096);

    waitForI2CFlag(I2C_ISR_STOPF);

    I2C_ClearFlag(MS5611_I2C, I2C_ICR_STOPCF);

    // -------------------------- ADC Read sequence
    waitForI2CFlag(I2C_ISR_BUSY);

    I2C_TransferHandling(MS5611_I2C, MS5611_ADDRESS_CSB_LOW, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);

    waitForI2CFlag(I2C_ISR_TXIS);

    I2C_SendData(MS5611_I2C, MS5611_ADC_READ);

    waitForI2CFlag(I2C_ISR_STOPF);

    I2C_ClearFlag(MS5611_I2C, I2C_ICR_STOPCF);

    // -------------------------- Read temperature sequence
    waitForI2CFlag(I2C_ISR_BUSY);

    I2C_TransferHandling(MS5611_I2C, MS5611_ADDRESS_CSB_LOW, 3, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

    waitForI2CFlag(I2C_ISR_RXNE);
    D2 = (uint32_t)I2C_ReceiveData(MS5611_I2C) << 16;
    waitForI2CFlag(I2C_ISR_RXNE);
    D2 = D2 | I2C_ReceiveData(MS5611_I2C) << 8;
    waitForI2CFlag(I2C_ISR_STOPF);
    I2C_ClearFlag(MS5611_I2C, I2C_ICR_STOPCF);
    waitForI2CFlag(I2C_ISR_RXNE);
    D2 = D2 | I2C_ReceiveData(MS5611_I2C);
}

/** Read pressure ADC from the IC.
 */
void MS5611::readPressure() {
    // -------------------------- Convert command
    waitForI2CFlag(I2C_ISR_BUSY);

    I2C_TransferHandling(MS5611_I2C, MS5611_ADDRESS_CSB_LOW, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);

    waitForI2CFlag(I2C_ISR_TXIS);

    I2C_SendData(MS5611_I2C, MS5611_RA_D1_OSR_4096);

    waitForI2CFlag(I2C_ISR_STOPF);

    I2C_ClearFlag(MS5611_I2C, I2C_ICR_STOPCF);

    // -------------------------- ADC Read command
    waitForI2CFlag(I2C_ISR_BUSY);

    I2C_TransferHandling(MS5611_I2C, MS5611_ADDRESS_CSB_LOW, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);

    waitForI2CFlag(I2C_ISR_TXIS);

    I2C_SendData(MS5611_I2C, MS5611_ADC_READ);

    waitForI2CFlag(I2C_ISR_STOPF);

    I2C_ClearFlag(MS5611_I2C, I2C_ICR_STOPCF);

    // -------------------------- Read pressure sequence
    waitForI2CFlag(I2C_ISR_BUSY);

    I2C_TransferHandling(MS5611_I2C, MS5611_ADDRESS_CSB_LOW, 3, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

    waitForI2CFlag(I2C_ISR_RXNE);
    D2 = I2C_ReceiveData(MS5611_I2C) << 16;
    waitForI2CFlag(I2C_ISR_RXNE);
    D2 = D2 | I2C_ReceiveData(MS5611_I2C) << 8;
    waitForI2CFlag(I2C_ISR_STOPF);
    I2C_ClearFlag(MS5611_I2C, I2C_ICR_STOPCF);
    waitForI2CFlag(I2C_ISR_RXNE);
    D2 = D2 | I2C_ReceiveData(MS5611_I2C);
}

/** Calculate temperature and pressure calculations and perform compensation
 *  More info about these calculations is available in the datasheet.
 */
void MS5611::calculate() {
    float dT = D2 - C5 * float(pow(2, 8));
    TEMP = (2000 + ((dT * C6) / float(pow(2, 23))));
    float OFF = C2 * float(pow(2, 16)) + (C4 * dT) / float(pow(2, 7));
    float SENS = C1 * float(pow(2, 15)) + (C3 * dT) / float(pow(2, 8));

    float T2 = 0, OFF2 = 0, SENS2 = 0;

    if (TEMP >= 2000) {
        T2 = 0;
        OFF2 = 0;
        SENS2 = 0;
    }
    if (TEMP < 2000) {
        T2 = dT * dT / float(pow(2, 31));
        OFF2 = 5 * float(pow(TEMP - 2000, 2)) / 2;
        SENS2 = OFF2 / 2;
    }
    if (TEMP < -1500) {
        OFF2 = OFF2 + 7 * float(pow(TEMP + 1500, 2));
        SENS2 = SENS2 + 11 * float(pow(TEMP + 1500, 2)) / 2;
    }

    TEMP = TEMP - T2;
    OFF = OFF - OFF2;
    SENS = SENS - SENS2;

    // Final calculations
    PRES = ((D1 * SENS) / float(pow(2, 21)) - OFF) / float(pow(2, 15)) / 100;
    TEMP = TEMP / 100;
}

float MS5611::toAltitude(float pressure) {
    const float R = 287.052; // specific gas constant R*/M0
    const float g = 9.80665; // standard gravity
    const float t_grad = 0.0065; // gradient of temperature
    const float t0 = (const float) (273.15 + 15.0); // temperature at 0 altitude
    const float p0 = 101325.0; // pressure at 0 altitude

    return t0 / t_grad * (1 - float(exp((t_grad * R / g)) * log(pressure / p0)));
}

float MS5611::getTemperature() {
    readTemperature();

    calculate();

    return TEMP;
}

float MS5611::getAltitude() {
    readPressure();

    calculate();

    return toAltitude(PRES);
}

void MS5611::waitForI2CFlag(uint32_t flag) {
    uint32_t timeout = MS5611_TIMEOUT;

    if (flag == I2C_ISR_BUSY) {
        while (I2C_GetFlagStatus(I2C1, flag) != RESET) {
            if (timeout-- == 0) {
                deviceStatus = MPU6050_COMM_STATUS_ERROR;
                return;
            }
        }
    } else {
        while (I2C_GetFlagStatus(I2C1, flag) == RESET) {
            if (timeout-- == 0) {
                deviceStatus = MPU6050_COMM_STATUS_ERROR;
                return;
            }
        }
    }
}
