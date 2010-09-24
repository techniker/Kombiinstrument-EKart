/**************************************** 
 * Stefan Wackwitz			*
 * main.c			        *
 ***************************************/

/* includes ----------------------------------------------------------------- */
#include "main.h"

/* variables ---------------------------------------------------------------- */
uint8_t menu_item = 0; // Variable für den Menüpunkt | 0 = Tacho | 1 = Cells

/* functions ---------------------------------------------------------------- */
void main(void){
  RCC_Configuration(); // Prozessortakt einstellen, Komponenten mit Takt "versorgen"
  GPIO_Configuration(); // Ein- und Ausgänge konfigurieren z.B. Pull-Up Widerstände aktivieren etc.
  
  for(int nTime = 0 ; nTime <= 0xFFFFFF ; nTime++); // Display hochfahren --> erst dann I2C Kommunikationen starten, da sonst Fehler auftreten
  
  I2C_Configuration(); // I2C konfigurieren (Schnittstelle zwischen Mikrocontroller und Display)
  USART_Configuration(); // USART konfigurieren (Schnittstelle zwischen Kombiinstrument und Telematik)
  CAN_Configuration(); // CAN Interface konfigurieren (ID Filter, Baudrate usw.)
  TIM_Configuration(); // Timer zur Tasterabfrage (Entprellung) konfigurieren
  DISPLAY_Configuration(); // Display hochfahren (zeitliche Verzögerung, damit die I2C Verbindung funktioniert) 
  NVIC_Configuration(); // Interrupt Controller konfigurieren
  
  /*
    Programm ist Interruptgesteuert --> siehe lib_src \ stm32f10x_it.c --> IR Handler für Timer und CAN
  */
  
  while(1); // main loop
}