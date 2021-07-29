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
	uint16_t count;               // Data Count
	uint8_t  status;              // Buffer status
	char*    ptr;                 // Buffer pointer

}Tx_Struct;

Tx_Struct uart_t;

/*****************************************************************************/
/*! @fn       uart_tx
 *  @brief    Sends data to the USART3 data buffer
 *  @param    Pointer to the data to be sent
 *  @return   ret -  0 for success and -1 for failure
 */
/*****************************************************************************/

char uart_tx(char* command, uint8_t cmd_length, USART_TypeDef *uart){

	uint8_t return_status = PASS;

	uart_rx_clear();

	// if command ready to transfer
	if(uart_t.status == TX_FREE){

		// set tx_status busy
		uart_t.status = TX_BUSY;
		// copy command into tx_buffer
		uart_t.count = cmd_length;
		// store command address
		uart_t.ptr = command;
		// transfer byte
		uart->TDR = uart_t.ptr[0]; 		// initiate interrupt driven transfer

		uart_t.ptr++;
		uart_t.count--;

		// enable transfer interrupt
		uart->CR1 |= USART_CR1_TXEIE;
	}else{

		return_status = FAIL;	// command fail
	}

	return return_status;
}

// Make generic for all uart peripherals (add buffer argument)
void uart_rx_clear(void){

	rx_idx = BUFF_RESET;

	uint16_t i = BUFF_RESET;

	while(i < BUFF_MAX - 1){
		rx_buff[i] = NULL;
		i++;
	}

}


uint8_t uart_rx_check(char* needle, uint8_t needle_size, uint16_t test_cnt){

	while(test_cnt)
	{
		HAL_Delay(UART_RX_DELAY);

		if( uart_rx_find(needle, needle_size) ) // uart_rx_find returns nonzero if needle found
		{
			return PASS;
		}

		test_cnt--;

	}

	return FAIL;

}

uint16_t uart_rx_find(char* needle, uint8_t needle_size){	// return index to end of needle, else 0

	uint16_t i = 0;
	uint16_t buff_idx = 0;

	while (buff_idx < BUFF_MAX) {

		// check for complete needle
		while (needle[i] == rx_buff[buff_idx]) {
			i++;
			buff_idx++;
		}

		if (i == needle_size) {
			return buff_idx; // needle found
		}

		i = 0; // reset needle index
		buff_idx++;

	}

	return FAIL; // needle not found

}



/*----------------------------------------------------------------------------
  Uart_isr
  Handles generic USART global interrupt request.
 *----------------------------------------------------------------------------*/
void uart_isr(USART_TypeDef *uart){	// add buffer argument

	// if receive buffer ready to read
	if( uart->ISR & USART_ISR_RXNE ){

		// store into buffer (testing)
		rx_buff[rx_idx] = uart->RDR;

		// return echo received byte
		PC_UART->TDR = rx_buff[rx_idx];

		rx_idx++;
	}

	// if ready to transfer
	if( uart->ISR & USART_ISR_TC ){

		if(uart_t.count > 0){
			uart->TDR = *uart_t.ptr;
			uart_t.ptr++;
			uart_t.count--;
		}else{
			uart->ICR |= USART_ICR_TCCF; // testing to clear TC bit
			uart->CR1 &= ~USART_CR1_TXEIE;	// disable TX interupt when message complete
			uart_t.status = TX_FREE;
		}
	}

}




















