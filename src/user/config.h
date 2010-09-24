/**************************************** 
 * Stefan Wackwitz			*
 * config.h			        *
 ***************************************/

#ifndef __CONFIG_H
#define __CONFIG_H

/* includes ----------------------------------------------------------------- */
#include "main.h"

/* functions ---------------------------------------------------------------- */
void RCC_Configuration(void); // Prozessortakt einstellen, Komponenten mit Takt "versorgen"
void GPIO_Configuration(void); // Ein- und Ausgänge konfigurieren z.B. Pull-Up Widerstände aktivieren etc.
void USART_Configuration(void); // USART konfigurieren (Schnittstelle zwischen Kombiinstrument und Telematik)
void CAN_Configuration(void); // CAN Interface konfigurieren (ID Filter, Baudrate usw.)
void I2C_Configuration(void); // I2C konfigurieren (Schnittstelle zwischen Mikrocontroller und Display)
void NVIC_Configuration(void); // Interrupt Controller konfigurieren
void DISPLAY_Configuration(void); // Display hochfahren (zeitliche Verzögerung, damit die I2C Verbindung funktioniert) 
void TIM_Configuration(void); // Timer zur Tasterabfrage (Entprellung) konfigurieren

#endif /* __CONFIG_H */
