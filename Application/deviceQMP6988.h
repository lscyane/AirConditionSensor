/*
 * deviceQMP6988.h
 *
 *  Created on: Feb 12, 2023
 *      Author: lscyane
 */

#ifndef DEVICEQMP6988_H_
#define DEVICEQMP6988_H_

void	deviceQMP6988_Initialize(I2C_HandleTypeDef *i2c_handle);

float	deviceQMP6988_GetAirPressure(void);

#endif /* DEVICEQMP6988_H_ */
