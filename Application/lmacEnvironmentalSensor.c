/*
 * lmacEnvironmentalSensor.c
 *
 *  Created on: Feb 10, 2023
 *      Author: lscyane
 */

/* Includes ------------------------------------------------------------------*/
// standard
#include "main.h"
// external
#include "deviceMHZ19C.h"
#include "deviceSHT31.h"
#include "deviceQMP6988.h"
#include "umacDisp.h"
// own
#include "lmacEnvironmentalSensor.h"


/* Public variables ---------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c2;


/* User code -----------------------------------------------------------------*/

/*!*************************************************
* @fn	初期化
* @param	None
* @return	None
*//*************************************************/
void lmacEnvSens_Initialize(void)
{
	deviceSHT31_Initialize(&hi2c2);
	deviceQMP6988_Initialize(&hi2c2);
	deviceMHZ19C_Initialize();
}


/*!*************************************************
* @fn	メインループ
* @param	None
* @return	None
*//*************************************************/
void lmacEnvSens_MainLoop(void)
{
	// 温湿度の取得、表示
	float temp, humd;
	deviceSHT31_GetTemperatureHumidity(&temp, &humd);
	umacDisp_SetHumid(humd);
	umacDisp_SetTemp(temp);

	// 気圧の取得、表示
	float air_pressure = deviceQMP6988_GetAirPressure();
	umacDisp_SetPress(air_pressure);

	// CO2の取得、表示
	float co2 = deviceMHZ19C_GetCO2();
	umacDisp_SetCo2(co2);
}


