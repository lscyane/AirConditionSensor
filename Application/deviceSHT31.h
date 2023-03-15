/*
 * deviceSHT31.h
 *
 *  Created on: Feb 12, 2023
 *      Author: lscyane
 */

#ifndef DEVICESHT31_H_
#define DEVICESHT31_H_

void	deviceSHT31_Initialize(I2C_HandleTypeDef *i2c_handle);
void	deviceSHT31_GetTemperatureHumidity(float *temp, float *humd);

#endif /* DEVICESHT31_H_ */
