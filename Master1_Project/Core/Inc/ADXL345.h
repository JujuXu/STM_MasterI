/*
 * TT_ADXL_345.h
 *
 *  Created on: May 19, 2023
 *      Author: talha
 */


#ifndef INC_ADXL345_H_
#define INC_ADXL345_H_
#include "stm32f4xx_hal.h"
#include <math.h>
extern I2C_HandleTypeDef hi2c1;

typedef struct {
	I2C_HandleTypeDef *i2c_handle;
	uint8_t dev_address;
} ADXL345_t;

//HAL_StatusTypeDef ADXL345_Init(ADXL345_t *dev);
HAL_StatusTypeDef ADXL345_ReadXYZ(ADXL345_t *dev, int16_t *x, int16_t *y, int16_t *z);

#define ADXL_ADDRESS 0x53
#define PI 3.141592654
void TT_ADXL_Init();
void TT_ADXL_Write(uint8_t reg, uint8_t value);
void TT_ADXL_Read(uint8_t reg, uint8_t numberOfBytes);
void TT_GET_ACCELERATIONS(void);
float TT_GET_X_TILT_ANGLE(void);
float TT_GET_Y_TILT_ANGLE(void);

void ADXL345_Init(void); // use
void ADXL345_ReadData(int16_t *x, int16_t *y, int16_t *z); // the one i use

#define ADXL345_ADDRESS 0x53 << 1
#define ADXL345_DEVID 0x00
#define ADXL345_POWER_CTL 0x2D
#define ADXL345_DATAX0 0x32
#define ADXL345_DATAX1 0x33
#define ADXL345_DATAY0 0x34
#define ADXL345_DATAY1 0x35
#define ADXL345_DATAZ0 0x36
#define ADXL345_DATAZ1 0x37

#endif /* INC_ADXL345_H_ */
