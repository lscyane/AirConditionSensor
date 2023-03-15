/*
 * deviceQMP6988.c
 *
 *  Created on: Feb 12, 2023
 *      Author: lscyane
 */

/* Includes ------------------------------------------------------------------*/
// standard
#include "main.h"
// own
#include "deviceQMP6988.h"
// debug
#include "usbd_cdc_if.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>


/* Private define ------------------------------------------------------------*/
#define QMP6988_I2C_ADDR	(0x70 << 1)

#define QMP6988_U16_t unsigned short
#define QMP6988_S16_t short
#define QMP6988_U32_t unsigned int
#define QMP6988_S32_t int
#define QMP6988_U64_t unsigned long long
#define QMP6988_S64_t long long

#define QMP6988_CALIBRATION_DATA_LENGTH	(25)
#define QMP6988_CALIBRATION_DATA_START		(0xA0)	/* QMP6988 compensation coefficients */

#define SHIFT_RIGHT_4_POSITION				(4)
#define SHIFT_LEFT_2_POSITION				(2)
#define SHIFT_LEFT_4_POSITION				(4)
#define SHIFT_LEFT_5_POSITION				(5)
#define SHIFT_LEFT_8_POSITION				(8)
#define SHIFT_LEFT_12_POSITION				(12)
#define SHIFT_LEFT_16_POSITION				(16)


/* Private typedef -----------------------------------------------------------*/
typedef struct Qmp6988CaliData {
	QMP6988_S32_t COE_a0;
	QMP6988_S16_t COE_a1;
	QMP6988_S16_t COE_a2;
	QMP6988_S32_t COE_b00;
	QMP6988_S16_t COE_bt1;
	QMP6988_S16_t COE_bt2;
	QMP6988_S16_t COE_bp1;
	QMP6988_S16_t COE_b11;
	QMP6988_S16_t COE_bp2;
	QMP6988_S16_t COE_b12;
	QMP6988_S16_t COE_b21;
	QMP6988_S16_t COE_bp3;
}qmp6988_cali_data_t;

typedef struct Qmp6988IkData {
	QMP6988_S32_t a0, b00;
	QMP6988_S32_t a1, a2;
	QMP6988_S64_t bt1, bt2, bp1, b11, bp2, b12, b21, bp3;
}qmp6988_ik_data_t;

typedef struct Qmp6988Data {
	uint8_t chip_id;
	uint8_t power_mode;
	float temperature;
	float pressure;
	float altitude;
	qmp6988_cali_data_t qmp6988_cali;
	qmp6988_ik_data_t ik;
}qmp6988_data_t;


/* Private variables ---------------------------------------------------------*/
static I2C_HandleTypeDef *hi2c = NULL;
static qmp6988_data_t qmp6988 = {0};


/* User code -----------------------------------------------------------------*/

static void QMP6988_LOG (const char * format, ...)
{
    va_list args;
    va_start(args, format);

	char string[256];
	uint16_t stlen = snprintf(string, sizeof(string), format, args);
	CDC_Transmit_FS((uint8_t *)string, stlen);

    va_end(args);
}


static void CalibrationDataGet(void)
{
    // BITFIELDS temp_COE;
    uint8_t a_data_uint8_tr[QMP6988_CALIBRATION_DATA_LENGTH] = {0};

    HAL_I2C_Mem_Read(hi2c, QMP6988_I2C_ADDR, 0xA0, I2C_MEMADD_SIZE_8BIT, a_data_uint8_tr, QMP6988_CALIBRATION_DATA_LENGTH, 1000);

    qmp6988.qmp6988_cali.COE_a0 = (QMP6988_S32_t)(((a_data_uint8_tr[18] << SHIFT_LEFT_12_POSITION) | (a_data_uint8_tr[19] << SHIFT_LEFT_4_POSITION) | (a_data_uint8_tr[24] & 0x0f)) << 12);
    qmp6988.qmp6988_cali.COE_a0 = qmp6988.qmp6988_cali.COE_a0 >> 12;

    qmp6988.qmp6988_cali.COE_a1 =  (QMP6988_S16_t)(((a_data_uint8_tr[20]) << SHIFT_LEFT_8_POSITION) | a_data_uint8_tr[21]);
    qmp6988.qmp6988_cali.COE_a2 =  (QMP6988_S16_t)(((a_data_uint8_tr[22]) << SHIFT_LEFT_8_POSITION) | a_data_uint8_tr[23]);

    qmp6988.qmp6988_cali.COE_b00 = (QMP6988_S32_t)(((a_data_uint8_tr[0] << SHIFT_LEFT_12_POSITION) | (a_data_uint8_tr[1] << SHIFT_LEFT_4_POSITION) | ((a_data_uint8_tr[24] & 0xf0) >> SHIFT_RIGHT_4_POSITION)) << 12);
    qmp6988.qmp6988_cali.COE_b00 = qmp6988.qmp6988_cali.COE_b00 >> 12;
    qmp6988.qmp6988_cali.COE_bt1 = (QMP6988_S16_t)(((a_data_uint8_tr[ 2]) << SHIFT_LEFT_8_POSITION) | a_data_uint8_tr[3]);
    qmp6988.qmp6988_cali.COE_bt2 = (QMP6988_S16_t)(((a_data_uint8_tr[ 4]) << SHIFT_LEFT_8_POSITION) | a_data_uint8_tr[5]);
    qmp6988.qmp6988_cali.COE_bp1 = (QMP6988_S16_t)(((a_data_uint8_tr[ 6]) << SHIFT_LEFT_8_POSITION) | a_data_uint8_tr[7]);
    qmp6988.qmp6988_cali.COE_b11 = (QMP6988_S16_t)(((a_data_uint8_tr[ 8]) << SHIFT_LEFT_8_POSITION) | a_data_uint8_tr[9]);
    qmp6988.qmp6988_cali.COE_bp2 = (QMP6988_S16_t)(((a_data_uint8_tr[10]) << SHIFT_LEFT_8_POSITION) | a_data_uint8_tr[11]);
    qmp6988.qmp6988_cali.COE_b12 = (QMP6988_S16_t)(((a_data_uint8_tr[12]) << SHIFT_LEFT_8_POSITION) | a_data_uint8_tr[13]);
    qmp6988.qmp6988_cali.COE_b21 = (QMP6988_S16_t)(((a_data_uint8_tr[14]) << SHIFT_LEFT_8_POSITION) | a_data_uint8_tr[15]);
    qmp6988.qmp6988_cali.COE_bp3 = (QMP6988_S16_t)(((a_data_uint8_tr[16]) << SHIFT_LEFT_8_POSITION) | a_data_uint8_tr[17]);

    QMP6988_LOG(">-----------calibration data--------------<\r\n");
    QMP6988_LOG("COE_a0[%d]	COE_a1[%d]	COE_a2[%d]\r\nCOE_b00[%d]\r\n",
                qmp6988.qmp6988_cali.COE_a0, qmp6988.qmp6988_cali.COE_a1,
                qmp6988.qmp6988_cali.COE_a2, qmp6988.qmp6988_cali.COE_b00);
    QMP6988_LOG("COE_bt1[%d]	COE_bt2[%d]	COE_bp1[%d]	COE_b11[%d]\r\n",
                qmp6988.qmp6988_cali.COE_bt1, qmp6988.qmp6988_cali.COE_bt2,
                qmp6988.qmp6988_cali.COE_bp1, qmp6988.qmp6988_cali.COE_b11);
    QMP6988_LOG("COE_bp2[%d]	COE_b12[%d]	COE_b21[%d]	COE_bp3[%d]\r\n",
                qmp6988.qmp6988_cali.COE_bp2, qmp6988.qmp6988_cali.COE_b12,
                qmp6988.qmp6988_cali.COE_b21, qmp6988.qmp6988_cali.COE_bp3);
    QMP6988_LOG("<-----------calibration data-------------->\r\n");

    qmp6988.ik.a0  = qmp6988.qmp6988_cali.COE_a0;   // 20Q4
    qmp6988.ik.b00 = qmp6988.qmp6988_cali.COE_b00;  // 20Q4

    qmp6988.ik.a1 =    3608L * (QMP6988_S32_t)qmp6988.qmp6988_cali.COE_a1  - 1731677965L;  	// 31Q23
    qmp6988.ik.a2 =   16889L * (QMP6988_S32_t)qmp6988.qmp6988_cali.COE_a2  -   87619360L;  	// 30Q47

    qmp6988.ik.bt1 =   2982L * (QMP6988_S64_t)qmp6988.qmp6988_cali.COE_bt1 +  107370906L;  	// 28Q15
    qmp6988.ik.bt2 = 329854L * (QMP6988_S64_t)qmp6988.qmp6988_cali.COE_bt2 +  108083093L;  	// 34Q38
    qmp6988.ik.bp1 =  19923L * (QMP6988_S64_t)qmp6988.qmp6988_cali.COE_bp1 + 1133836764L;  	// 31Q20
    qmp6988.ik.b11 =   2406L * (QMP6988_S64_t)qmp6988.qmp6988_cali.COE_b11 +  118215883L; 	// 28Q34
    qmp6988.ik.bp2 =   3079L * (QMP6988_S64_t)qmp6988.qmp6988_cali.COE_bp2 -  181579595L;  	// 29Q43
    qmp6988.ik.b12 =   6846L * (QMP6988_S64_t)qmp6988.qmp6988_cali.COE_b12 +   85590281L;  	// 29Q53
    qmp6988.ik.b21 =  13836L * (QMP6988_S64_t)qmp6988.qmp6988_cali.COE_b21 +   79333336L;  	// 29Q60
    qmp6988.ik.bp3 =   2915L * (QMP6988_S64_t)qmp6988.qmp6988_cali.COE_bp3 +  157155561L;  	// 28Q65

    QMP6988_LOG(">----------- int calibration data --------------<\r\n");
    QMP6988_LOG("a0[%d]	a1[%d] a2[%d] b00[%d]\r\n", qmp6988.ik.a0, qmp6988.ik.a1, qmp6988.ik.a2, qmp6988.ik.b00);
    QMP6988_LOG("bt1[%ld]	bt2[%ld]	bp1[%ld]	b11[%ld]\r\n", qmp6988.ik.bt1, qmp6988.ik.bt2, qmp6988.ik.bp1, qmp6988.ik.b11);
    QMP6988_LOG("bp2[%ld]	b12[%ld]	b21[%ld]	bp3[%ld]\r\n", qmp6988.ik.bp2, qmp6988.ik.b12, qmp6988.ik.b21, qmp6988.ik.bp3);
    QMP6988_LOG("<----------- int calibration data -------------->\r\n");
}


static QMP6988_S16_t convTx02e(qmp6988_ik_data_t* ik, QMP6988_S32_t dt)
{
    QMP6988_S16_t ret;
    QMP6988_S64_t wk1, wk2;

    // wk1: 60Q4 // bit size
    wk1 = ((QMP6988_S64_t)ik->a1 * (QMP6988_S64_t)dt);  		// 31Q23+24-1=54 (54Q23)
    wk2 = ((QMP6988_S64_t)ik->a2 * (QMP6988_S64_t)dt) >> 14; 	// 30Q47+24-1=53 (39Q33)
    wk2 = (wk2 * (QMP6988_S64_t)dt) >> 10;       				// 39Q33+24-1=62 (52Q23)
    wk2 = ((wk1 + wk2) / 32767) >> 19;           				// 54,52->55Q23 (20Q04)
    ret = (QMP6988_S16_t)((ik->a0 + wk2) >> 4);  				// 21Q4 -> 17Q0
    return ret;
}


static QMP6988_S32_t getPressure02e(qmp6988_ik_data_t* ik, QMP6988_S32_t dp, QMP6988_S16_t tx)
{
    QMP6988_S32_t ret;
    QMP6988_S64_t wk1, wk2, wk3;

    // wk1 = 48Q16 // bit size
    wk1 = ((QMP6988_S64_t)ik->bt1 * (QMP6988_S64_t)tx);  		// 28Q15+16-1=43 (43Q15)
    wk2 = ((QMP6988_S64_t)ik->bp1 * (QMP6988_S64_t)dp) >> 5;    // 31Q20+24-1=54 (49Q15)
    wk1 += wk2;  												// 43,49->50Q15
    wk2 = ((QMP6988_S64_t)ik->bt2 * (QMP6988_S64_t)tx) >> 1;    // 34Q38+16-1=49 (48Q37)
    wk2 = (wk2 * (QMP6988_S64_t)tx) >> 8;  						// 48Q37+16-1=63 (55Q29)
    wk3 = wk2;                            						// 55Q29
    wk2 = ((QMP6988_S64_t)ik->b11 * (QMP6988_S64_t)tx) >> 4;    // 28Q34+16-1=43 (39Q30)
    wk2 = (wk2 * (QMP6988_S64_t)dp) >> 1;  						// 39Q30+24-1=62 (61Q29)
    wk3 += wk2;                            						// 55,61->62Q29
    wk2 = ((QMP6988_S64_t)ik->bp2 * (QMP6988_S64_t)dp) >> 13;   // 29Q43+24-1=52 (39Q30)
    wk2 = (wk2 * (QMP6988_S64_t)dp) >> 1;  						// 39Q30+24-1=62 (61Q29)
    wk3 += wk2;                            						// 62,61->63Q29
    wk1 += wk3 >> 14;                      						// Q29 >> 14 -> Q15
    wk2 = ((QMP6988_S64_t)ik->b12 * (QMP6988_S64_t)tx);  		// 29Q53+16-1=45 (45Q53)
    wk2 = (wk2 * (QMP6988_S64_t)tx) >> 22;             			// 45Q53+16-1=61 (39Q31)
    wk2 = (wk2 * (QMP6988_S64_t)dp) >> 1;              			// 39Q31+24-1=62 (61Q30)
    wk3 = wk2;                                         			// 61Q30
    wk2 = ((QMP6988_S64_t)ik->b21 * (QMP6988_S64_t)tx) >> 6;    // 29Q60+16-1=45 (39Q54)
    wk2 = (wk2 * (QMP6988_S64_t)dp) >> 23;  					// 39Q54+24-1=62 (39Q31)
    wk2 = (wk2 * (QMP6988_S64_t)dp) >> 1;   					// 39Q31+24-1=62 (61Q20)
    wk3 += wk2;                             					// 61,61->62Q30
    wk2 = ((QMP6988_S64_t)ik->bp3 * (QMP6988_S64_t)dp) >> 12;   // 28Q65+24-1=51 (39Q53)
    wk2 = (wk2 * (QMP6988_S64_t)dp) >> 23;  					// 39Q53+24-1=62 (39Q30)
    wk2 = (wk2 * (QMP6988_S64_t)dp);        					// 39Q30+24-1=62 (62Q30)
    wk3 += wk2;                            						// 62,62->63Q30
    wk1 += wk3 >> 15;                       					// Q30 >> 15 = Q15
    wk1 /= 32767L;
    wk1 >>= 11;      											// Q15 >> 7 = Q4
    wk1 += ik->b00;  											// Q4 + 20Q4
    // wk1 >>= 4; 												// 28Q4 -> 24Q0
    ret = (QMP6988_S32_t)wk1;
    return ret;
}


/*!*************************************************
* @fn	初期化
* @param	i2cハンドル
* @return	None
*//*************************************************/
void deviceQMP6988_Initialize(I2C_HandleTypeDef *i2c_handle)
{
	hi2c = i2c_handle;

	uint8_t val[10];

	// IO_SETUP
	//HAL_I2C_Mem_Write(&hi2c3, QMP6988_I2C_ADDR, 0xF5, I2C_MEMADD_SIZE_8BIT, val, 1, 1000);

	// CTRL_MEAS
	val[0] = 0xFF;
	HAL_I2C_Mem_Write(hi2c, QMP6988_I2C_ADDR, 0xF4, I2C_MEMADD_SIZE_8BIT, val, 1, 1000);


	CalibrationDataGet();
}


/*!*************************************************
* @fn	気圧の取得
* @param	None
* @return	気圧[hPa]
*//*************************************************/
float deviceQMP6988_GetAirPressure(void)
{
	uint8_t val[3];

	// 気温生データ取得 (補正計算用)
	HAL_I2C_Mem_Read(hi2c, QMP6988_I2C_ADDR, 0xFA, I2C_MEMADD_SIZE_8BIT, val, sizeof(val), 1000);
	QMP6988_S32_t rawT = (int32_t)((uint32_t)val[0]<<16) + ((uint32_t)val[1]<<8) + ((uint32_t)val[2]) - (2 << 22);

	// 気圧生データ取得
	HAL_I2C_Mem_Read(hi2c, QMP6988_I2C_ADDR, 0xF7, I2C_MEMADD_SIZE_8BIT, val, sizeof(val), 1000);
	QMP6988_S32_t rawP = (int32_t)((uint32_t)val[0]<<16) + ((uint32_t)val[1]<<8) + ((uint32_t)val[2]) - (2 << 22);

	// 温度補正込気圧計算
	QMP6988_S16_t tInt  = convTx02e(&qmp6988.ik, rawT);
	QMP6988_U32_t pInt = getPressure02e(&qmp6988.ik, rawP, tInt);

	// デバッグ出力
	char string[50] = {0};
	int stlen = snprintf(string, sizeof(string), "%f / %f\r\n\r\n", tInt / 256.0 , pInt / 16.0 / 100);
	CDC_Transmit_FS((uint8_t *)string, stlen);

	// hPa 単位にして返す
	return (float)( pInt / 16.0 / 100.0);
}

