/****************************************	
* Stefan Wackwitz			*
* display.h				*
 ***************************************/

#ifndef TFT_H_
#define TFT_H_

/* includes ----------------------------------------------------------------- */
#include "main.h"

/* defines ----------------------------------------------------------------- */
#define SpalteEnde       65
#define SpalteEndeStatus 47
#define SpalteEndeZeit   18

#define ROT               3
#define GRUEN             4
#define GELB              7
#define ORANGE           10
#define GRAU             16
#define SCHWARZ           1
#define WEISS             8
#define BLAU              2

/* functions ---------------------------------------------------------------- */
/* Allgemeine Befehle */
void display_on(void);
void display_off(void);

void display_orientation(uint8_t);

void display_brightness_adjust(uint8_t);
void display_brightness_up(void);
void display_brightness_down(void);
void display_brightness_save(void);

void display_light_timer(uint8_t);

void define_color(uint8_t,uint8_t,uint8_t,uint8_t);

void display_show_version(void);
void display_show_project_name(void);
void display_show_interface(void);
void display_show_informations(void);

/* Terminalbefehle */
void display_clear(void);

void display_cursor_on(void);
void display_cursor_off(void);
void display_cursor_position(uint8_t,uint8_t);
void display_cursor_left(void);
void display_cursor_down(void);
void display_cursor_save(void);
void display_cursor_restore(void);

void display_define_window(uint8_t,uint8_t,uint8_t,uint8_t,uint8_t);

void display_terminal_printf(char*);

void display_terminal_ff(void);
void display_terminal_cr(void);
void display_terminal_lf(void);
void display_terminal_gradc(void);

/* Grafikfunktionen */
void display_set_color(uint8_t,uint8_t);

void display_set_color_straight_line(uint8_t,uint8_t);

void display_draw_rectangle(uint16_t,uint16_t,uint16_t,uint16_t);
void display_draw_line(uint16_t,uint16_t,uint16_t,uint16_t);
void display_draw_point(uint16_t,uint16_t);
void display_set_point_size(uint8_t,uint8_t);
void display_set_font(uint8_t);

void display_fill_area(uint16_t,uint16_t,uint16_t,uint16_t);
void display_clear_area(uint16_t,uint16_t,uint16_t,uint16_t);

void display_printf(uint8_t,uint8_t,uint8_t,char*);
void display_printf_coordinates(uint16_t,uint16_t,uint8_t,char*);

void display_bargraph_colors(uint8_t,uint8_t,uint8_t);
void display_bargraph_send_refresh(uint8_t,uint8_t);
void display_bargraph_design(uint8_t);
void display_bargraph_define(uint8_t,uint8_t,uint16_t,uint16_t,uint16_t,uint16_t,uint8_t,uint8_t,uint8_t);
void display_bargraph_text(uint8_t,uint16_t,uint16_t,char*);

/* eigene Funktionen */

void display_send_data(uint8_t *,uint8_t);

void display_show_menu0(void); 
void display_show_menu1(void);

void display_refresh_value_cellvoltage(uint8_t);
void display_refresh_percent_cellvoltage(uint8_t);
void display_refresh_value_current(uint8_t);
void display_refresh_value_currentlow(uint8_t);
void display_refresh_value_currentcenter(uint8_t);
void display_refresh_percent_current(uint8_t);
void display_refresh_value_temperature(uint8_t);
void display_refresh_value_bypass(uint8_t);
void display_refresh_value_fanspeed(uint8_t);
void display_refresh_percent_fanspeed(uint8_t);
void display_refresh_value_switch(uint8_t);
void display_refresh_value_error(uint8_t);
void display_refresh_value_warning(uint8_t);
void display_refresh_value_egas(uint8_t);
void display_refresh_percent_egas(uint8_t);
void display_refresh_percent_soc(uint8_t);
void display_refresh_value_motorspeed(uint8_t);
void display_refresh_value_egas(uint8_t);
void display_refresh_value_speed(uint8_t);
void display_refresh_value_warning(uint8_t);
void display_refresh_value_error(uint8_t);
void display_refresh_value_fanspeed(uint8_t);
void display_refresh_value_voltage(void);
void display_refresh_value_power(void);

#endif /* TFT_H_ */
