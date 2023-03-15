/*
 * deviceSHT31.c
 *
 *  Created on: Feb 12, 2023
 *      Author: lscyane
 */

/* Includes ------------------------------------------------------------------*/
// standard
#include "main.h"
// own
#include "deviceSHT31.h"


/* Private define ------------------------------------------------------------*/
#define SHT31_I2C_ADDR		(0x44 << 1)


/* private variables ---------------------------------------------------------*/
static I2C_HandleTypeDef *hi2c = NULL;


/* User code -----------------------------------------------------------------*/

/*!*************************************************
* @fn	初期化
* @param	i2cハンドル
* @return	None
*//*************************************************/
void deviceSHT31_Initialize(I2C_HandleTypeDef *i2c_handle)
{
	hi2c = i2c_handle;
}


/*!*************************************************
* @fn	温度湿度の取得
* @param[out]	温度[℃]
* @param[out]	湿度[％]
* @return		None
*//*************************************************/
void deviceSHT31_GetTemperatureHumidity(float *temp, float *humd)
{
	uint8_t	sh31_data[6] = {0};
	uint8_t	sht31_measure_cmd[] = {0x24, 00};

	// 測定コマンド送信 SingleShot クロックストレッチ無効　高精度
	HAL_I2C_Master_Transmit(hi2c, SHT31_I2C_ADDR , sht31_measure_cmd, sizeof(sht31_measure_cmd), 1000);

	// 測定完了待機 高精度15msec以上
	HAL_Delay(20);

	// 測定値読み出し
	HAL_I2C_Master_Receive(hi2c, SHT31_I2C_ADDR, sh31_data, 6, 1000);

	// 温度計算
	*temp = (float)(-45.0 + 175.0 * (sh31_data[0] << 8 | sh31_data[1]) / 65535.0);

	// 湿度計算
	*humd = 100.0 * (sh31_data[3] << 8 | sh31_data[4]) / 65535.0;
}

