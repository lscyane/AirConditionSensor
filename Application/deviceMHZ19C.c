/*
 * deviceMHZ19C.c
 *
 *  Created on: Feb 12, 2023
 *      Author: lscyane
 */

/* Includes ------------------------------------------------------------------*/
// standard
#include "main.h"
// own
#include "deviceMHZ19C.h"


/* Public variables ---------------------------------------------------------*/
extern UART_HandleTypeDef huart1;


/* User code -----------------------------------------------------------------*/

/*!*************************************************
* @fn	初期化
* @param	None
* @return	None
*//*************************************************/
void deviceMHZ19C_Initialize(void)
{

}


/*!*************************************************
* @fn	CO2濃度の取得
* @param	None
* @return	CO2濃度[ppm]
*//*************************************************/
float deviceMHZ19C_GetCO2(void)
{
	uint8_t co2cmd[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79 };
	HAL_UART_Transmit(&huart1, co2cmd, sizeof(co2cmd), 100);
	HAL_UART_Receive(&huart1, co2cmd, 9, 100);

	return ((co2cmd[2]<<8) + co2cmd[3]);
}
