/*
 * lmacDisp.h
 *
 *  Created on: Feb 10, 2023
 *      Author: lscyane
 */

#ifndef LMACDISP_H_
#define LMACDISP_H_

typedef enum EDispNum {
	EDispNum_1,
	EDispNum_2,
	EDispNum_3,
	EDispNum_4
}EDispNum_t;

void	lmacDisp_SetChar(EDispNum_t disp, uint16_t x, uint16_t y, uint8_t char_id, uint16_t font_wid);

void	lmacDisp_Initialize(void);
void	lmacDisp_Clear(int disp_num);
void	lmacDisp_MainLoop(void);

#endif /* LMACDISP_H_ */
