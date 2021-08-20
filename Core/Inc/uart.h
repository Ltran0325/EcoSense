/*****************************************************************************/
/*!
 * @project    EcoSense
 * @file       uart.h
 * @author     Long Tran
 * @version    0.0.1
 * @brief	   UART handler
 * @date       28/July/2021
 * @bug        NA

 * @note
 */
/*****************************************************************************/

/******************** INCLUDE FILES ******************************************/
#include "stdint.h"
#include "stm32l476xx.h"
/******************** DEFINE MACROS ******************************************/

#define TX_FREE                  		 (uint8_t)0
#define TX_BUSY                  		 (uint8_t)1

#define PASS 0
#define FAIL 1

#define BUFF_MAX 3*1024
#define BUFF_RESET 0

#define WIFI_UART   USART1
#define PC_UART     USART2
#define CAMERA_UART USART3
#define LTEGPS_UART UART4

#define UART_DELAY      20
#define UART_1S_TIMEOUT 1000/UART_DELAY

/******************** DEFINE GLOBAL VARIABLES  *******************************/
// generic uart return buffer
char rx_buff[BUFF_MAX];
volatile uint16_t rx_idx;

/******************** DEFINE STRUCT ******************************************/




/******************** FUNCTION DECLARATION************************************/

/*****************************************************************************/
/*! @fn       uart_tx
 *  @brief    Sends data to the USART3 data buffer
 *  @param    Pointer to the data to be sent, data length, USART handler
 */
/*****************************************************************************/
void uart_tx(char* cmd, uint8_t cmd_length, USART_TypeDef *uart);

/*****************************************************************************/
/*! @fn       uart_rx_print
 *  @brief    Print uart response buffer to PC
 *  @return   ret -  0 for success and -1 for failure
 */
/*****************************************************************************/
char uart_rx_print(void);

/*****************************************************************************/
/*! @fn       uart_rx_flush
 *  @brief    Resets uart receive buffer and index
 */
/*****************************************************************************/
void uart_rx_flush(void);

/*****************************************************************************/
/*! @fn        uart_rx_check
 *   @brief    Calls uart_rx_find until success or timeout
 *   @param    needle to scanned for in rx_buff, size of needle,
 *   		   times to call UART_Rx_Find
 *  @return    ret -  0 for success and -1 for failure
 */
/*****************************************************************************/
uint8_t uart_rx_check(char* needle, uint8_t needle_size, uint16_t test_cnt);

/*****************************************************************************/
/*! @Function Name: uart_rx_find
 *  @brief        : This function finds needle inside rx_buffer.
 *  @return		  : index to end of needle, used for rx_idx
 *  				else 0
 */
/*****************************************************************************/
uint16_t uart_rx_find(char* needle, uint8_t needle_size);

/*****************************************************************************/
/*! @fn       LOG
 *  @brief    Log message to PC
 *  @param    Pointer to the data to be sent
 */
/*****************************************************************************/
char LOG(char* message);

/*****************************************************************************/
/*! @fn       LOG_BOX
 *  @brief    Log message to PC between two rows of asterisks.
 *  @param    Pointer to the data to be sent
 */
/*****************************************************************************/
void LOG_BOX(char* message);

/*****************************************************************************/
/*! @Function Name: uart_isr
 *  @brief        : Handles generic USART global interrupt request
 */
/*****************************************************************************/
void uart_isr(USART_TypeDef *uart);

