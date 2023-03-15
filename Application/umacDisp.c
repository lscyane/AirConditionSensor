/*
 * umacDisp.c
 *
 *  Created on: Feb 6, 2023
 *      Author: lscyane
 */

/* Includes ------------------------------------------------------------------*/
// standard
#include <stdint.h>
// external
#include "lmacDisp.h"
// own
#include "umacDisp.h"

#define CHAR1X (20)
#define CHAR2X (40)
#define CHAR3X (60)
#define CHAR4X (80)


/* User code -----------------------------------------------------------------*/

/*!*************************************************
* @fn	温度の表示
* @param	温度[℃]
* @return	None
*//*************************************************/
void umacDisp_SetTemp(float temp)
{
	uint8_t minus_flg = 0;

	// 負値の時は負値表示用のフラグを立てておいて絶対値をとる
	if (temp < 0) {
		temp *= -1;
		temp += 0.1;		// -0.0℃ と表示させないために切り上げる
		minus_flg = 1;
	}

	int dec10 = (((int)temp) / 10) % 10;
	int dec01 = (((int)temp)     ) % 10;
	int point = ((int)(temp * 10)) % 10;

	if (minus_flg) {
		// 負値の時はマイナスを表示
		lmacDisp_SetChar(EDispNum_1, CHAR1X, 0, 12,	FONT_WIDTH);
	} else if (dec10 == 0) {
		// 2桁目が0の時は空白を表示
		lmacDisp_SetChar(EDispNum_1, CHAR1X, 0, 11, FONT_WIDTH);
	} else {
		lmacDisp_SetChar(EDispNum_1, CHAR1X, 0, dec10,	FONT_WIDTH);
	}
	lmacDisp_SetChar(EDispNum_1, CHAR2X, 0, dec01,	FONT_WIDTH);
	lmacDisp_SetChar(EDispNum_1, CHAR3X, 0, 10,		FONT_WIDTH);
	lmacDisp_SetChar(EDispNum_1, CHAR4X, 0, point,	FONT_WIDTH);
}


/*!*************************************************
* @fn	湿度の表示
* @param	湿度[％]
* @return	None
*//*************************************************/
void umacDisp_SetHumid(float humid)
{
	if (humid > 0) {
		int dec10 = (((int)humid) / 10) % 10;
		int dec01 = (((int)humid)     ) % 10;

		if (dec10 == 0) {
			// 2桁目が0の時は空白を表示
			lmacDisp_SetChar(EDispNum_3, CHAR2X, 0, 11, FONT_WIDTH);
		} else {
			lmacDisp_SetChar(EDispNum_3, CHAR2X, 0, dec10, FONT_WIDTH);
		}
		lmacDisp_SetChar(EDispNum_3, CHAR3X, 0, dec01, FONT_WIDTH);
	} else {
		// 0以下の異常値は－－表示
		lmacDisp_SetChar(EDispNum_3, CHAR2X, 0, 12, FONT_WIDTH);
		lmacDisp_SetChar(EDispNum_3, CHAR3X, 0, 12, FONT_WIDTH);
	}
}


/*!*************************************************
* @fn	CO2濃度の表示
* @param	CO2濃度[ppm]
* @return	None
*//*************************************************/
void umacDisp_SetCo2(float co2)
{
	if (co2 > 0) {
		int dec1000 = (((int)co2) / 1000) % 10;
		int dec0100 = (((int)co2) / 100 ) % 10;
		int dec0010 = (((int)co2) / 10  ) % 10;
		int dec0001 = (((int)co2)       ) % 10;

		if (dec1000 == 0) {
			// 4桁目が0の時は空白を表示
			lmacDisp_SetChar(EDispNum_2, CHAR1X, 0, 11, FONT_WIDTH);
		} else {
			lmacDisp_SetChar(EDispNum_2, CHAR1X, 0, dec1000, FONT_WIDTH);
		}
		lmacDisp_SetChar(EDispNum_2, CHAR2X, 0, dec0100, FONT_WIDTH);
		lmacDisp_SetChar(EDispNum_2, CHAR3X, 0, dec0010, FONT_WIDTH);
		lmacDisp_SetChar(EDispNum_2, 80, 0, dec0001, FONT_WIDTH);
	} else {
		// 0以下の異常値は－－－－表示
		lmacDisp_SetChar(EDispNum_2, CHAR1X, 0, 12, FONT_WIDTH);
		lmacDisp_SetChar(EDispNum_2, CHAR2X, 0, 12, FONT_WIDTH);
		lmacDisp_SetChar(EDispNum_2, CHAR3X, 0, 12, FONT_WIDTH);
		lmacDisp_SetChar(EDispNum_2, CHAR4X, 0, 12, FONT_WIDTH);
	}
}


/*!*************************************************
* @fn	気圧の表示
* @param	気圧[hPa]
* @return	None
*//*************************************************/
void umacDisp_SetPress(float press)
{
	if (press > 0) {
		int dec1000 = (((int)press) / 1000) % 10;
		int dec0100 = (((int)press) / 100 ) % 10;
		int dec0010 = (((int)press) / 10  ) % 10;
		int dec0001 = (((int)press)       ) % 10;

		if (dec1000 == 0) {
			// 4桁目が0の時は空白を表示
			lmacDisp_SetChar(EDispNum_4, CHAR1X, 0, 11, FONT_WIDTH);
		} else {
			lmacDisp_SetChar(EDispNum_4, CHAR1X, 0, dec1000, FONT_WIDTH);
		}
		lmacDisp_SetChar(EDispNum_4, CHAR2X, 0, dec0100, FONT_WIDTH);
		lmacDisp_SetChar(EDispNum_4, CHAR3X, 0, dec0010, FONT_WIDTH);
		lmacDisp_SetChar(EDispNum_4, CHAR4X, 0, dec0001, FONT_WIDTH);
	} else {
		// 0以下の異常値は－－－－表示
		lmacDisp_SetChar(EDispNum_4, CHAR1X, 0, 12, FONT_WIDTH);
		lmacDisp_SetChar(EDispNum_4, CHAR2X, 0, 12, FONT_WIDTH);
		lmacDisp_SetChar(EDispNum_4, CHAR3X, 0, 12, FONT_WIDTH);
		lmacDisp_SetChar(EDispNum_4, CHAR4X, 0, 12, FONT_WIDTH);
	}
}
