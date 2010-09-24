/**************************************** 
 * Stefan Wackwitz			*
 * main.h			        *
 ***************************************/

#ifndef __MAIN_H
#define __MAIN_H

/* includes ----------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>

#include "stm32f10x.h" // libary von ST Microelectronics

#include "string.h"

#include "config.h"
#include "display.h"
#include "holder.h"

/* variables ---------------------------------------------------------------- */
extern uint8_t menu_item; // Variable für den Menüpunkt | 0 = Tacho | 1 = Cells

/* defines -------------------------------------------------------------------*/
#define VOLT1_1              1
#define VOLT1_2              2
#define VOLT1_3              3
#define VOLT1_4              4
#define VOLT1_5              5
#define VOLT1_6              6
#define VOLT1_7              7
#define VOLT1_8              8
#define VOLT1_9              9
#define VOLT1_10             10
#define VOLT1_11             11
#define VOLT1_12             12
#define VOLT2_1              13
#define VOLT2_2              14
#define VOLT2_3              15
#define VOLT2_4              16
#define VOLT2_5              17
#define VOLT2_6              18
#define VOLT2_7              19
#define VOLT2_8              20
#define VOLT2_9              21
#define VOLT2_10             22
#define VOLT2_11             23
#define VOLT2_12             24

#define CURR_1               25
#define CURR_2               26
#define CURRLOW_1            27
#define CURRLOW_2            28
#define CURR_CENTER          29

#define TEMP1_1              30
#define TEMP1_2              31
#define TEMP1_3              32
#define TEMP1_4              33
#define TEMP1_5              34
#define TEMP1_6              35
#define TEMP1_7              36
#define TEMP1_8              37
#define TEMP1_9              38
#define TEMP1_10             39
#define TEMP1_11             40
#define TEMP1_12             41
#define TEMP2_1              42
#define TEMP2_2              43
#define TEMP2_3              44
#define TEMP2_4              45
#define TEMP2_5              46
#define TEMP2_6              47
#define TEMP2_7              48
#define TEMP2_8              49
#define TEMP2_9              50
#define TEMP2_10             51
#define TEMP2_11             52
#define TEMP2_12             53
#define TEMP_AMB             54

#define BYPASS1              55
#define BYPASS2              56

#define FAN1_1               57
#define FAN1_2               58
#define FAN1_3               59
#define FAN1_4               60
#define FAN1_5               61
#define FAN1_6               62
#define FAN1_7               63
#define FAN1_8               64
#define FAN2_1               65
#define FAN2_2               66
#define FAN2_3               67
#define FAN2_4               68
#define FAN2_5               69
#define FAN2_6               70
#define FAN2_7               71
#define FAN2_8               72

#define SWITCH               73

#define ERRORS1_1            74
#define ERRORS1_2            75
#define ERRORS1_3            76

#define WARNINGS1_1          77
#define WARNINGS1_2          78
#define WARNINGS1_3          79

#define ERRORS2_1            80
#define ERRORS2_2            81
#define ERRORS2_3            82

#define WARNINGS2_1          83
#define WARNINGS2_2          84
#define WARNINGS2_3          85

#define EGAS                 86

#define MOTORSPEED1          87
#define MOTORSPEED2          88

#define UPDATE1              89
#define NEWDAT1              90
#define UPDATE2              91
#define NEWDAT2              92
#define UPDATE_C             93
#define NEWDATA_C            94

#define SOC                  95

#endif /* __MAIN_H */
