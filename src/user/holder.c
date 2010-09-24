/**************************************** 
 * Anton Marcuse			*
 * holder.c			        *
 ***************************************/

/* includes ----------------------------------------------------------------- */
#include "holder.h"
#include "display.h"

/* variables ---------------------------------------------------------------- */

// Array beinhaltet alle Werte, die vom CAN empfangen wurden. Initialisierung mit Nullen.
uint16_t tbl_can_values[256]={0};

// Ist keine CAN-Kommunikation vorhanden, kann das Array zu testzwecken mit Einsen initialisiert werden.
/*uint16_t tbl_can_values[256]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                              1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                              1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                              1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                              1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                              1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                              1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                              1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                              1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                              1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                              1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                              1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                              1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};*/


/* functions ---------------------------------------------------------------- */


// ---------------------------- getter / setter für das CAN-Werte-Array

void set_can_value(uint8_t index, uint16_t value){
  tbl_can_values[index] = value;
}

int get_can_value(uint8_t index) {
  return tbl_can_values[index];
}


// ---------------------------- helper-functions für Werte ins Array schreiben

/*
  Die Funktionen werden von new_can_message() benutzt und vereinfachen den Quelltext.
*/

// Setzt vier Zellspannung die mit einer Can-Nachricht empfangen wurden
void set_can_values_cellvoltage(unsigned char index_startvalue, uint16_t *pData_Values) {
  int index;
  int byte_index=0;
  for (index=index_startvalue; index<index_startvalue+4; index++) {
     set_can_value(index, pData_Values[byte_index]);
     display_refresh_value_cellvoltage(index);
     byte_index++;
  }
  // Gesamtspannung und Watt müssen auch aktualisiert werden.
  display_refresh_value_voltage();
  display_refresh_value_power();
}

// Setzt vier Temperaturwerte die mit einer Can-Nachricht empfangen wurden
void set_can_values_temperature(unsigned char index_startvalue, uint16_t *pData_Values) {
  int index;
  int byte_index=0;
  for (index=index_startvalue; index<index_startvalue+4; index++) {
     set_can_value(index, pData_Values[byte_index]);
     display_refresh_value_temperature(index);
     byte_index++;
  }
}

// Setzt vier Lüftergeschwindigkeiten die mit einer Can-Nachricht empfangen wurden
void set_can_values_fanspeed(unsigned char index_startvalue, uint16_t *pData_Values) {
  int index;
  int byte_index=0;
  for (index=index_startvalue; index<index_startvalue+4; index++) {
     set_can_value(index, pData_Values[byte_index]);
     display_refresh_value_fanspeed(index);
     byte_index++;
  }
}

// Setzt drei Errornachrichten die mit einer Can-Nachricht empfangen wurden
void set_can_values_error(unsigned char index_startvalue, uint16_t *pData_Values) {
  int index;
  int byte_index=0;
  for (index=index_startvalue; index<index_startvalue+3; index++) {
     set_can_value(index, pData_Values[byte_index]);
     display_refresh_value_error(index);
     byte_index++;
  }
}

// Setzt drei Warnungsnachrichten die mit einer Can-Nachricht empfangen wurden
void set_can_values_warning(unsigned char index_startvalue, uint16_t *pData_Values) {
  int index;
  int byte_index=0;
  for (index=index_startvalue; index<index_startvalue+3; index++) {
     set_can_value(index, pData_Values[byte_index]);
     display_refresh_value_warning(index);
     byte_index++;
  }
}
 
// ---------------------------- Werte aus CAN-Message ins Array speichern

/*
  Funktion wird vom CAN-Interrupt aufgerufen, speichert die empfangenen Daten
  im Werte-Array und ruft die entsprechenden Display-Refresh-Funktionen auf.
  
  can_id: ID der CAN-Message
  *pData_Bytes: Pointer, der auf das erste Byte der CAN-Daten zeigt.
*/
  
void new_can_message(uint16_t can_id, uint8_t *pData_Bytes){
  
  char i;
  uint16_t pData_Values[4];
  *pData_Values = *pData_Bytes;
  
  // Mach aus dem 8-Byte-Array ein 4-uint16_t-Array
  for (i=0; i<=3; i++) {
    pData_Values[i] = (pData_Bytes[i*2]) + (pData_Bytes[(i*2)+1] <<8);
  }
  
  // Die CAN-ID entscheided über den Inhalt der Datenbytes
  switch(can_id){
    case 0x30:
      // Zellspannung
      set_can_values_cellvoltage(1, pData_Values);
      break;
    case 0x31:
      // Zellspannung
      set_can_values_cellvoltage(5, pData_Values); 
      break;     
    case 0x32:
      // Zellspannung
      set_can_values_cellvoltage(9, pData_Values); 
      break;      
    case 0x40:
      // Zellspannung
      set_can_values_cellvoltage(13, pData_Values); 
      break;
    case 0x41:
      // Zellspannung
      set_can_values_cellvoltage(17, pData_Values); 
      break;
    case 0x42:
      // Zellspannung
      set_can_values_cellvoltage(21, pData_Values);
      break;
    case 0x50:
      // Stromstärke
      set_can_value(25, pData_Values[0]);
      set_can_value(27, pData_Values[1]);
      display_refresh_value_current(25);
      display_refresh_percent_current(25);
      display_refresh_value_currentlow(27);
      display_refresh_value_power();
      break;  
    case 0x60:
      // Stromstärke
      set_can_value(26, pData_Values[0]);
      set_can_value(28, pData_Values[1]);
      display_refresh_value_current(26);
      display_refresh_value_currentlow(28);
      display_refresh_value_power();
      break;
    case 0x70:
      // Stromstärke
      set_can_value(29, pData_Values[0]);
      display_refresh_value_currentcenter(29);
      display_refresh_value_power();
      break;
    case 0x80:
      // Temperatur
      set_can_values_temperature(30, pData_Values); 
      break;
    case 0x81:
      // Temperatur
      set_can_values_temperature(34, pData_Values); 
      break;
    case 0x82:
      // Temperatur
      set_can_values_temperature(38, pData_Values); 
      break;
    case 0x90:
      // Temperatur
      set_can_values_temperature(42, pData_Values); 
      break;
    case 0x91:
      // Temperatur
      set_can_values_temperature(46, pData_Values); 
      break;
    case 0x92:
      // Temperatur
      set_can_values_temperature(50, pData_Values); 
      break;
    case 0x100:
      // Temperatur
      set_can_value(54, pData_Values[0]);
      display_refresh_value_temperature(54); 
      break;
    case 0x110:
      // Bypass
      set_can_value(55, pData_Values[0]);
      display_refresh_value_bypass(55); 
      break;
    case 0x120:
      // Bypass
      set_can_value(56, pData_Values[0]);
      display_refresh_value_bypass(56); 
      break;
    case 0x130:
      // Lüfter
      set_can_values_fanspeed(57, pData_Values); 
      break;
    case 0x135:
      // Lüfter
      set_can_values_fanspeed(61, pData_Values); 
      break;
    case 0x140:
      // Lüfter
      set_can_values_fanspeed(65, pData_Values); 
      break;
    case 0x145:
      // Lüfter
      set_can_values_fanspeed(69, pData_Values); 
      break;
    case 0x0:
      // Schalter
      set_can_value(73, pData_Values[0]);
      display_refresh_value_switch(73); 
      break;      
    case 0x10:
      // Errorcode
      set_can_values_error(74, pData_Values);
      break;      
    case 0x20:
      // Warningcode
      set_can_values_warning(77, pData_Values); 
      break; 
    case 0x11:
      // Errorcode
      set_can_values_error(80, pData_Values); 
      break;       
    case 0x21:
      // Warningcode
      set_can_values_warning(83, pData_Values); 
      break;
    case 0x150:
      // Egas
      set_can_value(86, pData_Values[0]);
      display_refresh_value_egas(86); 
      break;
    case 0x160:
      // Geschwindigkeit
      set_can_value(87, pData_Values[0]);
      display_refresh_value_motorspeed(87);
      break;
    case 0x170:
      // Geschwindigkeit
      set_can_value(88, pData_Values[0]);
      display_refresh_value_speed(88); 
      break;
   }
}


// ---------------------------- helper-functions für die getter-functions

/*
  Diese Funktionen werden von den get_value_...() Funktionen aufgerufen und
  konvertieren zwischen verschiedenen Datentypen.
*/

double int16_to_double(int16_t value){
  return (double)value;
}

uint8_t int16_to_int8(int16_t value){
  return (uint8_t)value;
}

bool int16_to_bool(int16_t value){
  if ( value == 0x00 ) {
    return FALSE;
  } else {
    return TRUE;
  }
}

int8_t get_percent(int16_t iVal, int16_t min, int16_t max) {
  int8_t iPercent;
  iPercent = (iVal - min) * 100 / (max - min);
  if (iPercent > 100) { iPercent = 100; }
  if (iPercent < 0 )  { iPercent = 0; }
  return iPercent;
}


// ---------------------------- getter-functions fürs Display

/*
  Das Display holt sich mit den Funktionen alle Werte für die Darstellung in
  richtigem Format und Einheit.
*/

double get_value_cellvoltage(uint8_t index){
  double dRet = int16_to_double(get_can_value(index));
  dRet /= 1000; // mV => V
  return dRet;
}

uint8_t get_percent_cellvoltage(uint8_t index){
  return get_percent(get_can_value(index), MIN_CELLVOLTAGE, MAX_CELLVOLTAGE);
}

double get_value_current(uint8_t index){
  double dRet = int16_to_double(get_can_value(index));
  dRet /= 100; //dA => A
  return dRet;
}

double get_value_currentlow(uint8_t index){
  double dRet = int16_to_double(get_can_value(index));
  return dRet; //mA
}

double get_value_currentcenter(uint8_t index){
  double dRet = int16_to_double(get_can_value(index));
  dRet /= 10; //dA => A
  return dRet; 
}

double get_value_temperature(uint8_t index){
  double dRet = int16_to_double(get_can_value(index));
  dRet /= 10; //d° => °
  return dRet;
}

uint8_t get_value_bypass(uint8_t index){
  uint8_t iRet = int16_to_int8(get_can_value(index));
  return iRet; // bin
}

uint16_t get_value_fanspeed(uint8_t index){
  return get_can_value(index); // rpm
}

uint8_t get_percent_fanspeed(uint8_t index){
  return get_percent(get_can_value(index), MIN_FANSPEED, MAX_FANSPEED);
}

bool get_value_switch(uint8_t index){
  bool bRet = int16_to_bool(get_can_value(index));
  return bRet; // on / off
}

uint16_t get_value_error(uint8_t index){
  return get_can_value(index); // error id
}

uint16_t get_value_warning(uint8_t index){
  return get_can_value(index); // warning id
}

uint16_t get_value_egas(uint8_t index) {
  return get_can_value(index); // egas
}

uint8_t get_percent_egas(uint8_t index){
  return get_percent(get_can_value(index), MIN_EGAS, MAX_EGAS);
}

uint8_t get_percent_soc(uint8_t index){
  // Kann noch nicht ausgerechnet werden.
  return 90;
}

uint16_t get_value_motorspeed(uint8_t index){
  return get_can_value(index); // motorspeed in km/h
}

// Gesamtspannung
double get_value_voltage(void){
  uint8_t i;
  double sum = 0;
  for (i=1; i<=24; i++) {
    sum += get_value_cellvoltage(i);
  }
  return sum;
}

// Betrag der Stromstärke
uint16_t get_value_current_abs() {
  double current = get_value_current(CURR_1);
  if (current < 0) {
    current *= -1;
  }
  uint16_t iRet = (uint16_t) current;
  return iRet/10;
}

uint8_t get_percent_current(uint8_t index){
  return get_percent(get_value_current_abs(), MIN_CURRENTCENTER, MAX_CURRENTCENTER);
}

uint16_t get_value_power(void){
  // Watt = Gesamtspannung * Stromstärke
  return (uint16_t)(get_value_voltage() * get_value_current_abs());
}