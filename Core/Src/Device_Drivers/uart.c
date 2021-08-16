/*****************************************************************************/
/*!
 * @project    EcoSense
 * @file       uart.c
 * @author     Long Tran
 * @version    1.1
 * @brief	   UART handler
 * @date       28/July/2021
 * @bug        NA

 * @note
 */
/*****************************************************************************/

/******************** INCLUDE FILES ******************************************/
#include "string.h"
#include "uart.h"
#include "stdint.h"
#include "stm32l4xx_hal.h"

/******************** GLOBAL VARIABLE ****************************************/
/* Structure for UART TX transmission */

/* Structure for UART Tx */
typedef struct
{
	volatile uint16_t count;               // Data Count
	volatile uint8_t  status;              // Buffer status
	char*    ptr;                 // Buffer pointer

}Tx_Struct;

Tx_Struct uart_t;

/******************** FUNCTION DECLARATION************************************/

/*****************************************************************************/
/*! @fn       uart_tx
 *  @brief    Sends data to the USART3 data buffer
 *  @param    Pointer to the data to be sent
 */
/*****************************************************************************/
void uart_tx(char* cmd, uint8_t cmd_length, USART_TypeDef *uart){

	uart_rx_flush();	// Reset

	uart_t.ptr = cmd;			// Load new command
	uart_t.count = cmd_length;  // and command length
	uart->TDR = *uart_t.ptr & 0xFF; // Writing to TDR clears TX

	uart->CR1 |= USART_CR1_TXEIE; // Initiate USART Tx interrupt

}

/*****************************************************************************/
/*! @fn       uart_rx_print
 *  @brief    Print uart response buffer to PC for debug purposes.
 *  @return   ret -  0 for success and -1 for failure
 */
/*****************************************************************************/
char uart_rx_print(void){

	uint16_t i;

	for(i = 0; i < rx_idx; i++){
		while(!(PC_UART->ISR & USART_ISR_TXE));	// Wait until hardware sets TXE
		PC_UART->TDR = rx_buff[i] & 0xFF;		// Writing to TDR clears TXE flag
	}

	//while(! (PC_UART->ISR & USART_ISR_TC));	// Wait until TC: transmission complete

	//PC_UART->ICR |= USART_ICR_TCCF;			// Clear TCCF: transmission complete clear flag
}

/*****************************************************************************/
/*! @fn       uart_rx_flush
 *  @brief    Resets uart receive buffer and index
 */
/*****************************************************************************/
void uart_rx_flush(void){

	uint16_t i;

	for(i = 0; i < rx_idx; i++){
		rx_buff[i] = NULL;
	}

	rx_idx = BUFF_RESET;
}

/*****************************************************************************/
/*! @fn        uart_rx_check
 *   @brief    calls uart_rx_find until success or timeout
 *   @param    needle to scanned for in rx_buff, size of needle,
 *   		   times to call UART_Rx_Find
 *  @return    ret -  0 for success and 1 for failure
 */
/*****************************************************************************/
uint8_t uart_rx_check(char* needle, uint8_t needle_size, uint16_t test_cnt){

	while(test_cnt)
	{
		HAL_Delay(UART_DELAY);

		if( uart_rx_find(needle, needle_size) )
		{
			return PASS;
		}

		test_cnt--;

	}

	return FAIL;

}

/*****************************************************************************/
/*! @Function Name: uart_rx_find
 *  @brief        : This function finds needle inside rx_buffer.
 *  @return		  : index to end of needle, used for rx_idx
 *  				else 0
 */
/*****************************************************************************/
uint16_t uart_rx_find(char* needle, uint8_t needle_size){	// return index to end of needle, else 0

	uint16_t i = 0;
	uint16_t buff_idx = 0;

	while (buff_idx <= rx_idx) {

		// check for complete needle
		while (needle[i] == rx_buff[buff_idx] && i < needle_size) {
			i++;
			buff_idx++;
		}

		if (i == needle_size) {
			return buff_idx; // needle found
		}

		i = 0; // reset needle index
		buff_idx++;

	}

	return 0; // needle not found

}

/*****************************************************************************/
/*! @fn       LOG
 *  @brief    Log message to PC
 *  @param    Pointer to the data to be sent
 */
/*****************************************************************************/
char LOG(char* message){

	uint16_t i;

	for(i = 0; i < strlen(message) ; i++){
		while(!(PC_UART->ISR & USART_ISR_TXE));	// Wait until hardware sets TXE
		PC_UART->TDR = message[i] & 0xFF;		// Writing to TDR clears TXE flag
	}
#if 0
	while(! (PC_UART->ISR & USART_ISR_TC));	// Wait until TC: transmission complete

	PC_UART->ICR |= USART_ICR_TCCF;			// Clear TCCF: transmission complete clear flag
#endif
}

/*****************************************************************************/
/*! @fn       LOG_BOX
 *  @brief    Log message to PC between two rows of asterisks.
 *  @param    Pointer to the data to be sent
 */
/*****************************************************************************/
void LOG_BOX(char* message)
{
	LOG("\r\n************************************************\r\n");
	LOG(message);
	LOG("\r\n************************************************\r\n");
}

/*****************************************************************************/
/*! @Function Name: uart_isr
 *  @brief        : Handles generic USART global interrupt request
 */
/*****************************************************************************/
void uart_isr(USART_TypeDef *uart){

	// if receive buffer ready to read
	if( uart->ISR & USART_ISR_RXNE ){ // Check RXNE event
		rx_buff[rx_idx] = uart->RDR;  // Reading RDR clears RXNE flag
		rx_idx++;					  // Update buffer index
		if(rx_idx >= BUFF_MAX){		  // Circular buffer
			rx_idx = BUFF_RESET;
		}
	}

	// if ready to transfer
	if( uart->ISR & USART_ISR_TC ){		// Check TXE event
		if(uart_t.count > 0){				// Transmit next byte
			uart_t.ptr++;
			uart_t.count--;
			uart->TDR = *uart_t.ptr & 0xFF; // Writing to TDR clears TC
		}else{
			uart->CR1 &= ~USART_CR1_TXEIE;	// Disable TXE interrupt
			uart->ICR |= USART_ICR_TCCF;
		}
	}

}




















