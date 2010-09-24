/**************************************** 
 * Anton Marcuse			*
 * holder.h			        *
 ***************************************/

#ifndef __HOLDER_H
#define __HOLDER_H

/* includes ----------------------------------------------------------------- */
#include "main.h"

/* defines ------------------------------------------------------------------ */
#define   MIN_CELLVOLTAGE     2700    // 2.7V
#define   MAX_CELLVOLTAGE     4100    // 4.1V
#define   MIN_CURRENTCENTER   0
#define   MAX_CURRENTCENTER   150
#define   MIN_FANSPEED    0x0000
#define   MAX_FANSPEED    0xFFFF
#define   MIN_EGAS  0x0000
#define   MAX_EGAS  0xFFFF
#define   MIN_SOC   0x0000
#define   MAX_SOC   0xFFFF

/* variables ---------------------------------------------------------------- */
extern uint16_t tbl_can_values[256];

/* functions ---------------------------------------------------------------- */
void set_can_value(uint8_t index, uint16_t value);
int get_can_value(uint8_t index);
void new_can_message(uint16_t can_id, uint8_t *byte_value);

double int16_to_double(int16_t value);
uint8_t int16_to_int8(int16_t value);
bool int16_to_bool(int16_t value);

double get_value_cellvoltage(uint8_t);
uint8_t get_percent_cellvoltage(uint8_t);
double get_value_current(uint8_t);
double get_value_currentlow(uint8_t);
double get_value_currentcenter(uint8_t);
uint8_t get_percent_current(uint8_t);
double get_value_temperature(uint8_t);
uint8_t get_value_bypass(uint8_t);
uint16_t get_value_fanspeed(uint8_t);
uint8_t get_percent_fanspeed(uint8_t);
bool get_value_switch(uint8_t);
uint16_t get_value_error(uint8_t);
uint16_t get_value_warning(uint8_t);
uint16_t get_value_egas(uint8_t);
uint8_t get_percent_egas(uint8_t);
uint8_t get_percent_soc(uint8_t);
uint16_t get_value_motorspeed(uint8_t);
double get_value_voltage(void);
uint16_t get_value_power(void);

#endif /* __HOLDER_H */
