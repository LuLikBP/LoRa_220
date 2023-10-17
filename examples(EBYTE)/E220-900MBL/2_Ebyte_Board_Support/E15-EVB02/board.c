/**
  **********************************************************************************
  * @file      board.c
  * @brief     E15-EVB02 �弶����������     
  * @details   ������μ� https://www.ebyte.com/       
  * @author    JiangHeng     
  * @date      2021-05-06     
  * @version   1.0.0     
  **********************************************************************************
  * @copyright BSD License 
  *            �ɶ��ڰ��ص��ӿƼ����޹�˾  
  *   ______   ____   __     __  _______   ______ 
  *  |  ____| |  _ \  \ \   / / |__   __| |  ____|
  *  | |__    | |_) |  \ \_/ /     | |    | |__   
  *  |  __|   |  _ <    \   /      | |    |  __|  
  *  | |____  | |_) |    | |       | |    | |____ 
  *  |______| |____/     |_|       |_|    |______| 
  *
  **********************************************************************************
  */

#include "board.h"
#include "ebyte_conf.h"

BSP_BTN_FIFO_t BSP_BTN_FIFO;    
      

/*!< @brief ������ʱ���� ���ڶ�ʱ���ж� �ݼ� */
volatile uint32_t Ebyte_TimerDelayCounter = 0;

/* !
 * @brief �ڲ�ʱ�ӳ�ʼ��
 */
void Ebyte_BSP_HSI_Init(void)
{
  /* �ڲ� 16M HSI ʱ�� */ 
  CLK_SYSCLKSourceConfig( CLK_SYSCLKSource_HSI );
  
  /* 1��Ƶ  16M/1 */
  CLK_SYSCLKDivConfig( CLK_SYSCLKDiv_1 );
}



void Ebyte_BSP_E22xE220xGPIO_Init(void)
{
    /* E22/E220 */
    GPIO_Init( BSP_GPIO_PORT_NRST, BSP_GPIO_PIN_NRST, GPIO_Mode_Out_PP_High_Slow );  //ģ��:�͵�ƽ��λ 
    GPIO_Init( BSP_GPIO_PORT_BUSY, BSP_GPIO_PIN_BUSY, GPIO_Mode_In_FL_No_IT      );  
    GPIO_Init( BSP_GPIO_PORT_RXEN, BSP_GPIO_PIN_RXEN, GPIO_Mode_Out_PP_Low_Slow  );  //ģ��:�ߵ�ƽ��������
    GPIO_Init( BSP_GPIO_PORT_TXEN, BSP_GPIO_PIN_TXEN, GPIO_Mode_Out_PP_Low_Slow  );  //ģ��:�ߵ�ƽ��������   
    GPIO_ExternalPullUpConfig( BSP_GPIO_PORT_NRST, BSP_GPIO_PIN_NRST, ENABLE);
    GPIO_ExternalPullUpConfig( BSP_GPIO_PORT_RXEN, BSP_GPIO_PIN_RXEN, ENABLE);
    GPIO_ExternalPullUpConfig( BSP_GPIO_PORT_TXEN, BSP_GPIO_PIN_TXEN, ENABLE);
}

void Ebyte_BSP_E10xGPIO_Init(void)
{
    GPIO_Init( BSP_GPIO_PORT_E10_SDN, BSP_GPIO_PIN_E10_SDN, GPIO_Mode_Out_PP_High_Slow ); //E10 �ߵ�ƽ��λ
    GPIO_Init( BSP_GPIO_PORT_E10_IRQ, BSP_GPIO_PIN_E49_DIO2, GPIO_Mode_In_FL_No_IT      );  
}

void Ebyte_BSP_E49xGPIO_Init(void)
{
    GPIO_Init( BSP_GPIO_PORT_E49_DIO1, BSP_GPIO_PIN_E49_DIO1,  GPIO_Mode_In_PU_No_IT      ); 
    GPIO_Init( BSP_GPIO_PORT_E49_DIO2, BSP_GPIO_PIN_E49_DIO2,  GPIO_Mode_In_PU_IT      ); 
    EXTI_SetPinSensitivity(EXTI_Pin_2,EXTI_Trigger_Rising);

    
    
    GPIO_Init( BSP_GPIO_PORT_E49_CSB,  BSP_GPIO_PIN_E49_CSB,  GPIO_Mode_Out_PP_High_Fast );
    GPIO_Init( BSP_GPIO_PORT_E49_FCSB, BSP_GPIO_PIN_E49_FCSB, GPIO_Mode_Out_PP_High_Fast );
    GPIO_Init( BSP_GPIO_PORT_E49_SDIO, BSP_GPIO_PIN_E49_SDIO, GPIO_Mode_Out_PP_High_Fast );
    GPIO_Init( BSP_GPIO_PORT_E49_SLCK, BSP_GPIO_PIN_E49_SLCK, GPIO_Mode_Out_PP_Low_Fast );
    GPIO_ExternalPullUpConfig( BSP_GPIO_PORT_E49_CSB, BSP_GPIO_PIN_E49_CSB, ENABLE);
    GPIO_ExternalPullUpConfig( BSP_GPIO_PORT_E49_FCSB, BSP_GPIO_PIN_E49_FCSB, ENABLE);
    GPIO_ExternalPullUpConfig( BSP_GPIO_PORT_E49_SLCK, BSP_GPIO_PIN_E49_SLCK, ENABLE);
}

/* !
 * @brief ��ʼ������IO
 * 
 * @note  Ŀ��Ӳ��: EBYTE E15-EVB02 
 */
void Ebyte_BSP_GPIO_Init(void)
{
    /* ����ģ��״̬���� */
    
#if defined(EBYTE_E22_400M22S)||defined(EBYTE_E22_900M22S)||defined(EBYTE_E220_400M22S)||defined(EBYTE_E220_900M22S)
    Ebyte_BSP_E22xE220xGPIO_Init();
    
#elif defined(EBYTE_E10_400M20S)||defined(EBYTE_E10_900M20S)   
    Ebyte_BSP_E10xGPIO_Init();
    
#elif defined(EBYTE_E49_900M20S)
    Ebyte_BSP_E49xGPIO_Init();
     
#endif   
    /* LED */
    GPIO_Init( BSP_GPIO_PORT_LED_1, BSP_GPIO_PIN_LED_1, GPIO_Mode_Out_PP_Low_Slow );
    GPIO_Init( BSP_GPIO_PORT_LED_2, BSP_GPIO_PIN_LED_2, GPIO_Mode_Out_PP_Low_Slow );   
       
    /* ���� */
    GPIO_Init( BSP_GPIO_PORT_BUTTON_1, BSP_GPIO_PIN_BUTTON_1, GPIO_Mode_In_PU_No_IT ); 

    GPIO_Init( BSP_GPIO_PORT_BUTTON_2, BSP_GPIO_PIN_BUTTON_2, GPIO_Mode_In_PU_No_IT );     
}

/* !
 * @brief ͨ�Ŵ��ڳ�ʼ��
 * 
 * @note  ��ע�⣬��ͬ��MCU��������˿�ӳ��
 */
void Ebyte_BSP_UART_Init( void )
{
    /* ʱ�� */
    CLK_PeripheralClockConfig( BSP_USER_UART_CLOCK, ENABLE);
    
    /* GPIO */
    GPIO_ExternalPullUpConfig( BSP_GPIO_PORT_UART_TX, BSP_GPIO_PIN_UART_TX, ENABLE );
    GPIO_ExternalPullUpConfig( BSP_GPIO_PORT_UART_RX, BSP_GPIO_PIN_UART_RX, ENABLE );

    /* �˿���ӳ��  */
    SYSCFG_REMAPPinConfig( REMAP_Pin_USART1TxRxPortA, ENABLE ); 
    
    /* ������������ E15-EVB02Ĭ�ϲ�����9600 8N1 */
    USART_Init( BSP_USER_UART, BSP_USER_UART_BAUDRATE, USART_WordLength_8b, USART_StopBits_1, BSP_USER_UART_PARITY,  (USART_Mode_TypeDef)(USART_Mode_Rx | USART_Mode_Tx ));//�������պͷ���
    
    /* �򿪽����ж� */;
    USART_ITConfig( BSP_USER_UART, USART_IT_RXNE, ENABLE );
     
    /* ���� ʹ�� */
    USART_Cmd( BSP_USER_UART, ENABLE);
}

void Ebyte_BSP_ThreeLinesSPI_Init()
{
    /* �ϲ�����E49 GPIO��ʼ���� */
}

/* !
 * @brief ����ģ��ͨ��SPI�ӿڳ�ʼ������
 */
void Ebyte_BSP_SPI_Init( void )
{
    /* ʱ�� */
    CLK_PeripheralClockConfig( CLK_Peripheral_SPI1, ENABLE);
    
    /* GPIO */
    GPIO_Init( BSP_GPIO_PORT_SPI_NSS,  BSP_GPIO_PIN_SPI_NSS,  GPIO_Mode_Out_PP_High_Slow); //Ƭѡ CS
    GPIO_ExternalPullUpConfig( BSP_GPIO_PORT_SPI_SCK, BSP_GPIO_PIN_SPI_MOSI | BSP_GPIO_PIN_SPI_MISO | BSP_GPIO_PIN_SPI_SCK, ENABLE); // MOSI MISO SCK   
    
    /* �������� */
    SPI_Init(  BSP_RF_SPI, 
               SPI_FirstBit_MSB,                 //�Ӹ�λ��ʼ����
               SPI_BaudRatePrescaler_2,          //16M/2 SCK����
               SPI_Mode_Master,                  //����ģʽ
               SPI_CPOL_Low,                     //���� CPOL=0  
               SPI_CPHA_1Edge,                   //���� CPHA=0  ��һ��ʱ�ӱ��ز�������
               SPI_Direction_2Lines_FullDuplex,  //ȫ˫��
               SPI_NSS_Soft,                     //�������ƴӻ�CSƬѡ
               0x07);                            //CRC����
    
    /* ʹ�� */
    SPI_Cmd( BSP_RF_SPI, ENABLE );
}

/* !
 * @brief RFģ��SPIͨ����/������
 * 
 * @param data ��������  
 * @return �������� 
 * @note stm8l SPI�⺯���е�SPI_SendData()/SPI_ReceiveData() ����ֱ��ʹ��
 */
uint8_t Ebyte_BSP_SpiTransAndRecv( uint8_t data )
{
    BSP_RF_SPI->DR = data;
    while ((BSP_RF_SPI->SR & SPI_FLAG_TXE) == RESET);
    
    while ((BSP_RF_SPI->SR & SPI_FLAG_RXNE) == RESET);
    return BSP_RF_SPI->DR;
}

/* !
 * @brief ��ʱ����ʼ��
 *
 * @note  ʹ����TIM3����1ms�������ж�
 *        TIM3����ʱ��ΪHSI 16MHz, 128��Ƶ��Ϊ 16 MHz / 128 = 125 000 Hz
 *        Ŀ�궨ʱ1ms �������ڼ�Ϊ ( 0.001 x 125000 - 1) = 124 
 */
void Ebyte_BSP_TIMER_Init( void )
{
    /* ʱ�� */
    CLK_PeripheralClockConfig( CLK_Peripheral_TIM3, ENABLE); 
    
    /* ���� */
    TIM3_TimeBaseInit( TIM3_Prescaler_128 , TIM3_CounterMode_Up, 124);
    
    /* �����ж� */
    TIM3_ClearFlag(TIM3_FLAG_Update);
    TIM3_ITConfig(TIM3_IT_Update, ENABLE);
    
    /* ʹ�� */
    TIM3_Cmd(ENABLE);
}       

/* !
 * @brief E15-EVB02 ������Դ��ʼ��
 * 
 * @note  �ڲ�ʱ��HSI  x 16MHz
 *        �û�ͨ�Ŵ��� x USART1
 *        ����ģ��ͨ�Žӿ� x SPI1
 *        ��ʱ��  x TIM3
 *        ����    x 2
 *        ָʾ��  x 2
 */
void Ebyte_BSP_Init( void )
{
    /* ʱ��     ��ʼ�� */
    Ebyte_BSP_HSI_Init();
    
    /* IO       ��ʼ�� */
    Ebyte_BSP_GPIO_Init();
    
    /* ����     ��ʼ�� */
    Ebyte_BSP_UART_Init();
  
    /* SPI�ӿ�  ��ʼ��  (E49�Ƚ�����) */
#if defined(EBYTE_E49_900M20S)  
    Ebyte_BSP_ThreeLinesSPI_Init();
#else    
    Ebyte_BSP_SPI_Init();
#endif    
    
    /* ��ʱ��   ��ʼ�� */
    Ebyte_BSP_TIMER_Init();
    
    /* �����¼����� ��ʼ�� */
    Ebyte_BTN_FIFO_Init( &BSP_BTN_FIFO );
    
    
    

}

/* !
 * @brief ����LED ��/��/��ת
 * 
 * @param LEDx  �������ŷ��������
 *              @arg BSP_LED_1 : LED1
 *              @arg BSP_LED_2 : LED2 
 * 
 * @param ctl   �� / ��
 *              @arg OFF     : ��
 *              @arg ON      : ��  
 *              @arg TOGGLE  : ��ת 
 */
void Ebyte_BSP_LedControl( BSP_LED_t LEDx , BSP_LED_Ctl_t ctl)
{
    if( TOGGLE == ctl )
    {
      switch( LEDx )
      {
        case BSP_LED_1 : GPIO_ToggleBits( BSP_GPIO_PORT_LED_1, BSP_GPIO_PIN_LED_1); break;
        case BSP_LED_2 : GPIO_ToggleBits( BSP_GPIO_PORT_LED_2, BSP_GPIO_PIN_LED_2); break;
        default: break;
      }    
    }
    else
    {
      switch( LEDx )
      {
        case BSP_LED_1 : GPIO_WriteBit( BSP_GPIO_PORT_LED_1, BSP_GPIO_PIN_LED_1, (BitAction)ctl); break;
        case BSP_LED_2 : GPIO_WriteBit( BSP_GPIO_PORT_LED_2, BSP_GPIO_PIN_LED_2, (BitAction)ctl); break;
        default: break;
      }    
    }

}


/* !
 * @brief ���ڶ�ʱ���ĺ�����ʱ����
 * 
 * @param nTime ��λ:����
 */
void Ebyte_BSP_DelayMs( volatile uint32_t nTime )
{
    Ebyte_TimerDelayCounter = nTime;
    
    while( Ebyte_TimerDelayCounter !=0 );
}

/* !
 * @brief ����������ʱ���� ��ʱ���жϵ���
 */
void Ebyte_BSP_TimerDecrement(void)
{

  
    if( Ebyte_TimerDelayCounter != 0 ) 
    {
        Ebyte_TimerDelayCounter--;
    }
}


/* !
 * @brief ��ȡ����״̬
 * 
 * @param btn ��Ӧ�İ������
 *            @arg BSP_BUTTON_1 :����1 
 *            @arg BSP_BUTTON_2 :����2
 * @return 0:����������  ��0:����δ���� 
 * @note  ���ذ���δ����ʱ IO��������״̬ ��Ϊ1�����º�IO�ӵ� ��Ϊ0
 */
uint8_t Ebyte_BSP_ReadButton( BSP_BUTTON_t btn )
{
    BitStatus result = RESET;
    
    switch ( btn )
    {
      case BSP_BUTTON_1: result = GPIO_ReadInputDataBit( BSP_GPIO_PORT_BUTTON_1 , BSP_GPIO_PIN_BUTTON_1); break;
      case BSP_BUTTON_2: result = GPIO_ReadInputDataBit( BSP_GPIO_PORT_BUTTON_2 , BSP_GPIO_PIN_BUTTON_2); break;
      default : break;
    }
    
    return result;
}


/* !
 * @brief ���ڷ��ͺ���
 */
void Ebyte_BSP_UartTransmit( uint8_t *buffer , uint16_t length )
{
  
    uint8_t i;
    
    for( i = 0; i < length; i++ )
    {
        while ( USART_GetFlagStatus( USART1, USART_FLAG_TXE ) == RESET );
        USART_SendData8( USART1, *buffer++ );
    }  
}
