/****************************************	
* Stefan Wackwitz			*
* display.c				*
 ***************************************/

/* includes ----------------------------------------------------------------- */
#include "display.h"

/* variables ---------------------------------------------------------------- */
uint8_t oldspeed = 0;
double voltage_old = 0;
double current_old = 0;
double power_old = 0;
NVIC_InitTypeDef NVIC_InitStructure;

/* functions ---------------------------------------------------------------- */
void display_on(void){ // Terminal einschalten
  uint8_t user_data[3] = {0x1B,0x54,0x45};
  display_send_data(user_data,3);
}
void display_off(void){ // Terminal ausschalten
  uint8_t user_data[3] = {0x1B,0x54,0x41};
  display_send_data(user_data,3);
}

void display_clear(void){ // Bildschirm löschen (Hintergrundfarbe anzeigen) und Cursor nach Pos. 1,1 setzen
  uint8_t user_data[1] = {0x0C};
  display_send_data(user_data,1);
}

void display_cursor_on(void){ // Cursor blinkt
  uint8_t user_data[4] = {0x1B,0x54,0x43,0x31};
  display_send_data(user_data,4);
}
void display_cursor_off(void){ // Cursor ist unsichtbar
  uint8_t user_data[4] = {0x1B,0x54,0x43,0x30};
  display_send_data(user_data,4);
}
void display_cursor_position(uint8_t s,uint8_t z){ // s = Spalte ; z = Zeile ; Ursprung ist links oben (1,1)
  uint8_t user_data[5] = {0x1B,0x54,0x50,s,z};
  display_send_data(user_data,5);  
}
void display_cursor_left(void){ // Cursor ganz nach links zum Zeilenanfang
  uint8_t user_data[1] = {0x0D};
  display_send_data(user_data,1);
}
void display_cursor_down(void){ // Cursor 1 Zeile tiefer, falls Cursor in letzter Zeile dann wird gescrollt
  uint8_t user_data[1] = {0x0A};
  display_send_data(user_data,1);
}
void display_cursor_save(void){ // die aktuelle Cursorposition wird gesichert
  uint8_t user_data[3] = {0x1B,0x54,0x53};
  display_send_data(user_data,3);
}
void display_cursor_restore(void){ // die letzte gesicherte Cursorposition wird wieder hergestellt
  uint8_t user_data[3] = {0x1B,0x54,0x52};
  display_send_data(user_data,3);
}

void display_define_window(uint8_t n1, uint8_t s,uint8_t z,uint8_t b ,uint8_t h){ // Terminal Fenster definieren
  if(n1 == 1){ // Font 8x8
    uint8_t user_data[8] = {0x1B,0x54,0x57,0x31,s,z,b,h};
    display_send_data(user_data,8);	
  }
  else if(n1 == 2){ // Font 8x16 (Standard nach Reset)
    uint8_t user_data[8] = {0x1B,0x54,0x57,0x32,s,z,b,h};
    display_send_data(user_data,8);
  }
}

void display_show_version(void){ // Die Version wird im Terminal ausgegeben z.B. "EA eDIPTFT43-A V1.0 Rev.A"
  uint8_t user_data[3] = {0x1B,0x54,0x56};
  display_send_data(user_data,3);
}
void display_show_project_name(void){ // Der Makro-Projektname wird im Terminal ausgegeben z.B. "init / delivery state"
  uint8_t user_data[3] = {0x1B,0x54,0x4A};
  display_send_data(user_data,3);
}
void display_show_interface(void){
  /*
   * Die eingestellte Schnittstelle wird im Terminal ausgegeben z.B. RS232, 115200 baud, ADR $07"
   */
  uint8_t user_data[3] = {0x1B,0x54,0x51};
  display_send_data(user_data,3);
}
void display_show_informations(void){
  /*
   * Das Terminal wird initialisiert und gelöscht, die Software Version, Hardware Revision, der
   * Makro-Projektname und die CRC-Checksummen werden im Terminal ausgegeben.
   */
  uint8_t user_data[3] = {0x1B,0x54,0x49};
  display_send_data(user_data,3);
}

/* Grafikfunktionen */

void display_set_color(uint8_t vf,uint8_t hf){
  /*
   * Farbe für den Terminal Betrieb einstellen. vf = Schiftfarbe ; hf = Hintergrundfarbe
   */
  uint8_t user_data[5] = {0x1B,0x46,0x5A,vf,hf};
  display_send_data(user_data,5);	
}

void display_orientation(uint8_t n1){
  /*
   * n1 = 0: 0° ; n1 = 1: 90° ; n1 = 2: 180° ; n1 = 2: 270°
   * (0° & 180° = 480x272 ; 90° & 270° = 272x480)
   */
  uint8_t user_data[4] = {0x1B,0x44,0x4F,(n1+0x30)};
  display_send_data(user_data,4);
}

void display_set_color_straight_line(uint8_t vf,uint8_t hf){ // Farbe der Linien definieren
  uint8_t user_data[5] = {0x1B,0x46,0x47,vf,hf};  
  display_send_data(user_data,5); 
}
void display_draw_rectangle(uint16_t xx1,uint16_t yy1,uint16_t xx2,uint16_t yy2){ // Rechteck spannen
  /* Vier Geraden als Rechteck von xx1,yy1 bis xx2,yy2 zeichnen */
  uint8_t user_data[11] = {0x1B,0x47,0x52,xx1&0xFF,(xx1&0xFF00)>>8,yy1&0xFF,(yy1&0xFF00)>>8,xx2&0xFF,(xx2&0xFF00)>>8,yy2&0xFF,(yy2&0xFF00)>>8};
  display_send_data(user_data,11);    
}

void display_draw_line(uint16_t xx1,uint16_t yy1,uint16_t xx2,uint16_t yy2){ // Linie zeichnen
  uint8_t user_data[11] = {0x1B,0x47,0x44,xx1&0xFF,(xx1&0xFF00)>>8,yy1&0xFF,(yy1&0xFF00)>>8,xx2&0xFF,(xx2&0xFF00)>>8,yy2&0xFF,(yy2&0xFF00)>>8};
  display_send_data(user_data,11);
}

void display_fill_area(uint16_t xx1,uint16_t yy1,uint16_t xx2,uint16_t yy2){ // Bereich mit Farbe füllen
  uint8_t user_data[11] = {0x1B,0x52,0x53,xx1&0xFF,(xx1&0xFF00)>>8,yy1&0xFF,(yy1&0xFF00)>>8,xx2&0xFF,(xx2&0xFF00)>>8,yy2&0xFF,(yy2&0xFF00)>>8};
  display_send_data(user_data,11); 
}

void display_clear_area(uint16_t xx1,uint16_t yy1,uint16_t xx2,uint16_t yy2){ // Bereich löschen (mit Hintergrundfarbe füllen)
  uint8_t user_data[11] = {0x1B,0x52,0x4C,xx1&0xFF,(xx1&0xFF00)>>8,yy1&0xFF,(yy1&0xFF00)>>8,xx2&0xFF,(xx2&0xFF00)>>8,yy2&0xFF,(yy2&0xFF00)>>8};
  display_send_data(user_data,11);  
}

void display_printf(uint8_t spalte,uint8_t zeile,uint8_t mask,char* text){ // String an vorher definierten Zeilen und Spalten ausgeben
  uint16_t xx1 = 0;
  uint16_t yy1 = 0;
  uint16_t length = strlen(text);
  uint8_t* user_data;  
  
  if(zeile < 20){
    if(length > SpalteEnde){
      length = SpalteEnde;
    }
    switch(mask & 0x09){ // Farbe anhand der Maske festlegen
    case 0x01:
      display_set_color(GELB,0); 
      break;
    case 0x08:
      display_set_color(GRAU,0); 
      break;
    case 0x09:
      display_set_color(ROT,0); 
      break;
    default:
      display_set_color(GRUEN,0); 
      break;
  }
  }
  else{
    if(length > SpalteEndeStatus){
      if(spalte == 1){
        length = SpalteEndeZeit;
      }
      else{
        length = SpalteEndeStatus;
      }
    }
    display_set_color(mask,0); // Farbe anhand des Übergabewertes (Farbe) festlegen
  } 

  switch(spalte){
      case 0: 
        xx1 = 5;
        break;
      case 1:   
        xx1 = 343; // 343 Uhrzeit + Datum Anfang
        break;
  }  
  switch(zeile){
      case 0: 
        yy1 = 5;
        break;
      case 1: 
        yy1 = 17;
        break;
      case 2: 
        yy1 = 29;
        break;
      case 3: 
        yy1 = 41;
        break;
      case 4: 
        yy1 = 53;
        break;
      case 5: 
        yy1 = 65;
        break;
      case 6: 
        yy1 = 77;
        break;
      case 7: 
        yy1 = 89;
        break;
      case 8: 
        yy1 = 101;
        break;
      case 9: 
        yy1 = 113;
        break;
      case 10: 
        yy1 = 125;
        break;
      case 11: 
        yy1 = 137;
        break; 
      case 12: 
        yy1 = 149;
        break;
      case 13: 
        yy1 = 161;
        break;
      case 14: 
        yy1 = 173;
        break;
      case 15: 
        yy1 = 185;
        break;
      case 16: 
        yy1 = 197;
        break;
      case 17: 
        yy1 = 209;
        break;
      case 18: 
        yy1 = 221;
        break;
      case 19: 
        yy1 = 233;
        break;
      case 20: 
        yy1 = 254;
        break;  
  }
  
  user_data = (uint8_t*) malloc((length + 8) * sizeof(uint8_t)); // Speicher reservieren
    
  user_data[0] = 0x1B;
  user_data[1] = 0x5A;
  user_data[2] = 0x4C;
  user_data[3] = xx1&0xFF;
  user_data[4] = (xx1&0xFF00)>>8;
  user_data[5] = yy1&0xFF;
  user_data[6] = (yy1&0xFF00)>>8;
  
  for(int i = 7 ; i < (length + 7) ; i++){
    user_data[i] = text[i - 7];
  }
  
  user_data[length + 7] = 0x00;
  
  display_send_data(user_data,length + 8);
  
  free(user_data);
}

void display_printf_coordinates(uint16_t xx1,uint16_t yy1,uint8_t color,char* text){ // String an x,y Koordinaten ausgeben
  uint16_t length = strlen(text);
  uint8_t* user_data;   
  
  display_set_color(color,0); // Farbe anhand des Übergabewertes (Farbe) festlegen
   
  user_data = (uint8_t*) malloc((length + 8) * sizeof(uint8_t)); // Speicher reservieren
  
  user_data[0] = 0x1B;
  user_data[1] = 0x5A;
  user_data[2] = 0x4C;
  user_data[3] = xx1&0xFF;
  user_data[4] = (xx1&0xFF00)>>8;
  user_data[5] = yy1&0xFF;
  user_data[6] = (yy1&0xFF00)>>8;
  
  for(int i = 7 ; i < (length + 7) ; i++){
    user_data[i] = text[i - 7];
  }
  
  user_data[length + 7] = 0x00;
  
  display_send_data(user_data,length + 8);
  
  free(user_data);
}

void display_bargraph_define(uint8_t direction,uint8_t n1,uint16_t xx1,uint16_t yy1,uint16_t xx2,uint16_t yy2,uint8_t aw,uint8_t ew,uint8_t typ){ // Bargraph definieren
  uint8_t user_data[15] = {0x1B,0x42,direction,n1,xx1&0xFF,(xx1&0xFF00)>>8,yy1&0xFF,(yy1&0xFF00)>>8,xx2&0xFF,(xx2&0xFF00)>>8,yy2&0xFF,(yy2&0xFF00)>>8,aw,ew,typ};
  display_send_data(user_data,15);
}

void display_bargraph_design(uint8_t n1){ // Bargraph Design festlegen
  uint8_t user_data[4] = {0x1B,0x42,0x4D,n1};
  display_send_data(user_data,4);  
}

void display_bargraph_colors(uint8_t vf,uint8_t hf,uint8_t rf){ // Bargraph Farben festlegen
  uint8_t user_data[6] = {0x1B,0x46,0x42,vf,hf,rf};
  display_send_data(user_data,6);  
}

void display_bargraph_send_refresh(uint8_t n1,uint8_t value){ // Bargraph Wert aktualisieren
  uint8_t user_data[5] = {0x1B,0x42,0x41,n1,value};
  display_send_data(user_data,5);
}

void display_bargraph_text(uint8_t n1,uint16_t xx1,uint16_t yy1,char* text){ // Bargraph Wert als Text darstellen
  uint16_t length = strlen(text);
  uint8_t* user_data;  
  
  user_data = (uint8_t*) malloc((length + 9) * sizeof(uint8_t)); // Speicher reservieren
    
  user_data[0] = 0x1B;
  user_data[1] = 0x42;
  user_data[2] = 0x58;
  user_data[3] = n1;
  user_data[4] = xx1&0xFF;
  user_data[5] = (xx1&0xFF00)>>8;
  user_data[6] = yy1&0xFF;
  user_data[7] = (yy1&0xFF00)>>8;
  
  for(int i = 8 ; i < (length + 8) ; i++){
    user_data[i] = text[i - 8];
  }
  
  user_data[length + 8] = 0x00;
  
  display_send_data(user_data,length + 9);
}

void display_terminal_printf(char* text){ // String auf Terminal (nicht grafisch) ausgeben
  uint16_t length = strlen(text);
  uint8_t* user_data;
  user_data = (uint8_t*) malloc(length * sizeof(uint8_t)); // Speicher reservieren
  for(int i = 0 ; i < length ; i++){
    user_data[i] = text[i];
  }    
  display_send_data(user_data,length);
  
  free(user_data);
}

void display_terminal_ff(void){
  uint8_t user_data[1];
  user_data[0] = 0x0C;
  display_send_data(user_data,1);  
}

void display_terminal_cr(void){
  uint8_t user_data[1];
  user_data[0] = 0x0D;
  display_send_data(user_data,1);  
}

void display_terminal_lf(void){
  uint8_t user_data[1];
  user_data[0] = 0x0A;
  display_send_data(user_data,1);  
}

void display_terminal_gradc(void){
  uint8_t user_data[3];
  user_data[0] = 0x23;
  user_data[1] = 0xF0;
  user_data[2] = 0x08;
  display_send_data(user_data,3);
}

void display_draw_point(uint16_t xx1,uint16_t yy1){ // Punkt an Koordinaten zeichen
  uint8_t user_data[7] = {0x1B,0x47,0x50,xx1&0xFF,(xx1&0xFF00)>>8,yy1&0xFF,(yy1&0xFF00)>>8};
  display_send_data(user_data,7);
}

void display_set_point_size(uint8_t n1,uint8_t n2){ // Punktgröße festlegen
  uint8_t user_data[5] = {0x1B,0x47,0x5A,n1,n2};
  display_send_data(user_data,5);  
}

void display_set_font(uint8_t n1){ // Font einstellen
  uint8_t user_data[4] = {0x1B,0x5A,0x46,n1};
  display_send_data(user_data,4);    
}

/* Allgemeine Befehle */

void display_brightness_adjust(uint8_t n1){ // Helligkeit der LED-Beleuchtung auf n1 = 0..100% einstellen
  uint8_t user_data[4] = {0x1B,0x59,0x48,n1};
  display_send_data(user_data,4);		
}
void display_brightness_up(void){ // Helligkeit der LED-Belechtung um einen Schritt erhöhen
  uint8_t user_data[3] = {0x1B,0x59,0x4E};
  display_send_data(user_data,3);
}
void display_brightness_down(void){ // Helligkeit der LED-Belechtung um einen Schritt verringern
  uint8_t user_data[3] = {0x1B,0x59,0x50};
  display_send_data(user_data,3);
}
void display_brightness_save(void){ // Die aktuelle LED-Helligkeit und Änderungszeit als Startwert im EEPROM speichern
  uint8_t user_data[3] = {0x1B,0x59,0x40};
  display_send_data(user_data,3);
}

void display_light_timer(uint8_t n1){
  /*
   * Beleuchtung n1 = 0: Auint8_t ; n1 = 1: EIN ; n1 = 2..255: für n1 / 10s lang einschalten
   */
  uint8_t user_data[4] = {0x1B,0x59,0x4C,n1};
  display_send_data(user_data,4);
}

void define_color(uint8_t n1,uint8_t r5,uint8_t g6 ,uint8_t b5){
  uint8_t user_data[7] = {0x1B,0x46,0x50,n1,r5,g6,b5};
  display_send_data(user_data,7);  
}

/* eigene Funktionen */

void display_send_data(uint8_t* user_data, uint8_t len){ // Befehle an TFT senden, über I2C
  uint8_t* data;
  uint8_t bcc;
  data = (uint8_t*) malloc(len * sizeof(uint8_t)); // Speicher reservieren
  if(data != NULL){
    bcc = (0x11 + len);
    data[0] = 0x11; // Rahmen
    data[1] = len; // Anzahl der Nutzdaten
    for(uint8_t i = 2 ; i < len + 2 ; i++){ // Nutzdaten in Protokoll integrieren
      data[i] = *(user_data + i - 2);
      bcc += data[i];
    }
    bcc &= 0xFF; // Division durch 256
    data[len + 2] = bcc; // Prüfsumme anhängen
    
    /*
     * Ausgabe des Daten - Arrays
     */   
    
    /***********************************************************************************/
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn; // CAN1 Interrupt aktivieren
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // Priorität 0 ==> höchste Priorität
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; // Timer 2 Interrupt aktivieren
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // Priorität 1
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;  
    NVIC_Init(&NVIC_InitStructure);  
    
    do{
      I2C_AcknowledgeConfig(I2C1, ENABLE);
      I2C1->CR1 |= 0x0100; // START
      while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));  /* EV5 */
      I2C1->DR = 0x10; // ADRESSE + WRITE
      while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); /* EV6 */                    
      for(uint8_t i = 0 ; i <= (len + 2) ; i++){           
        I2C1->DR = data[i]; // DATA Send
        //for(uint16_t i = 0 ; i < 7500 ; i++); // 100 µs
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)); /* EV8 */            
      }
      I2C1->CR1 |= 0x0200; // STOP 
      
      for(uint16_t i = 0 ; i < 550 ; i++); // 6 µs
      
      I2C1->CR1 |= 0x0100; // START
      while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));  /* EV5 */
      I2C1->DR = (0x11); // ADRESSE + READ
      while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));  /* EV6 */
      I2C_AcknowledgeConfig(I2C1, DISABLE);
      I2C1->CR1 |= 0x0200; // STOP
      while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));  /* EV7 */    
    }
    while(I2C1->DR != 0x06); // solange bis Übertragung erfolgreich
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn; // CAN1 Interrupt aktivieren
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // Priorität 0 ==> höchste Priorität
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; // Timer 2 Interrupt aktivieren
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // Priorität 1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);  
    
    /***********************************************************************************/
    
    free(data); // Speicher freigeben 
  }
}

void display_show_menu0(void){
  display_clear_area(0,0,479,271); // Display schwarz zeichnen
  display_clear(); // alle Terminalausgaben löschen
  display_define_window(2,44,7,9,8); // Terminalfenster definieren
  display_cursor_off(); // Cursor deaktivieren
  
  display_brightness_adjust(100); // Helligkeit der Displaybeleuchtung auf 50% stellen
  display_set_point_size(2,2); // Punktgröße
  
  define_color(17,0xff,0x33,0x33);
  define_color(18,0x33,0xff,0x33);
  define_color(19,0x33,0xff,0x33);
  define_color(20,0x00,0xff,0x00);
  
  // Rahmen
  display_set_color_straight_line(ROT,1); // Linienfarbe Rot
  display_draw_rectangle(0,0,478,270);
  display_draw_rectangle(0,0,478,43);
  
  // Menüpunktname
  display_set_font(6);  
  display_printf_coordinates(10,10,ROT,"TACHO");
    
  // Einheiten etc. darstellen
  display_set_font(6);
  display_printf_coordinates(177,165,ROT," km/h");
  
  // Current BOOST % - Bargraph
  display_bargraph_design(0);
  display_bargraph_colors(ROT,1,8);
  display_bargraph_define(0x52,13,17,210,250,225,0,100,1); // Current %  
  
  display_set_color_straight_line(WEISS,1); 
  
  // Batterie + Pol zeichnen
  display_draw_line(300,86,320,86);
  display_draw_line(300,88,320,88);  
  
  // Motorsymbol zeichnen 
  display_set_font(6);
  display_printf_coordinates(422,180,WEISS,"M"); 
  
  // Relaisknubbel zeichnen
  display_draw_line(428,105,433,105); // Knubbel oben
  display_draw_line(428,107,433,107); // Knubbel oben
  display_draw_line(428,156,433,156); // Knubbel unten
  display_draw_line(428,158,433,158); // Knubbel unten        
  display_draw_line(448,156,453,156); // Knubbel unten rechts
  display_draw_line(448,158,453,158); // Knubbel unten rechts  
  
  // Werte darstellen
   
  display_refresh_value_motorspeed(MOTORSPEED1);
  
  display_refresh_value_current(CURR_1);
  
  display_refresh_percent_current(CURR_1);
  
  display_refresh_percent_soc(SOC);
 
  display_refresh_value_switch(SWITCH); 
  
  display_refresh_value_voltage();
  
  display_refresh_value_power();  
}

void display_show_menu1(void){  
  display_clear_area(0,0,479,271);
  display_clear();
  display_define_window(1,1,1,255,255);
  display_cursor_off();
  display_brightness_adjust(100);  
  display_set_point_size(2,2);
  
  // Rahmen
  display_set_color_straight_line(ROT,1);
  display_draw_rectangle(0,0,478,270);  
  display_draw_rectangle(0,0,478,43);
  
  display_draw_line(239,43,239,271);
  
  // Menüpunktname
  display_set_font(6);
  display_printf_coordinates(10,10,ROT,"CELLS");   
  
  // Zellbezeichnungen
  
  display_cursor_position(2,9);
  display_terminal_printf("C1_1");
  display_cursor_position(2,13);
  display_terminal_printf("C1_2");
  display_cursor_position(2,17);
  display_terminal_printf("C1_3");
  display_cursor_position(2,21);
  display_terminal_printf("C1_4");
  display_cursor_position(2,25);
  display_terminal_printf("C1_5");
  display_cursor_position(2,29);
  display_terminal_printf("C1_6");
  
  display_cursor_position(32,9);
  display_terminal_printf("C2_1");
  display_cursor_position(32,13);
  display_terminal_printf("C2_2");
  display_cursor_position(32,17);
  display_terminal_printf("C2_3");
  display_cursor_position(32,21);
  display_terminal_printf("C2_4");
  display_cursor_position(32,25);
  display_terminal_printf("C2_5");
  display_cursor_position(32,29);
  display_terminal_printf("C2_6");
  
  display_refresh_value_cellvoltage(VOLT1_1);
  display_refresh_value_cellvoltage(VOLT1_2);
  display_refresh_value_cellvoltage(VOLT1_3);
  display_refresh_value_cellvoltage(VOLT1_4);
  display_refresh_value_cellvoltage(VOLT1_5);
  display_refresh_value_cellvoltage(VOLT1_6);
  
  display_refresh_value_cellvoltage(VOLT2_1);
  display_refresh_value_cellvoltage(VOLT2_2);
  display_refresh_value_cellvoltage(VOLT2_3);
  display_refresh_value_cellvoltage(VOLT2_4);
  display_refresh_value_cellvoltage(VOLT2_5);
  display_refresh_value_cellvoltage(VOLT2_6);
  
  display_refresh_percent_cellvoltage(VOLT1_1);
  display_refresh_percent_cellvoltage(VOLT1_2);
  display_refresh_percent_cellvoltage(VOLT1_3);
  display_refresh_percent_cellvoltage(VOLT1_4);
  display_refresh_percent_cellvoltage(VOLT1_5);
  display_refresh_percent_cellvoltage(VOLT1_6);
  
  display_refresh_percent_cellvoltage(VOLT2_1);
  display_refresh_percent_cellvoltage(VOLT2_2);
  display_refresh_percent_cellvoltage(VOLT2_3);
  display_refresh_percent_cellvoltage(VOLT2_4);
  display_refresh_percent_cellvoltage(VOLT2_5);
  display_refresh_percent_cellvoltage(VOLT2_6);

  display_refresh_value_temperature(TEMP1_1);
  display_refresh_value_temperature(TEMP1_2);
  display_refresh_value_temperature(TEMP1_3);
  display_refresh_value_temperature(TEMP1_4);
  display_refresh_value_temperature(TEMP1_5);
  display_refresh_value_temperature(TEMP1_6);
  
  display_refresh_value_temperature(TEMP2_1);
  display_refresh_value_temperature(TEMP2_2);
  display_refresh_value_temperature(TEMP2_3);
  display_refresh_value_temperature(TEMP2_4);
  display_refresh_value_temperature(TEMP2_5);
  display_refresh_value_temperature(TEMP2_6);
  
  display_refresh_value_bypass(BYPASS1);
  display_refresh_value_bypass(BYPASS2);
}

void display_refresh_value_cellvoltage(uint8_t index){
  char buffer[10];
  if(menu_item == 1){
    double cellvoltage = get_value_cellvoltage(index);
    sprintf(buffer,"%.2f  V", cellvoltage);
    switch(index){
      case VOLT1_1:     
        display_cursor_position(8,9);
        display_terminal_printf(buffer);      
        break;
      case VOLT1_2:
        display_cursor_position(8,13);
        display_terminal_printf(buffer); 
        break;
      case VOLT1_3:
        display_cursor_position(8,17);
        display_terminal_printf(buffer); 
        break;
      case VOLT1_4:
        display_cursor_position(8,21); 
        display_terminal_printf(buffer);
        break; 
      case VOLT1_5:
        display_cursor_position(8,25);
        display_terminal_printf(buffer);
        break;
      case VOLT1_6:
        display_cursor_position(8,29);
        display_terminal_printf(buffer);
        break;      
      case VOLT2_1:
        display_cursor_position(38,9);
        display_terminal_printf(buffer);
        break;
      case VOLT2_2:
        display_cursor_position(38,13);
        display_terminal_printf(buffer);
        break;
      case VOLT2_3:
        display_cursor_position(38,17);
        display_terminal_printf(buffer);
        break;
      case VOLT2_4:
        display_cursor_position(38,21);
        display_terminal_printf(buffer);
        break; 
      case VOLT2_5:
        display_cursor_position(38,25);
        display_terminal_printf(buffer);
        break;
      case VOLT2_6:
        display_cursor_position(38,29);
        display_terminal_printf(buffer);
        break;      
    }  
  }  
}  
  
void display_refresh_percent_cellvoltage(uint8_t index){
  if(menu_item == 1){
    uint8_t cellvoltage = get_percent_cellvoltage(index); 
    
    if(cellvoltage <= 14){ // ff3333
      display_bargraph_design(12);
      display_bargraph_colors(17,1,8);
    }
    else if((cellvoltage > 14) && (cellvoltage <= 36)){ // ffff00
      display_bargraph_design(12);
      display_bargraph_colors(18,1,8);
    }
    else if((cellvoltage > 36) && (cellvoltage <= 96)){ // 33ff33
      display_bargraph_design(12);
      display_bargraph_colors(19,1,8);
    }
    else{ // 00ff00
      display_bargraph_design(12);
      display_bargraph_colors(20,1,8);
    }    
    switch(index){
      case VOLT1_1:
        display_bargraph_define(0x52,1,131,63,231,78,0,100,1); 
        display_bargraph_send_refresh(1,cellvoltage);        
        break;
      case VOLT1_2:
        display_bargraph_define(0x52,2,131,95,231,110,0,100,1);
        display_bargraph_send_refresh(2,cellvoltage);  
        break;
      case VOLT1_3:
        display_bargraph_define(0x52,3,131,127,231,142,0,100,1);
        display_bargraph_send_refresh(3,cellvoltage);  
        break;
      case VOLT1_4:
        display_bargraph_define(0x52,4,131,159,231,174,0,100,1);
        display_bargraph_send_refresh(4,cellvoltage);  
        break; 
      case VOLT1_5:
        display_bargraph_define(0x52,5,131,191,231,206,0,100,1);
        display_bargraph_send_refresh(5,cellvoltage);  
        break;
      case VOLT1_6:
        display_bargraph_define(0x52,6,131,223,231,238,0,100,1);
        display_bargraph_send_refresh(6,cellvoltage);  
        break;      
      case VOLT2_1:
        display_bargraph_define(0x52,7,370,63,470,78,0,100,1);
        display_bargraph_send_refresh(7,cellvoltage);
        break;
      case VOLT2_2:
        display_bargraph_define(0x52,8,370,95,470,110,0,100,1);
        display_bargraph_send_refresh(8,cellvoltage);
        break;
      case VOLT2_3:
        display_bargraph_define(0x52,9,370,127,470,142,0,100,1);
        display_bargraph_send_refresh(9,cellvoltage);  
        break;
      case VOLT2_4:
        display_bargraph_define(0x52,10,370,159,470,174,0,100,1);
        display_bargraph_send_refresh(10,cellvoltage);  
        break; 
      case VOLT2_5:
        display_bargraph_define(0x52,11,370,191,470,206,0,100,1);
        display_bargraph_send_refresh(11,cellvoltage);  
        break;
      case VOLT2_6:
        display_bargraph_define(0x52,12,370,223,470,238,0,100,1); 
        display_bargraph_send_refresh(12,cellvoltage);
        break;
    }  
  }  
}

void display_refresh_value_temperature(uint8_t index){
  char buffer[10];
  if(menu_item == 1){
    double temperature = get_value_temperature(index); 
    sprintf(buffer,"%.2f GC", temperature);
    switch(index){
      case TEMP1_1:
        display_cursor_position(8,10);
        display_terminal_printf(buffer);        
        break;
      case TEMP1_2:
        display_cursor_position(8,14); 
        display_terminal_printf(buffer);
        break;
      case TEMP1_3:
        display_cursor_position(8,18); 
        display_terminal_printf(buffer);
        break;
      case TEMP1_4:
        display_cursor_position(8,22); 
        display_terminal_printf(buffer);
        break; 
      case TEMP1_5:
        display_cursor_position(8,26);
        display_terminal_printf(buffer);
        break;
      case TEMP1_6:
        display_cursor_position(8,30); 
        display_terminal_printf(buffer);
        break;      
      case TEMP2_1:
        display_cursor_position(38,10); 
        display_terminal_printf(buffer);
        break;
      case TEMP2_2:
        display_cursor_position(38,14);
        display_terminal_printf(buffer);
        break;
      case TEMP2_3:
        display_cursor_position(38,18);
        display_terminal_printf(buffer);
        break;
      case TEMP2_4:
        display_cursor_position(38,22);
        display_terminal_printf(buffer);
        break; 
      case TEMP2_5:
        display_cursor_position(38,26); 
        display_terminal_printf(buffer);
        break;
      case TEMP2_6:
        display_cursor_position(38,30); 
        display_terminal_printf(buffer);
        break;      
    }  
  }  
}

void display_refresh_value_bypass(uint8_t index){
  if(menu_item == 1){
    uint8_t bypass = get_value_bypass(index);
    switch(index){
      case BYPASS1:      
        if((bypass & 0x01) == 0x01)
          display_fill_area(8,74,40,78);
        else
          display_clear_area(8,74,40,78);
        if(((bypass >> 1) & 0x01) == 0x01)
          display_fill_area(8,104,40,110);
        else
          display_clear_area(8,104,40,110);
        if(((bypass >> 2) & 0x01) == 0x01)
          display_fill_area(8,138,40,142);
        else
          display_clear_area(8,138,40,142);
        if(((bypass >> 3) & 0x01) == 0x01)
          display_fill_area(8,170,40,174);
        else
          display_clear_area(8,170,40,174);        
        if(((bypass >> 4) & 0x01) == 0x01)
          display_fill_area(8,202,40,206);
        else
          display_clear_area(8,202,40,206);        
        if(((bypass >> 5) & 0x01) == 0x01)
          display_fill_area(8,234,40,238);   
        else
          display_clear_area(8,234,40,238);
        break;
      case BYPASS2: 
        if((bypass & 0x01) == 0x01)
          display_fill_area(248,74,280,78);
        else
          display_clear_area(248,74,280,78);
        if(((bypass >> 1) & 0x01) == 0x01)
          display_fill_area(248,104,280,110);
        else
          display_clear_area(248,104,280,110);
        if(((bypass >> 2) & 0x01) == 0x01)
          display_fill_area(248,138,280,142);
        else
          display_clear_area(248,138,280,142);
        if(((bypass >> 3) & 0x01) == 0x01)
          display_fill_area(248,170,280,174);
        else
          display_clear_area(248,170,280,174);
        if(((bypass >> 4) & 0x01) == 0x01)
          display_fill_area(248,202,280,206);
        else
          display_clear_area(248,202,280,206);
        if(((bypass >> 5) & 0x01) == 0x01)
          display_fill_area(248,234,280,238);
        else
          display_clear_area(248,234,280,238);
        break;
    }   
  }
}

void display_refresh_value_motorspeed(uint8_t index){
  char buffer_speed[10];
  if(menu_item == 0){
    uint8_t speed = get_value_motorspeed(index);
    uint8_t speed_10er = (speed / 10) - (speed % 10);
    uint8_t speed_1er = (speed % 10);
    uint8_t oldspeed_10er = (oldspeed / 10) - (oldspeed % 10);
    uint8_t oldspeed_1er = (oldspeed % 10);
    switch(index){
      case MOTORSPEED1:
        if(speed_10er != oldspeed_10er){
          display_clear_area(17,85,95,195); // 10er Stelle löschen
        }
        if(speed_1er != oldspeed_1er){  
          display_clear_area(95,85,185,195); // 1er Stelle löschen
        }
        display_set_font(8);
        sprintf(buffer_speed,"%.2u", speed);
        if(speed <= 9){
          buffer_speed[0] = ' ';
          display_printf_coordinates(97,90,ROT,buffer_speed);
        }
        else{
          display_printf_coordinates(17,90,ROT,buffer_speed);
        }
          
        oldspeed = speed;
        break;
      case MOTORSPEED2:
        ;
        break; 
    }
  }  
}

void display_refresh_value_current(uint8_t index){
  char buffer[10];
  if(menu_item == 0){
    double current = get_value_current(index);
    sprintf(buffer,"%.2f A ", current);
    
    if(current != current_old){    
      display_cursor_position(1,3); 
      display_terminal_printf("        ");
      display_cursor_position(1,3);
      display_terminal_printf(buffer);
    }    
    
    current_old = current;
  }  
}

void display_refresh_percent_current(uint8_t index){
  if(menu_item == 0){
    uint8_t current = get_percent_current(index);
    display_bargraph_send_refresh(13,current);
  }  
}

void display_refresh_percent_soc(uint8_t index){
  char buffer[10];
  if(menu_item == 0){
    uint8_t soc = get_percent_soc(index);
    if(soc <= 33){
      display_bargraph_design(12);
      display_bargraph_colors(3,1,8);
      display_bargraph_define(0x4F,14,290,90,330,225,0,100,1); // SOC
    }
    else if((soc > 33) && (soc <= 66)){
      display_bargraph_design(12);
      display_bargraph_colors(7,1,8);
      display_bargraph_define(0x4F,14,290,90,330,225,0,100,1); // SOC
    }
    else{
      display_bargraph_design(12);
      display_bargraph_colors(4,1,8);
      display_bargraph_define(0x4F,14,290,90,330,225,0,100,1); // SOC
    }      
    display_bargraph_send_refresh(14,soc);
       
    sprintf(buffer,"%.2u %%",soc);
     
    display_cursor_position(1,8); 
    display_terminal_printf("     ");   
    display_cursor_position(1,8);
    display_terminal_printf(buffer);   
  }   
}
  
void display_refresh_value_switch(uint8_t index){
  if(menu_item == 0){
    bool relay = get_value_switch(index);
    if(relay == TRUE){
      display_clear_area(429,113,452,150); // Zeigerfläche löschen
      
      display_set_color_straight_line(GRUEN,1);      
      display_draw_line(310,60,310,75); // Linie + Pol 
      display_draw_line(310,240,310,255); // Linie - Pol
      display_draw_line(310,60,430,60); // Linie oben nach rechts 
      display_draw_line(310,255,430,255); // Linie unten nach rechts 
      display_draw_line(430,255,430,215); // Linie von unten rechts nach oben rechts 
      display_draw_line(430,60,430,104); // Linie von oben rechts nach unten rechts      
      display_draw_line(430,160,430,170); // Linie zwischen M und Switch     
    
      display_draw_line(430,114,430,149); // Schalter
    }
    else{
      display_clear_area(429,113,452,150); // Zeigerfläche löschen
      
      display_set_color_straight_line(ROT,1);      
      display_draw_line(310,60,310,75); // Linie + Pol 
      display_draw_line(310,240,310,255); // Linie - Pol
      display_draw_line(310,60,430,60); // Linie oben nach rechts 
      display_draw_line(310,255,430,255); // Linie unten nach rechts 
      display_draw_line(430,255,430,215); // Linie von unten rechts nach oben rechts 
      display_draw_line(430,60,430,104); // Linie von oben rechts nach unten rechts      
      display_draw_line(430,160,430,170); // Linie zwischen M und Switch        
        
      display_draw_line(430,114,450,149); // Schalter 
    }
  }  
}

void display_refresh_value_voltage(void){
  char buffer[10];
  if(menu_item == 0){
    double voltage = get_value_voltage();
    sprintf(buffer,"%.2f V ", voltage);
     
    if(voltage != voltage_old){
      display_cursor_position(1,1); 
      display_terminal_printf(buffer); 
    }
    
    voltage_old = voltage;
  }  
}

void display_refresh_value_power(void){
  char buffer[10];
  if(menu_item == 0){
    double power = (double)get_value_power();
    sprintf(buffer,"%.0f W ", power);
     
    if(power != power_old){
      display_cursor_position(1,5); 
      display_terminal_printf("       ");
      display_cursor_position(1,5); 
      display_terminal_printf(buffer);    
    }
    
    power_old = power;
  }
}

void display_refresh_value_egas(uint8_t index){;} // nicht definiert
void display_refresh_value_speed(uint8_t index){;} // nicht definiert
void display_refresh_value_warning(uint8_t index){;} // nicht definiert
void display_refresh_value_error(uint8_t index){;} // nicht definiert
void display_refresh_value_fanspeed(uint8_t index){;} // nicht definiert
void display_refresh_value_currentcenter(uint8_t index ){;} // nicht definiert
void display_refresh_value_currentlow(uint8_t index){;} // nicht definiert
