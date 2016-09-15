#ifndef BMP180Driver_H
#define BMP180Driver_H

#include <stdint.h>

void bmp180Init(void);

void bmp180Calibreer(void);


/*! @note druk sampling modes */
typedef enum {
    P0_MEASURE = 0,// pressure measurement (OSS=0, 4.5ms)
    P1_MEASURE, // pressure measurement (OSS=1, 7.5ms)
    P2_MEASURE, // pressure measurement (OSS=2, 13.5ms)
    P3_MEASURE, // pressure measurement (OSS=3, 25.5ms)
    LaatsteBMP180DrukMode
} BMP180DrukMode;

/*! @brief lees de druk.
   @param de BMP180 sampling mode.
 * @return geef de druk terug in Pa */
int32_t bmp180LeesDruk(const BMP180DrukMode);

/*! @brief lees de temperatuur.
 * @return geef de temperatuur terug in 10*graden C, b.v 151 = 15,1 graden C */
int16_t bmp180LeesTemp(void);


#endif /* BMP180Driver_H */
