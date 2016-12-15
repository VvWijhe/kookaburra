//
// Created by Jamie on 8-12-2016.
//
#include <airplane.h>
#include <stm32f0_discovery.h>

uint16_t currentPitch = 0;
uint16_t currentAltitude = 0;

/* Private define ------------------------------------------------------------*/
#define RCHIGH 5.0
#define RCLOW 4.0
#define MAXATTEMPTS 10
/* Private variables ---------------------------------------------------------*/
__IO uint16_t IC2Value = 0;
__IO uint16_t DutyCycle = 0;
__IO int16_t PrevDutyCycle = 0;
float DutyCyclePC = 0;
uint8_t Attempts = 0;
uint8_t StepCount = 0;
typedef enum { OFF = 0, ON } FlightPlanner;
FlightPlanner Newstate = OFF;

MPU6050 Airplane::accelerometer;

Airplane::Airplane() {
    // Enable flash acces
    //userData.init();

    // Initialize sensors
    accelerometer.init();

    // Initialize motor
    control.initMotor();

    // Initialize servos
    control.initServo();

    // Initialize uart with interupts
    uart.init();

    // Initialize clockwork timer

    // Initialize input compare for the receiver

    // Set initiate flight parameters
    mode = MANUAL_M;

//    userData.write32(ALT1_ADRESS, 1);
//    userData.write32(ALT2_ADRESS, 2);

    // Get user altitudes
//    altitude1 = userData.read32(ALT1_ADRESS);
//    altitude2 = userData.read32(ALT2_ADRESS);

    // Test
    STM_EVAL_LEDInit(LED4);
}

void Airplane::loop() {
    while (true) {
        while (mode == MANUAL_M) {
            // Read RC controller pwm and send to the motor and servos

            // Read RC controller pattern to activate autopilot

            // Test
            STM_EVAL_LEDOn(LED4);
        }

        while (mode == AUTOPILOT_M) {
            // control height

            // control pitch

        }
    }
}

uint32_t Airplane::getAltitude() {
    return 0;
}

uint32_t Airplane::getPitch() {
    const double CONVERSIONG = 3.9;
    accelGyroDataRaw_t accelGyroDataRaw;

    accelerometer.getRawAccelGyro(&accelGyroDataRaw);

    return (uint32_t) (accelGyroDataRaw.Ax * CONVERSIONG);
    //double accelerationY = (accelGyroDataRaw.Ay * CONVERSIONG);
    //double accelerationZ = (accelGyroDataRaw.Az * CONVERSIONG);
}

void TIM15_IRQHandler(void)
{
    /* Clear TIM2 Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(TIM15, TIM_IT_CC1);

    /* Get the Input Capture value */
    IC2Value = TIM_GetCapture2(TIM15);

    if ( IC2Value > TIM_GetCapture1(TIM15) )
    {
        /* Duty cycle computation */
        DutyCycle = IC2Value - TIM_GetCapture1(TIM15);
        PrevDutyCycle = DutyCycle;
        DutyCyclePC = (float)( DutyCycle - 49200 ) / 163.2;
        if ( DutyCyclePC < 0 )
        {
            DutyCyclePC += 301.9;
        }
    }
    else
    {
        DutyCycle = PrevDutyCycle;
    }

    //from here on the flightplanner switch is processed.
    // NOTE: MAXATTEMPTS, RCHIGH and RCLOW are still trivial, give them actual values



    if ( StepCount == 0 && DutyCyclePC > RCHIGH && Newstate == OFF ) //first up
    {
        StepCount++;
        Attempts = 0;
    }
    else if ( StepCount == 1 && DutyCyclePC < RCLOW && Attempts < MAXATTEMPTS && Newstate == OFF ) //then down
    {
        StepCount++;
        Attempts = 0;
    }
    else if ( StepCount == 2 && DutyCyclePC > RCHIGH && Attempts < MAXATTEMPTS && Newstate == OFF ) //up again, enable Flightplanner
    {
        Newstate = ON;
        Attempts = 0;
        StepCount = 0;
    }
    else if ( Newstate == ON && Attempts > MAXATTEMPTS && DutyCyclePC < RCLOW ) //down when Flightplanner is active, disable it
    {
        Newstate = OFF;
        Attempts = 0;
    }

    Attempts++;
}