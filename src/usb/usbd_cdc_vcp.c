/**
******************************************************************************
* @file    usbd_cdc_vcp.c
* @author  MCD Application Team
* @version V1.0.0
* @date    31-January-2014
* @brief   Generic media access Layer.
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
*
* Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
* You may not use this file except in compliance with the License.
* You may obtain a copy of the License at:
*
*        http://www.st.com/software_license_agreement_liberty_v2
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_vcp.h"
#include "time.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/



//static void STM_EVAL_COMInit(void);


LINE_CODING linecoding =
{
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* nb. of bits 8*/
};




extern __IO uint8_t USB_Tx_State;
__IO uint32_t bDeviceState = UNCONNECTED;


#define APP_TX_DATA_SIZE               64

/* These are external variables imported from CDC core to be used for IN transfer management. */

/* This is the buffer for data received from the MCU to APP (i.e. MCU TX, APP RX) */
extern uint8_t APP_Rx_Buffer[];
extern uint32_t APP_Rx_ptr_out;
/* Increment this buffer position or roll it back to
 start address when writing received data
 in the buffer APP_Rx_Buffer. */
extern uint32_t APP_Rx_ptr_in;

/*
    APP TX is the circular buffer for data that is transmitted from the APP (host)
    to the USB device (flight controller).
*/
static uint8_t APP_Tx_Buffer[APP_TX_DATA_SIZE];
static uint32_t APP_Tx_ptr_out = 0;
static uint32_t APP_Tx_ptr_in = 0;
/* Private function prototypes -----------------------------------------------*/
static uint16_t VCP_Init     (void);
static uint16_t VCP_DeInit   (void);
static uint16_t VCP_Ctrl     (uint32_t Cmd, uint8_t* Buf, uint32_t Len);
uint16_t VCP_DataTx   (uint8_t* Buf, uint32_t Len);
uint16_t VCP_DataRx   (uint8_t* Buf, uint32_t Len);

#define USB_TIMEOUT  50


static uint16_t VCP_COMConfig(uint8_t Conf);

CDC_IF_Prop_TypeDef VCP_fops =
{
    VCP_Init,
    VCP_DeInit,
    VCP_Ctrl,
    VCP_DataRx,
    VCP_DataRx
};

/* Private functions ---------------------------------------------------------*/
/**
* @brief  VCP_Init
*         Initializes the Media on the STM32
* @param  None
* @retval Result of the operation (USBD_OK in all cases)
*/
static uint16_t VCP_Init(void)
{

    bDeviceState = CONFIGURED;
    /* Configure and enable the USART */
// STM_EVAL_COMInit();

    return USBD_OK;
}

/**
* @brief  VCP_DeInit
*         DeInitializes the Media on the STM32
* @param  None
* @retval Result of the opeartion (USBD_OK in all cases)
*/
static uint16_t VCP_DeInit(void)
{
    bDeviceState = UNCONNECTED;
    return USBD_OK;
}


/**
* @brief  VCP_Ctrl
*         Manage the CDC class requests
* @param  Cmd: Command code
* @param  Buf: Buffer containing command data (request parameters)
* @param  Len: Number of data to be sent (in bytes)
* @retval Result of the operation (USBD_OK in all cases)
*/
static uint16_t VCP_Ctrl (uint32_t Cmd, uint8_t* Buf, uint32_t Len)
{
    switch (Cmd)
    {
//    case SEND_ENCAPSULATED_COMMAND:
//    /* Not  needed for this driver */
//    break;
//
//    case GET_ENCAPSULATED_RESPONSE:
//    /* Not  needed for this driver */
//    break;
//
//    case SET_COMM_FEATURE:
//    /* Not  needed for this driver */
//    break;
//
//    case GET_COMM_FEATURE:
//    /* Not  needed for this driver */
//    break;
//
//    case CLEAR_COMM_FEATURE:
//    /* Not  needed for this driver */
//    break;

    case SET_LINE_CODING:
        linecoding.bitrate = (uint32_t)(Buf[0] | (Buf[1] << 8) | (Buf[2] << 16) | (Buf[3] << 24));
        linecoding.format = Buf[4];
        linecoding.paritytype = Buf[5];
        linecoding.datatype = Buf[6];
        /* Set the new configuration */
        VCP_COMConfig(OTHER_CONFIG);
        break;

    case GET_LINE_CODING:
        Buf[0] = (uint8_t)(linecoding.bitrate);
        Buf[1] = (uint8_t)(linecoding.bitrate >> 8);
        Buf[2] = (uint8_t)(linecoding.bitrate >> 16);
        Buf[3] = (uint8_t)(linecoding.bitrate >> 24);
        Buf[4] = linecoding.format;
        Buf[5] = linecoding.paritytype;
        Buf[6] = linecoding.datatype;
        break;

//    case SET_CONTROL_LINE_STATE:
//    /* Not  needed for this driver */
//    break;
//
//    case SEND_BREAK:
//    /* Not  needed for this driver */
//    break;

    default:
        break;
    }

    return USBD_OK;
}

uint8_t usbIsConnected(void)
{
    return (bDeviceState != UNCONNECTED);
}

uint8_t usbIsConfigured(void)
{
    return (bDeviceState == CONFIGURED);
}


/**
* @brief  VCP_DataTx
*         CDC received data to be send over USB IN endpoint are managed in
*         this function.
* @param  Buf: Buffer of data to be sent
* @param  Len: Number of data to be sent (in bytes)
* @retval Result of the operation: USBD_OK if all operations are OK else VCP_FAIL
*/
/*******************************************************************************
 * Function Name  : Receive DATA .
 * Description    : receive the data from the PC to STM32 and send it through USB
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/

uint32_t CDC_Send_DATA(uint8_t *ptrBuffer, uint32_t sendLength)
{

    if (!(usbIsConnected() && usbIsConfigured())) {
        return 0;
    }

    VCP_DataTx(ptrBuffer, sendLength);
    return sendLength;
}

uint32_t CDC_Send_FreeBytes(void)
{
    /*
        return the bytes free in the circular buffer

        functionally equivalent to:
        (APP_Rx_ptr_out > APP_Rx_ptr_in ? APP_Rx_ptr_out - APP_Rx_ptr_in : APP_RX_DATA_SIZE - APP_Rx_ptr_in + APP_Rx_ptr_in)
        but without the impact of the condition check.
    */
    return ((APP_Rx_ptr_out - APP_Rx_ptr_in) + (-((int)(APP_Rx_ptr_out <= APP_Rx_ptr_in)) & APP_RX_DATA_SIZE)) - 1;
}

/**
 * @brief  VCP_DataTx
 *         CDC data to be sent to the Host (app) over USB
 * @param  Buf: Buffer of data to be sent
 * @param  Len: Number of data to be sent (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else VCP_FAIL
 */
static uint16_t VCP_DataTx(uint8_t* Buf, uint32_t Len)
{
    /*
        make sure that any paragraph end frame is not in play
        could just check for: USB_CDC_ZLP, but better to be safe
        and wait for any existing transmission to complete.
    */
    if(USB_Tx_State != 0)
    {
        return 0;
    }

    for (uint32_t i = 0; i < Len; i++) {
        APP_Rx_Buffer[APP_Rx_ptr_in] = Buf[i];
        APP_Rx_ptr_in = (APP_Rx_ptr_in + 1) % APP_RX_DATA_SIZE;

        while (CDC_Send_FreeBytes() == 0) {
            delay(1);
        }
    }

    return USBD_OK;
}

/*******************************************************************************
 * Function Name  : Receive DATA .
 * Description    : receive the data from the PC to STM32 and send it through USB
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
uint32_t CDC_Receive_DATA(uint8_t* recvBuf, uint32_t len)
{
    uint32_t count = 0;

    while (APP_Tx_ptr_out != APP_Tx_ptr_in && count < len) {
        recvBuf[count] = APP_Tx_Buffer[APP_Tx_ptr_out];
        APP_Tx_ptr_out = (APP_Tx_ptr_out + 1) % APP_TX_DATA_SIZE;
        count++;
    }
    return count;
}

uint32_t CDC_Receive_BytesAvailable(void)
{
    /* return the bytes available in the receive circular buffer */
    return APP_Tx_ptr_out > APP_Tx_ptr_in ? APP_TX_DATA_SIZE - APP_Tx_ptr_out + APP_Tx_ptr_in : APP_Tx_ptr_in - APP_Tx_ptr_out;
}

/**
 * @brief  VCP_DataRx
 *         Data received over USB OUT endpoint are sent over CDC interface
 *         through this function.
 *
 *         @note
 *         This function will block any OUT packet reception on USB endpoint
 *         until exiting this function. If you exit this function before transfer
 *         is complete on CDC interface (ie. using DMA controller) it will result
 *         in receiving more data while previous ones are still not sent.
 *
 * @param  Buf: Buffer of data to be received
 * @param  Len: Number of data received (in bytes)
 * @retval Result of the opeartion: USBD_OK if all operations are OK else VCP_FAIL
 */
static uint16_t VCP_DataRx(uint8_t* Buf, uint32_t Len)
{
    if (CDC_Receive_BytesAvailable() + Len > APP_TX_DATA_SIZE) {
        return USBD_FAIL;
    }

    for (uint32_t i = 0; i < Len; i++) {
        APP_Tx_Buffer[APP_Tx_ptr_in] = Buf[i];
        APP_Tx_ptr_in = (APP_Tx_ptr_in + 1) % APP_TX_DATA_SIZE;
    }

    return USBD_OK;
}

static uint16_t VCP_COMConfig(uint8_t Conf)
{
//  USART_InitTypeDef USART_InitStructure;
//
//  if (Conf == DEFAULT_CONFIG)
//  {
//    /* Configure and enable the USART */
//    STM_EVAL_COMInit();
//  }
//  else
//  {
//    /* set the Stop bit*/
//    switch (linecoding.format)
//    {
//      case 0:
//      USART_InitStructure.USART_StopBits = USART_StopBits_1;
//      break;
//      case 1:
//      USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
//      break;
//      case 2:
//      USART_InitStructure.USART_StopBits = USART_StopBits_2;
//      break;
//      default :
//      VCP_COMConfig(DEFAULT_CONFIG);
//      return (USBD_FAIL);
//    }
//
//    /* set the parity bit*/
//    switch (linecoding.paritytype)
//    {
//      case 0:
//      USART_InitStructure.USART_Parity = USART_Parity_No;
//      break;
//      case 1:
//      USART_InitStructure.USART_Parity = USART_Parity_Even;
//      break;
//      case 2:
//      USART_InitStructure.USART_Parity = USART_Parity_Odd;
//      break;
//      default :
//      VCP_COMConfig(DEFAULT_CONFIG);
//      return (USBD_FAIL);
//    }
//
//    /*set the data type : only 8bits and 9bits is supported */
//    switch (linecoding.datatype)
//    {
//      case 0x07:
//      /* With this configuration a parity (Even or Odd) should be set */
//      USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//      break;
//      case 0x08:
//      if (USART_InitStructure.USART_Parity == USART_Parity_No)
//      {
//        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//      }
//      else
//      {
//        USART_InitStructure.USART_WordLength = USART_WordLength_9b;
//      }
//
//      break;
//      default :
//      VCP_COMConfig(DEFAULT_CONFIG);
//      return (USBD_FAIL);
//    }
//
//    USART_InitStructure.USART_BaudRate = linecoding.bitrate;
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//
//    /* Configure and enable the USART */
//    STM_EVAL_COMInit();
//  }
    return USBD_OK;
}

/**
* @brief  EVAL_COM_IRQHandler
* @param  None.
* @retval None.
*/
//void EVAL_COM_IRQHandler(void)
//{
//  if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//  {
//    /* Send the received data to the PC Host*/
//    VCP_DataTx (0,0);
//  }
//
//  /* If overrun condition occurs, clear the ORE flag and recover communication */
//  if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
//  {
//    (void)USART_ReceiveData(USART2);
//  }
//}


///**
//* @brief  Configures COM port.
//* @param  COM: Specifies the COM port to be configured.
//*          This parameter can be one of following parameters:
//*            @arg COM1
//* @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that
//*         contains the configuration information for the specified USART peripheral.
//* @retval None
//*/
//static void STM_EVAL_COMInit(void)
//{
//  GPIO_InitTypeDef GPIO_InitStructure;
//  USART_InitTypeDef USART_InitStructure;
//  NVIC_InitTypeDef NVIC_InitStructure;
//
//  /* Enable GPIO clock */
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
//  /* Enable USART clock */
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//
//  /* Connect PXx to USARTx_Tx */
//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
//
//  /* Connect PXx to USARTx_Rx */
//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
//
//  /* Configure USART Tx as alternate function push-pull */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//
//  /* Configure USART Rx as alternate function push-pull */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//
//  /* EVAL_COM1 default configuration */
//  /* EVAL_COM1 configured as follow:
//  - BaudRate = 115200 baud
//  - Word Length = 8 Bits
//  - One Stop Bit
//  - Parity Odd
//  - Hardware flow control disabled
//  - Receive and transmit enabled
//  */
//  USART_InitStructure.USART_BaudRate = 115200;
//  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//  USART_InitStructure.USART_StopBits = USART_StopBits_1;
//  USART_InitStructure.USART_Parity = USART_Parity_No;
//  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//
//
//  /* USART configuration */
//  USART_Init(USART2, &USART_InitStructure);
//
//  /* Enable USART */
//  USART_Cmd(USART2, ENABLE);
//  USART_ClearFlag(USART2, USART_FLAG_TC);
//  /* Enable the USART Receive interrupt */
////  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//
//  /* Enable USART Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPriority = USART_IT_PRIO;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
