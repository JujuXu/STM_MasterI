/*
 * TT_ADXL345.c
 *
 *  Created on: May 19, 2023
 *      Author: talha
 */
#include <ADXL345.h>
#include <stdio.h>
#include <string.h>
uint8_t i;
uint8_t data_rec[6];
int16_t x, y, z;
float xg, yg, zg, angle_x, angle_y;
char accdata_string[50];

/*void TT_ADXL_Write(uint8_t reg, uint8_t value) {
	uint8_t data[2] = { 0 };
	data[0] = reg;
	data[1] = value;
	HAL_I2C_Master_Transmit(&hi2c1, ADXL_ADDRESS, data, 2, 100);
}

void TT_ADXL_Read(uint8_t reg, uint8_t numberOfBytes) {
	HAL_I2C_Mem_Read(&hi2c1, ADXL_ADDRESS, reg, 1, data_rec, numberOfBytes,
			100);

}

void TT_ADXL_Init() {
	TT_ADXL_Read(0x00, 1);
	TT_ADXL_Write(0x2D, 0);
	TT_ADXL_Write(0x2D, 0x08);
	TT_ADXL_Write(0x31, 0x01);
}
void TT_GET_ACCELERATIONS(void) {

	TT_ADXL_Read(0x32, 6);
	x = (data_rec[1] << 8) | data_rec[0];
	y = (data_rec[3] << 8) | data_rec[2];
	z = (data_rec[5] << 8) | data_rec[4];
	xg = x * .0078;
	yg = y * .0078;
	zg = z * .0078;

}


char* TT_GET_ACCELERATIONS_STRING(void) {
	    TT_ADXL_Read(0x32, 6);
		x = (data_rec[1] << 8) | data_rec[0];
		y = (data_rec[3] << 8) | data_rec[2];
		z = (data_rec[5] << 8) | data_rec[4];
		xg = x * .0078;
		yg = y * .0078;
		zg = z * .0078;
		sprintf(accdata_string, "%.2f %.2f %.2f\n\r", xg, yg, zg);
		return accdata_string;
}


float TT_GET_X_TILT_ANGLE(void) {
	TT_GET_ACCELERATIONS();
	angle_x = atan2(xg, zg);
	angle_x = angle_x * 180.0 / PI;
	return angle_x;

}
float TT_GET_Y_TILT_ANGLE(void) {
	TT_GET_ACCELERATIONS();
	angle_y = atan2(yg, zg);
	angle_y = angle_y * 180.0 / PI;
	return angle_y;



	#define ADXL345_DEVID_REG 0x00
	#define ADXL345_POWER_CTL_REG 0x2D
	#define ADXL345_DATA_FORMAT_REG 0x31
	#define ADXL345_DATAX0_REG 0x32

	HAL_StatusTypeDef ADXL345_Init(ADXL345_t *dev) {
	    uint8_t data = 0;

	    // Check device ID
	    if (HAL_I2C_Mem_Read(dev->i2c_handle, dev->dev_address << 1, ADXL345_DEVID_REG, 1, &data, 1, HAL_MAX_DELAY) != HAL_OK) {
	        return HAL_ERROR;
	    }
	    if (data != 0xE5) {
	        return HAL_ERROR;
	    }

	    // Set device to measure mode
	    data = 0x08;
	    if (HAL_I2C_Mem_Write(dev->i2c_handle, dev->dev_address << 1, ADXL345_POWER_CTL_REG, 1, &data, 1, HAL_MAX_DELAY) != HAL_OK) {
	        return HAL_ERROR;
	    }

	    return HAL_OK;
	}

	HAL_StatusTypeDef ADXL345_ReadXYZ(ADXL345_t *dev, int16_t *x, int16_t *y, int16_t *z) {
	    uint8_t buffer[6];

	    if (HAL_I2C_Mem_Read(dev->i2c_handle, dev->dev_address << 1, ADXL345_DATAX0_REG, 1, buffer, 6, HAL_MAX_DELAY) != HAL_OK) {
	        return HAL_ERROR;
	    }

	    *x = (int16_t)(buffer[1] << 8 | buffer[0]);
	    *y = (int16_t)(buffer[3] << 8 | buffer[2]);
	    *z = (int16_t)(buffer[5] << 8 | buffer[4]);

	    return HAL_OK;
	}

}*/


void ADXL345_ReadData(int16_t *x, int16_t *y, int16_t *z) {
    uint8_t buffer[6];

    // Read 6 bytes of data starting from DATAX0
    HAL_I2C_Mem_Read(&hi2c1, ADXL345_ADDRESS, ADXL345_DATAX0, 1, buffer, 6, HAL_MAX_DELAY);

    // Combine high and low bytes for each axis
    *x = (int16_t)((buffer[1] << 8) | buffer[0]);
    *y = (int16_t)((buffer[3] << 8) | buffer[2]);
    *z = (int16_t)((buffer[5] << 8) | buffer[4]);
}

void ADXL345_Init(void) {
    uint8_t data;

    // Verify device ID
    HAL_I2C_Mem_Read(&hi2c1, ADXL345_ADDRESS, ADXL345_DEVID, 1, &data, 1, HAL_MAX_DELAY);
    if (data != 0xE5) {
        // Handle error (e.g., blink an LED)
        while (1);
    }

    // Set power control to measure mode
    data = 0x08;
    HAL_I2C_Mem_Write(&hi2c1, ADXL345_ADDRESS, ADXL345_POWER_CTL, 1, &data, 1, HAL_MAX_DELAY);
}
