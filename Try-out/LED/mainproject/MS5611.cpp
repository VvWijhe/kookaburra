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
#define MS5611_I2C_Port GPIOB
#define MS5611_I2C I2C1
#define MS5611_TIMEOUT ((uint32_t)(0x00001000))

/** MS5611 constructor.
 * @param address I2C address
 * @see MS5611_DEFAULT_ADDRESS
 */
MS5611::MS5611(uint8_t address) {
    this->devAddr = address;
}

/** Power on and prepare for general usage.
 * This method reads coefficients stored in PROM.
 */
void MS5611::initialize(){
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
}


void MS5611::readvalues() {
    // Reading 6 calibration data values , length of a byte
    uint8_t buff[2];
    readToBuffer(devAddr, buff, MS5611_RA_C1,  2);
    C1 = buff[0]<<8 | buff[1];
    readToBuffer(devAddr, buff, MS5611_RA_C2,  2);
    C2 = buff[0]<<8 | buff[1];
    readToBuffer(devAddr, buff, MS5611_RA_C3,  2);
    C3 = buff[0]<<8 | buff[1];
    readToBuffer(devAddr, buff, MS5611_RA_C4,  2);
    C4 = buff[0]<<8 | buff[1];
    readToBuffer(devAddr, buff, MS5611_RA_C5,  2);
    C5 = buff[0]<<8 | buff[1];
    readToBuffer(devAddr, buff, MS5611_RA_C6,  2);
    C6 = buff[0]<<8 | buff[1];

    update();
}

/** Verify the I2C connection.
 * @return True if connection is valid, false otherwise
 */
bool MS5611::testConnection() {
    uint8_t data;
    readToBuffer(devAddr, &data, MS5611_RA_C0, 1);
    if (data > 0)
        return true;
    else
        return false;
}

/** Initiate the process of pressure measurement
 * @param OSR value
 * @see MS5611_RA_D1_OSR_4096
 */
void MS5611::refreshPressure(uint8_t OSR) {
    writeByte(devAddr, &OSR, 0);
}

/** Read pressure value
 */
void MS5611::readPressure() {
    //
    uint8_t buffer[3];
    readToBuffer(devAddr, buffer, MS5611_RA_ADC, 3);
    D1 = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
}

/** Initiate the process of temperature measurement
 * @param OSR value
 * @see MS5611_RA_D2_OSR_4096
 */
void MS5611::refreshTemperature(uint8_t OSR) {
    writeByte(devAddr, &OSR, 0);
}

/** Read temperature value
 */
void MS5611::readTemperature() {
    uint8_t buffer[3];
    readToBuffer(devAddr, buffer, MS5611_RA_ADC, 3);
    D2 = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
}

/** Calculate temperature and pressure calculations and perform compensation
 *  More info about these calculations is available in the datasheet.
 */
void MS5611::calculatePressureAndTemperature() {
    float dT = D2 - C5 * float(pow(2, 8));
    TEMP = (2000 + ((dT * C6) / float(pow(2, 23))));
    float OFF = C2 * float(pow(2, 16)) + (C4 * dT) / float(pow(2, 7));
    float SENS = C1 * float(pow(2, 15)) + (C3 * dT) / float(pow(2, 8));

    float T2, OFF2, SENS2;

    if (TEMP >= 2000)
    {
        T2 = 0;
        OFF2 = 0;
        SENS2 = 0;
    }
    if (TEMP < 2000)
    {
        T2 = dT * dT / float(pow(2, 31));
        OFF2 = 5 * float(pow(TEMP - 2000, 2)) / 2;
        SENS2 = OFF2 / 2;
    }
    if (TEMP < -1500)
    {
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

/** Perform pressure and temperature reading and calculation at once.
 *  Contains sleeps, better perform operations separately.
 */
void MS5611::update() {
    refreshPressure();
    delay(SystemCoreClock/8/100); // Waiting for pressure data ready 0.01 sec
    readPressure();

    refreshTemperature();
    delay(SystemCoreClock/8/100); // Waiting for temperature data ready 0.01 sec
    readTemperature();

    calculatePressureAndTemperature();
}

/** Get calculated temperature value
 @return Temperature in degrees of Celsius
 */
float MS5611::getTemperature() {
    return TEMP;
}

/** Get calculated pressure value
 @return Pressure in millibars
 */
float MS5611::getPressure() {
    return PRES;
}

float MS5611::getAltitude() {
    return toAltitude(getPressure()) * float(0.3048);
}

float MS5611::toAltitude(float pressure) {
    const float R = 287.052; // specific gas constant R*/M0
    const float g = 9.80665; // standard gravity
    const float t_grad = 0.0065; // gradient of temperature
    const float t0 = 273.15 + 15; // temperature at 0 altitude
    const float p0 = 101325; // pressure at 0 altitude

    return t0 / t_grad * (1 - float(exp((t_grad * R / g)) * log(pressure / p0)));
}

/**
 * @brief  Writes one byte to the  MPU6050.
 * @param  slaveAddr : slave address MPU6050_DEFAULT_ADDRESS
 * @param  pBuffer : pointer to the buffer  containing the data to be written to the MPU6050.
 * @param  writeAddr : address of the register in which the data will be written
 * @return None
 */
void MS5611::writeByte(u8 slaveAddr, u8 *pBuffer, u8 writeAddr) {
    /* While the bus is busy */
    waitForI2CFlag(I2C_ISR_BUSY);

    /* Send MPU6050 address for write */
    //I2C_Send7bitAddress(MPU6050_I2C, slaveAddr, I2C_Direction_Transmitter);
    I2C_TransferHandling(MS5611_I2C, slaveAddr, 2, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);

    /* Test on EV6 and clear it */
    waitForI2CFlag(I2C_ISR_TXIS);

    /* Send the MPU6050's internal address to write to */
    I2C_SendData(MS5611_I2C, writeAddr);

    /* Test on EV8 and clear it */
    waitForI2CFlag(I2C_ISR_TXIS);

    /* Send the byte to be written */
    I2C_SendData(MS5611_I2C, *pBuffer);

    /* Test on EV8 and clear it */
    waitForI2CFlag(I2C_ISR_STOPF);

    /* Send STOP condition */
    I2C_ClearFlag(MS5611_I2C, I2C_ICR_STOPCF);
}

/**
 * @brief  Reads a block of data from the MPU6050.
 * @param  slaveAddr  : slave address MPU6050_DEFAULT_ADDRESS
 * @param  pBuffer : pointer to the buffer that receives the data read from the MPU6050.
 * @param  readAddr : MPU6050's internal address to read from.
 * @param  NumByteToRead : number of bytes to read from the MPU6050 ( NumByteToRead >1  only for the Mgnetometer readinf).
 * @return None
 */
void MS5611::readToBuffer(u8 slaveAddr, u8 *pBuffer, u8 readAddr, u16 NumByteToRead) {
    // ENTR_CRT_SECTION();

    waitForI2CFlag(I2C_ISR_BUSY);

    /* Send MPU6050 address for write */
    I2C_TransferHandling(I2C1, slaveAddr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

    /* Test on EV6 and clear it */
    waitForI2CFlag(I2C_ISR_TXIS);

    /* Send the MPU6050's internal address to write to */
    I2C_SendData(MS5611_I2C, readAddr);

    /* Test on EV8 and clear it */
    waitForI2CFlag(I2C_ISR_TC);

    I2C_TransferHandling(I2C1, slaveAddr, (u8)NumByteToRead, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

    /* While there is data to be read */
    while (NumByteToRead) {
        if (NumByteToRead == 1) {
            /* Send STOP Condition */
            waitForI2CFlag(I2C_ISR_STOPF);
            I2C_ClearFlag(MS5611_I2C, I2C_ICR_STOPCF);
        }

        /* Test on EV7 and clear it */
        waitForI2CFlag(I2C_ISR_RXNE);

        /* Read a byte from the MPU6050 */
        *pBuffer = I2C_ReceiveData(MS5611_I2C);

        /* Point to the next location where the byte read will be saved */
        pBuffer++;

        /* Decrement the read bytes counter */
        NumByteToRead--;
    }
}


void MS5611::waitForI2CFlag(uint32_t flag) {
    uint32_t timeout = MS5611_TIMEOUT;

    if (flag == I2C_ISR_BUSY) {
        while (I2C_GetFlagStatus(I2C1, flag) != RESET) {
            if (timeout-- == 0) {
                commStatus = MPU6050_COMM_STATUS_ERROR;
                return;
            }
        }
    } else {
        while (I2C_GetFlagStatus(I2C1, flag) == RESET) {
            if (timeout-- == 0) {
                commStatus = MPU6050_COMM_STATUS_ERROR;
                return;
            }
        }
    }
}

/* Delay 1 sec.
**delay(SystemCoreClock/8);
*
* Delay 0.1 sec.
*delay(SystemCoreClock/8/10);
*/
void MS5611::delay(const int d)
{
    volatile int i;

    for(i=d; i>0; i--){ ; }

    return;
}