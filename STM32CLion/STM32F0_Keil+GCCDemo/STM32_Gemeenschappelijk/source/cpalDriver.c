/**
  ******************************************************************************
  * @file    cpalDriver.c
  * @author  Ewout Boks
  * @version V1.1.1
  * @date    15-12-2015
  * @brief   Aangepaste driver voor de CPAL bibliotheek
  *            
  *          @note This file is a modified version of stm320518_eval_i2c_tsensor.c driver;
  *                I2C CPAL library drivers are used instead of the Standard Peripherals
  *                I2C driver.
  *                 
  *
  * $Id: cpalDriver.c 2039 2016-01-05 16:26:34Z ewout $ */


#include <cpaldriver.h>

#include <assert.h>


static CPAL_TransferTypeDef CPAL_RXTransfer = {
        /* Initialize TX Transfer structure */
        pNULL,
        0,
        0,
        0};

static CPAL_TransferTypeDef CPAL_TXTransfer = {
        /* Initialize RX Transfer structure */
        pNULL,
        0,
        0,
        0};

static CPAL_InitTypeDef *const CPAL_DevStructure = &I2C1_DevStructure;

static uint32_t txSem = 0;
static uint32_t rxSem = 0;

void cpalInit(void) {
    /* Initialize CPAL peripheral */
    CPAL_I2C_DeInit(CPAL_DevStructure);

    /* Set SysClk as I2C1 clock source */
    RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);

    /* vul met standaard waarden */
    CPAL_I2C_StructInit(CPAL_DevStructure);

    /* Set I2C clock speed */
    CPAL_DevStructure->pCPAL_I2C_Struct->I2C_Timing = 0xC062121F; //0x1045061D;
    CPAL_DevStructure->pCPAL_I2C_Struct->I2C_AnalogFilter = I2C_AnalogFilter_Disable;
    CPAL_DevStructure->pCPAL_I2C_Struct->I2C_DigitalFilter = 8;
    /* point to CPAL_TransferTypeDef structure */
    CPAL_DevStructure->pCPAL_TransferTx = &CPAL_TXTransfer;
    CPAL_DevStructure->pCPAL_TransferRx = &CPAL_RXTransfer;
    CPAL_DevStructure->CPAL_ProgModel = CPAL_PROGMODEL_DMA;
    CPAL_DevStructure->wCPAL_Options =
            CPAL_OPT_I2C_DMA_TX_IT_MASK + CPAL_OPT_I2C_DMA_RX_IT_MASK + CPAL_OPT_I2C_AUTOMATIC_END;

    const unsigned int retWaarde = CPAL_I2C_Init(CPAL_DevStructure);

    assert(retWaarde == CPAL_PASS);
}

static void wachtOpKlaar(const uint8_t adres) {
    while ((CPAL_DevStructure->CPAL_State != CPAL_STATE_READY) &&
           (CPAL_DevStructure->CPAL_State != CPAL_STATE_ERROR)) {}

    assert(CPAL_DevStructure->CPAL_State != CPAL_STATE_ERROR);

/*	CPAL_TransferTypeDef * const readytp = CPAL_DevStructure->pCPAL_TransferTx;

	readytp->wAddr1 = (uint32_t)adres;
	uint32_t klaar,resTeller=5;
	do
	{ 
		klaar = CPAL_I2C_IsDeviceReady(CPAL_DevStructure);
		resTeller--;
	} while ((CPAL_PASS != klaar) && (0!= resTeller));
	
	assert(0 != resTeller);
	*/
}

uint8_t cpalLeesReg(const uint8_t adres,
                    const uint8_t reg) {
    uint8_t veld;

    wachtOpKlaar(adres);

    /* Select Interrupt programming model and disable all options */
    CPAL_DevStructure->CPAL_ProgModel = CPAL_PROGMODEL_INTERRUPT;
    CPAL_TransferTypeDef *const tp = CPAL_DevStructure->pCPAL_TransferRx;

    tp->wAddr1 = (uint32_t) adres;
    tp->wAddr2 = reg;
    tp->wNumData = 1;
    tp->pbBuffer = &veld;

    rxSem = 1;

    /* Read Operation */
    if (CPAL_I2C_Read(CPAL_DevStructure) != CPAL_PASS) {
        assert(0);
    }

    return (veld);
}

uint16_t cpalLees2Regs(const uint8_t adres,
                       const uint8_t reg) {
    uint16_t veld;

    wachtOpKlaar(adres);

    /* Select Interrupt programming model and disable all options */
    CPAL_DevStructure->CPAL_ProgModel = CPAL_PROGMODEL_INTERRUPT;
    CPAL_TransferTypeDef *const tp = CPAL_DevStructure->pCPAL_TransferRx;

    tp->wAddr1 = (uint32_t) adres;
    tp->wAddr2 = reg;
    tp->wNumData = 2;
    tp->pbBuffer = (uint8_t * ) & veld;

    rxSem = 1;

    /* Read Operation */
    if (CPAL_I2C_Read(CPAL_DevStructure) != CPAL_PASS) {
        assert(0);
    }

    return (veld);
}

void cpalLeesRegs(const uint8_t adres,
                  const uint8_t reg,
                  uint8_t *const veldPtr,
                  const uint32_t aantal) {
    assert(0 != veldPtr);
    assert(aantal != 0);

    wachtOpKlaar(adres);

    /* Select Interrupt programming model and disable all options */
    CPAL_DevStructure->CPAL_ProgModel = CPAL_PROGMODEL_DMA;
    CPAL_TransferTypeDef *const tp = CPAL_DevStructure->pCPAL_TransferRx;

    tp->wAddr1 = (uint32_t) adres;
    tp->wAddr2 = reg;
    tp->wNumData = aantal;
    tp->pbBuffer = veldPtr;

    rxSem = 2;

    /* Read Operation */
    if (CPAL_I2C_Read(CPAL_DevStructure) != CPAL_PASS) {
        assert(0);
    } else
        do {

        } while (0 != rxSem);

}


void cpalSchrijfReg(const uint8_t adres,
                    const uint8_t reg,
                    const uint8_t waarde) {

    wachtOpKlaar(adres);

    /* Select Interrupt programming model and disable all options */
    CPAL_DevStructure->CPAL_ProgModel = CPAL_PROGMODEL_INTERRUPT;
    CPAL_TransferTypeDef *const tp = CPAL_DevStructure->pCPAL_TransferTx;

    tp->wAddr1 = (uint32_t) adres;
    tp->wAddr2 = reg;
    tp->wNumData = 1;
    tp->pbBuffer = (uint8_t * ) & waarde;

    txSem = 1;

    if (CPAL_I2C_Write(CPAL_DevStructure) != CPAL_PASS) {
        assert(0);
    } else
        do {

        } while (0 != txSem);

}

void cpalSchrijf2Regs(const uint8_t adres,
                      const uint8_t reg,
                      const uint16_t waarde) {

    wachtOpKlaar(adres);

    /* Select Interrupt programming model and disable all options */
    CPAL_DevStructure->CPAL_ProgModel = CPAL_PROGMODEL_INTERRUPT;
    CPAL_TransferTypeDef *const tp = CPAL_DevStructure->pCPAL_TransferTx;

    tp->wAddr1 = (uint32_t) adres;
    tp->wAddr2 = reg;
    tp->wNumData = 2;
    tp->pbBuffer = (uint8_t * ) & waarde;

    txSem = 1;

    if (CPAL_I2C_Write(CPAL_DevStructure) != CPAL_PASS) {
        assert(0);
    }

}


void cpalSchrijfRegs(const uint8_t adres,
                     const uint8_t reg,
                     uint8_t const *const veldPtr,
                     const uint32_t aantal) {
    assert(0 != veldPtr);
    assert(aantal != 0);

    wachtOpKlaar(adres);

    /* Select Interrupt programming model and disable all options */
    CPAL_DevStructure->CPAL_ProgModel = CPAL_PROGMODEL_DMA;
    CPAL_TransferTypeDef *const tp = CPAL_DevStructure->pCPAL_TransferTx;

    tp->wAddr1 = (uint32_t) adres;
    tp->wAddr2 = reg;
    tp->wNumData = aantal;
    tp->pbBuffer = (uint8_t *) veldPtr;

    txSem = 2;

    if (CPAL_I2C_Write(CPAL_DevStructure) != CPAL_PASS) {
        assert(0);
    }
}


/* verplichte time out funktie van de CPAL bibliotheek */
uint32_t CPAL_TIMEOUT_UserCallback(CPAL_InitTypeDef *cpalPtr) {
    assert(0);
    return (12345);
}

void CPAL_I2C_ERR_UserCallback(CPAL_DevTypeDef pDevInstance, uint32_t DeviceError) {
    assert(0);
}

void CPAL_I2C_TXTC_UserCallback(CPAL_InitTypeDef *cpalPtr) {
    assert(txSem != 0);
    txSem--;
}

void CPAL_I2C_DMATXTC_UserCallback(CPAL_InitTypeDef *cpalPtr) {
    assert(txSem != 0);
    txSem--;
}

void CPAL_I2C_RXTC_UserCallback(CPAL_InitTypeDef *cpalPtr) {
    assert(rxSem != 0);
    rxSem--;
}

void CPAL_I2C_DMARXTC_UserCallback(CPAL_InitTypeDef *cpalPtr) {
    assert(rxSem != 0);
    rxSem--;
}
