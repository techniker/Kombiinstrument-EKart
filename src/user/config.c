/**************************************** 
 * Stefan Wackwitz			*
 * config.c			        *
 ***************************************/

/* includes ----------------------------------------------------------------- */
#include "config.h"

/* functions ---------------------------------------------------------------- */
void RCC_Configuration(void){ // Prozessortakt einstellen, Komponenten mit Takt "versorgen"
  /*	Setup the microcontroller system. Initialize the Embedded Flash Interface,
        initialize the PLL and update the SystemFrequency variable. */
  SystemInit();
  /* Enable AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // Takt für AFIO aktivieren --> benötigt für PinRemap der CAN Pins
  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // Takt für Port A aktivieren
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // Takt für Port B aktivieren
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); // Takt für Port D aktivieren
  /* Enable USART1 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // Takt für USART1 aktivieren
  /* Enable I2C1 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE); // Takt für I2C1 aktivieren  
  /* Enable CAN1 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); // Takt für CAN1 aktivieren  
  /* Enable TIM2 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // Takt für Timer 2 aktivieren   
}

void GPIO_Configuration(void){ // Ein- und Ausgänge konfigurieren z.B. Pull-Up Widerstände aktivieren etc.
  /* Configure USART1 RX as input floating */
  GPIO_InitTypeDef GPIO_MENU;
  GPIO_MENU.GPIO_Pin = GPIO_Pin_1;
  GPIO_MENU.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_MENU.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_MENU);   
  
  /* Configure USART1 TX as alternate function push-pull */
  GPIO_InitTypeDef GPIO_USART1_TX;
  GPIO_USART1_TX.GPIO_Pin = GPIO_Pin_9;
  GPIO_USART1_TX.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_USART1_TX.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_USART1_TX);
  
  /* Configure USART1 RX as input floating */
  GPIO_InitTypeDef GPIO_USART1_RX;
  GPIO_USART1_RX.GPIO_Pin = GPIO_Pin_10;
  GPIO_USART1_RX.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_USART1_RX.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_USART1_RX);    
  
  /* Configure CAN1 RX as input pull-pull */
  GPIO_InitTypeDef GPIO_CAN1_RX;
  //GPIO_CAN1_RX.GPIO_Pin = GPIO_Pin_0;
  GPIO_CAN1_RX.GPIO_Pin = GPIO_Pin_11;
  GPIO_CAN1_RX.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_CAN1_RX.GPIO_Speed = GPIO_Speed_50MHz;
  //GPIO_Init(GPIOD,&GPIO_CAN1_RX);  
  GPIO_Init(GPIOA,&GPIO_CAN1_RX);
  
  /* Configure CAN1 TX as alternate function push-pull */
  GPIO_InitTypeDef GPIO_CAN1_TX;
  //GPIO_CAN1_TX.GPIO_Pin = GPIO_Pin_1;
  GPIO_CAN1_TX.GPIO_Pin = GPIO_Pin_12;
  GPIO_CAN1_TX.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_CAN1_TX.GPIO_Speed = GPIO_Speed_50MHz;
  //GPIO_Init(GPIOD,&GPIO_CAN1_TX);
  GPIO_Init(GPIOA,&GPIO_CAN1_TX);
  
  
  //GPIO_PinRemapConfig(GPIO_Remap2_CAN1,ENABLE);
  
  /* Configure I2C1 SCL and SDA as alternate function push-pull */  
  GPIO_InitTypeDef GPIO_I2C;
  GPIO_I2C.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;        
  GPIO_I2C.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_I2C.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_I2C);
}

void USART_Configuration(void){ // USART konfigurieren (Schnittstelle zwischen Kombiinstrument und Telematik)
  USART_InitTypeDef USART1_InitStructure;        
       
  USART1_InitStructure.USART_BaudRate = 115200;
  USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART1_InitStructure.USART_StopBits = USART_StopBits_1;
  USART1_InitStructure.USART_Parity = USART_Parity_No;
  USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART1_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  
  USART_Init(USART1, &USART1_InitStructure);
  
  USART_Cmd(USART1, ENABLE);
}

void I2C_Configuration(void){ // I2C konfigurieren (Schnittstelle zwischen Mikrocontroller und Display)
  I2C_InitTypeDef I2C1_InitStructure;              
        
  I2C1_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C1_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C1_InitStructure.I2C_OwnAddress1 = 0xAA;
  I2C1_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C1_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C1_InitStructure.I2C_ClockSpeed = 40000;
  I2C_Init(I2C1, &I2C1_InitStructure);   
        
  I2C_Cmd(I2C1, ENABLE);
}

void CAN_Configuration(void){ // CAN Interface konfigurieren (ID Filter, Baudrate usw.)
  CAN_InitTypeDef CAN1_InitStructure;
  
  CAN1_InitStructure.CAN_TTCM = DISABLE; // disable time triggered communication mode
  CAN1_InitStructure.CAN_ABOM = DISABLE; //disable automatic bus-off management
  CAN1_InitStructure.CAN_AWUM = DISABLE; // disable automatic wake-up mode
  CAN1_InitStructure.CAN_NART = DISABLE; // disable no-automatic retransmission mode
  CAN1_InitStructure.CAN_RFLM = DISABLE; // disable Receive FIFO Locked mode
  CAN1_InitStructure.CAN_TXFP = DISABLE; // disable transmit FIFO priority
  CAN1_InitStructure.CAN_Mode = CAN_Mode_Normal; // normal CAN operating mode
  CAN1_InitStructure.CAN_SJW = CAN_SJW_1tq; // a bit change is expected to occur within this time segment
  CAN1_InitStructure.CAN_BS1 = CAN_BS1_4tq; // defines the location of the sample point
  CAN1_InitStructure.CAN_BS2 = CAN_BS2_7tq; // defines the location of the transmit point
  CAN1_InitStructure.CAN_Prescaler = 24; // BaudRate = 1 / NominalBitTime ==> 125.000 bit/s
                                        // NominalBitTime = 1 * tq + tBS1 + tBS2
                                        // tq = (BRP[9:0] + 1) x tPCLK
                                        // tBS1 = tq x (TS1[3:0] + 1)
                                        // tBS2 = tq x (TS2[2:0] + 1)
  CAN_Init(CAN1, &CAN1_InitStructure); // CAN initialisieren
   
  CAN_FilterInitTypeDef CAN1_FilterInitStructure;
 
  CAN1_FilterInitStructure.CAN_FilterNumber = 0;
  CAN1_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN1_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
  CAN1_FilterInitStructure.CAN_FilterIdHigh = 0x0000; // CAN ID's werden mit 0 maskiert --> alle ID's kommen durch
  CAN1_FilterInitStructure.CAN_FilterIdLow = 0x0000; // CAN ID's werden mit 0 maskiert --> alle ID's kommen durch
  CAN1_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000; // CAN ID's werden mit 0 maskiert --> alle ID's kommen durch
  CAN1_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000; // CAN ID's werden mit 0 maskiert --> alle ID's kommen durch
  CAN1_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN1_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN1_FilterInitStructure); // ID Filter initialisieren
  
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE); // Interrupt konfiguration --> IR wird ausgelöst wenn eine Nachricht eintrifft
}

void NVIC_Configuration(void){ // Interrupt Controller konfigurieren
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn; // CAN1 Interrupt aktivieren
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // Priorität 0 ==> höchste Priorität
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; // Timer 2 Interrupt aktivieren
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // Priorität 1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void TIM_Configuration(void){ // Timer zur Tasterabfrage (Entprellung) konfigurieren
  TIM_TimeBaseInitTypeDef TIM2_TimeBaseStructure;    
  
  TIM2_TimeBaseStructure.TIM_Period = 19; // T = 20ms
  TIM2_TimeBaseStructure.TIM_Prescaler = 35999; // Takt Vorteiler ==> 36000 (35999 + 1)
  TIM2_TimeBaseStructure.TIM_ClockDivision = 0; // Takt kann noch weiter heruntergeteilt werden, wird nicht benötigt
  TIM2_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // Timer zählt aufwärts
  
  TIM_TimeBaseInit(TIM2, &TIM2_TimeBaseStructure); // Timer als normalen Timer mit Zeitbasis initialisieren
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // Timer Interrupt aktivieren
  TIM_UpdateRequestConfig(TIM2, TIM_UpdateSource_Global); // Interrupt wird bei einem Überlauf ausgelöst
  
  TIM_Cmd(TIM2, ENABLE); // Zählvorgang starten
}

void DISPLAY_Configuration(void){ // Display hochfahren (zeitliche Verzögerung, damit die I2C Verbindung funktioniert) 
  display_show_menu0(); // nach dem Hochfahren wird Menüüunkt 0 (Tacho) angezeigt
}