/*
 * umacDisp.h
 *
 *  Created on: Feb 6, 2023
 *      Author: lscyane
 */

#ifndef UMACDISP_H_
#define UMACDISP_H_

#define FONT_WIDTH		(20)

void	umacDisp_SetTemp(float temp);
void	umacDisp_SetHumid(float humid);
void	umacDisp_SetCo2(float co2);
void	umacDisp_SetPress(float press);

#endif /* UMACDISP_H_ */
