/**************************************** 
 * Stefan Wackwitz			*
 * stm32f10x_it.c         	        *
 ***************************************/

/* includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "main.h"

/* variables ---------------------------------------------------------------- */
CanRxMsg ResponseMessage;
static uint8_t key_state;
static bool press_flag;

/* functions ---------------------------------------------------------------- */
void NMI_Handler(void){
}

void HardFault_Handler(void){
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1){
  }
}

void MemManage_Handler(void){
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1){
  }
}

void BusFault_Handler(void){
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1){
  }
}

void UsageFault_Handler(void){
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1){
  }
}

void DebugMon_Handler(void){
}

void SVC_Handler(void){
}

void PendSV_Handler(void){
}

void SysTick_Handler(void){
}

void USB_LP_CAN1_RX0_IRQHandler(void){ // IR Handler für eingehende CAN Nachrichten
  uint8_t can_data[8]; /* Array für die CAN Daten erstellen, wird benötigt, da die Funktion
                          new_can_message(ResponseMessage.StdId, can_data); unabhängig von der Anzahl an Datenbytes (DLC),
                          immer 8 Datenbytes erwartet, die leeren Bytes werden mit 0 aufgefüllt  
                       */
 
  CAN_Receive(CAN1, CAN_FIFO0, &ResponseMessage); /* Nachricht aus dem FIFO lesen und in die Struktur ResponseMessage speichern,
                                                     welche alle Informationen der Nachricht beeinhaltet, wie z.B. ID, DLC & Daten
                                                  */
  
  USART_SendData(USART1, (uint8_t)((ResponseMessage.StdId & 0xFF00) >> 8)); // High Byte der CAN ID an die Telematik über den USART1 senden  
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); // Loop until the end of transmission
  
  USART_SendData(USART1, (uint8_t)(ResponseMessage.StdId & 0x00FF)); // Low Byte der CAN ID an die Telematik über den USART1 senden 
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); // Loop until the end of transmission
  
  USART_SendData(USART1, ResponseMessage.DLC); // Anzahl der Nutzdaten (DLC) der CAN Nachricht an die Telematik über den USART1 senden  
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); // Loop until the end of transmission
  
  for(uint8_t i = 0 ; i < ResponseMessage.DLC ; i++){ // Nutzdaten der CAN Nachricht an die Telematik über den USART1 senden  
    can_data[i] = ResponseMessage.Data[i]; // Informationen in can_data schreiben
    USART_SendData(USART1, ResponseMessage.Data[i]); // verschicken
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); // Loop until the end of transmission
  }  
  
  USART_SendData(USART1, 0x0D); // CR carriage return (0x0D) an die Telematik über den USART1 senden  
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); // Loop until the end of transmission
  
  USART_SendData(USART1, 0x0A); // LF line feed (0x0A) an die Telematik über den USART1 senden  
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); // Loop until the end of transmission 
  
  for(uint8_t i = ResponseMessage.DLC ; i < 8 ; i++){ // Felder ohne Informationen mit 0 füllen --> nötigt für die Funktion new_can_message(ResponseMessage.StdId, can_data); 
    can_data[i] = 0;
  }    
  new_can_message(ResponseMessage.StdId, can_data); /* Informationen werden in einem Array abgespeichert, somit steht immer der letze Wert zur Verfügung,
                                                       zudem werden relevante Informationen in darstellbare Einheiten umgerechnet und der Wert dieser Informationen,
                                                       auf dem Display aktualisiert, trifft also z.B. ein neuer Zellspannungswert ein, so wird dieser bei den entsprechenden
                                                       Displaykoordinaten neu gezeichnet
                                                    */
}

void TIM2_IRQHandler(void){ // IR Handler für den Timer 2 --> wird zur Entprellung des Tasters verwendet
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){ // check pending bit
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // clear pending bit   
    
     /*
        alle 20ms wird geprüft, ob der Taster gedrückt wurde, ein Prellen dauert weniger als 1ms,
        ein Tasterdruck dauert ca. 100ms | Wird beim Interrupt ein Tastendruck festgestellt, wird dies in der Variablen key_state gespeichert,
        tritt nach 20ms wieder ein IR auf, wird der Zustand des Tasters wieder geprüft, wird der Taster wieder als "gedrückt" detektiert, wird folgende Abfrage wahr:
     */
    
    if(key_state == FALSE && GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == FALSE){
      
      /*
        ein Tastendruck wurde nun erfolgreich detektiert, da die Taste mindestens 40ms gedrückt wurde, der Taster wurde erfolgreich entpellt,
        da das zeitlich verhältnismäßig kurze Prellen des Taster durch die lange Sample Periode nicht mehr detektiert wird, es wurde gewissermaßen tiefpassgefiltert 
      */
      
      if(press_flag == FALSE){
        
        // wechsel zum nächsten Menüpunkt
        if(menu_item == 1){
          menu_item = 0;
        }
        else{  
           menu_item++; // Menüpunkt hochzählen
        }        
                
        switch(menu_item){ // bei erfolgreich detektiertem Tastendruck --> wechsel zwischen den Menüpunkten
          case 0:            
            display_show_menu0(); // Tacho
            break;
          case 1:            
            display_show_menu1(); // Cells
            break;          
        }        
        press_flag = TRUE;
      }      
    }
    else{
      /*
        die Variable press_flag ist nötig, um zu gewährleisten, dass die Funktion display_show_menu0/1(); bei einem längeren Tastendruck nur einmal ausgeführt wird, wird der Taster
        losgelassen, wird die Variable zurückgesetzt und kann anschließend bei erneutem Tastendruck wieder einen mehrfachen Funktionsaufruf verhindern
      */      
      press_flag = FALSE;
    }
    
    key_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1); // Zustand des Tastendrucks speichern und beim nächsten IR mit dem dann aktuellen Zustand vergleichen
  }  
}


