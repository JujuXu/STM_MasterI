/*
 * TT_ADXL345.c
 *
 *  Created on: May 19, 2023
 *      Author: talha
 */
#include <ADXL345.h>
#include <stdio.h>
#include <string.h>

// Global variables for data storage and processing
uint8_t i;
uint8_t data_rec[6];
int16_t x, y, z;
float xg, yg, zg, angle_x, angle_y;
char accdata_string[50];

/**
 * @brief Reads acceleration data from the ADXL345 sensor.
 * @param x Pointer to store X-axis data
 * @param y Pointer to store Y-axis data
 * @param z Pointer to store Z-axis data
 */
void ADXL345_ReadData(int16_t *x, int16_t *y, int16_t *z) {
	uint8_t buffer[6];

	// Read 6 bytes of data starting from DATAX0 register
	HAL_I2C_Mem_Read(&hi2c1, ADXL345_ADDRESS, ADXL345_DATAX0, 1, buffer, 6, HAL_MAX_DELAY);

	// Combine high and low bytes for each axis
	*x = (int16_t)((buffer[1] << 8) | buffer[0]); // X-axis
	*y = (int16_t)((buffer[3] << 8) | buffer[2]); // Y-axis
	*z = (int16_t)((buffer[5] << 8) | buffer[4]); // Z-axis
}

/**
 * @brief Initializes the ADXL345 sensor.
 */
void ADXL345_Init(void) {
	uint8_t data;

	// Read and verify device ID
	HAL_I2C_Mem_Read(&hi2c1, ADXL345_ADDRESS, ADXL345_DEVID, 1, &data, 1, HAL_MAX_DELAY);
	if (data != 0xE5) {
		// Device ID mismatch, handle error (e.g., blink an LED or halt)
		while (1);
	}

	// Set power control register to measurement mode
	data = 0x08;
	HAL_I2C_Mem_Write(&hi2c1, ADXL345_ADDRESS, ADXL345_POWER_CTL, 1, &data, 1, HAL_MAX_DELAY);
}
