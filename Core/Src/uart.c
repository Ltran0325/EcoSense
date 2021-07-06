/*****************************************************************************/
/*!
 * @company    Autosense Internation Inc.
 * @file       uart.c
 * @author     Long Tran
 * @version    1.0
 * @brief	   Debug UART handler
 * @date       12/May/2018
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
Tx_Struct AT_Tx;

// return buffer for USART3

char RX_Buffer[BUFF_MAX];
uint16_t RX_idx;


/*****************************************************************************/
/*! @fn       AT_command_IT
 *  @brief    Sends data to the USART3 data buffer
 *  @param    Pointer to the data to be sent
 *  @return   ret -  0 for success and -1 for failure
 */
/*****************************************************************************/

char AT_command_IT(char* command, uint8_t cmd_length){

	uint8_t return_status = RET_PASS;

	RX_clear();

	// if command ready to transfer
	if(AT_Tx.status == TX_FREE){

		// set tx_status busy
		AT_Tx.status = TX_BUSY;
		// copy command into tx_buffer
		AT_Tx.count = cmd_length;
		// store command address
		AT_Tx.ptr = command;
		// transfer byte
		USART3->TDR = AT_Tx.ptr[0]; 		// initiate interrupt driven transfer
		AT_Tx.ptr++;
		AT_Tx.count--;

		// enable transfer interrupt
		USART3->CR1 |= USART_CR1_TXEIE;
	}else{

		return_status = RET_FAIL;	// command fail
	}

	return return_status;
}

void RX_clear(void){

	RX_idx = BUFF_RESET;

	uint16_t i = BUFF_RESET;

	while(i < BUFF_MAX - 1){
		RX_Buffer[i] = NULL;
		i++;
	}

}

uint8_t RX_parse(char* needle, uint8_t needle_size){	// return index to end of needle, else 0

	uint16_t haystack_size = BUFF_MAX;
	uint8_t i = 0;

	while (haystack_size >= 0) {

		// check for complete needle
		while (needle[i] == RX_Buffer[i] && i < needle_size) {
			i++;
		}

		if (i == needle_size) {
			return i; // needle found
		}

		i = 0; // reset needle index

		haystack_size--;
	}

	return 0;

}

/*----------------------------------------------------------------------------
  USART3_IRQHandler
  Handles USART3 global interrupt request.
 *----------------------------------------------------------------------------*/
void USART3_IRQHandler(void)
{
	// if receive buffer ready to read
	if( USART3->ISR & USART_ISR_RXNE ){

		// store into buffer (testing)
		RX_Buffer[RX_idx] = USART3->RDR;

		// return echo received byte
		USART2->TDR = RX_Buffer[RX_idx];

		RX_idx++;
	}

	// if ready to transfer
	if( USART3->ISR & USART_ISR_TC ){

		if(AT_Tx.count > 0){
			USART3->TDR = *AT_Tx.ptr;
			AT_Tx.ptr++;
			AT_Tx.count--;
		}else{
			USART3->CR1 &= ~USART_CR1_TXEIE;	// disable TX interupt when message complete
			AT_Tx.status = TX_FREE;
		}
	}

}
