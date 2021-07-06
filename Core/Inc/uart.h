/*****************************************************************************/
/*!
 * @company    Autosense Internation Inc.
 * @file       uart.h
 * @author     Long Tran
 * @version    1.0
 * @brief	   Debug UART handler
 * @date       12/May/2018
 * @bug        NA

 * @note
 */
/*****************************************************************************/
/******************** INCLUDE FILES ******************************************/
#include "stdint.h"
/******************** DEFINE MACROS ******************************************/
#define TX_BUSY                  		 (uint8_t)1
#define TX_FREE                  		 (uint8_t)0
#define RET_PASS                  		 0
#define RET_FAIL                  		-1

#define BUFF_MAX 3000
#define BUFF_RESET 0

/******************** DEFINE STRUCT ***************************************/
/* Structure for UART Rx & Tx */
typedef struct
{
	uint16_t count;               // Data Count
	uint8_t  status;              // Buffer status
	char*    ptr;                 // Buffer pointer
}Tx_Struct;


/******************** FUNCTION DECLARATION************************************/
char AT_command_IT(char* command, uint8_t cmd_length);
uint8_t RX_parse(char* needle, uint8_t needle_size);
void RX_clear(void);

/******************** HANDLER DECLARATION************************************/
void USART3_IRQHandler(void);
