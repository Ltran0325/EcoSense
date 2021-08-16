/*****************************************************************************/
/*!
 * @project    EcoSense
 * @file       api_wifi.c
 * @author     Long Tran
 * @version    1.1
 * @brief	   Wi-Fi handler
 * @date       28/July/2021
 * @bug        NA

 * @note
 */
/*****************************************************************************/

/******************** INCLUDE FILES ******************************************/
#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#include "stm32l476xx.h"
#include "api_wifi.h"
#include "uart.h"
/******************** DEFINE ENUMS and STRUCT ********************************/



/******************** DEFINE GLOBAL VARIABLES  *******************************/
uint8_t strAP_idx = 0;

WiFi_Struct AP_1 = { "NETGEAR35 - 5G", "Doe" , RSSI_DEFAULT };

WiFi_Struct AP_2 = { "Team_a_live", "PassWord", RSSI_DEFAULT };

WiFi_Struct AP_3 = { "Trans 5G", "2232portal", RSSI_DEFAULT };

WiFi_Struct* AP_List_Known[3] = { &AP_1, &AP_2, &AP_3 };
/******************** FUNCTION DECLARATION************************************/

/******************** WI-FI APPLICATION FUNCTIONS START **********************/
/*****************************************************************************/
/*! @Function Name: api_wifi_connect
 *  @brief        : High level function to issue command sequence to allow
 *  				ping to google.com
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_wifi_connect(void){

	HAL_Delay(20);
	if( api_wifi_echodisable() ){
		return FAIL;
	}

	HAL_Delay(20);
	if( api_wifi_station() ){
		return FAIL;
	}

	HAL_Delay(20);
	if( api_wifi_scan() ){
		return FAIL;
	}

	HAL_Delay(20);
	if( api_wifi_known() ){
		return FAIL;
	}

	return PASS;
}

/*****************************************************************************/
/*! @Function Name: api_wifi_ping
 *  @brief        : High level function to ping to google.com
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_wifi_ping(void){

	HAL_Delay(20);
	uart_tx(AT_ping, strlen(AT_ping), WIFI_UART);

	if( uart_rx_check(Resp_WIFI_SUCCESS, strlen(Resp_WIFI_SUCCESS), 10 * UART_1S_TIMEOUT) ){
		LOG("ERROR: Packets returned unsuccessfully.\r\n");
		uart_rx_print();
		return FAIL;
	}

	uart_rx_print();
	return PASS;
}

/******************** WI-FI APPLICATION FUNCTIONS END ************************/

/******************** WI-FI API START ****************************************/

/*****************************************************************************/
/*! @Function Name: api_wifi_station
 *  @brief        : Station mode command of USART Wi-Fi module. This command
 *  				sets Wi-Fi module in Station mode.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_wifi_station(void){

	uart_tx(AT_station, strlen(AT_station), WIFI_UART);

	if( uart_rx_check(Resp_WIFI_OK, strlen(Resp_WIFI_OK), UART_1S_TIMEOUT) ){
		LOG("ERROR: No response.\r\n");
		uart_rx_print();
		return FAIL;
	}

	uart_rx_print();
	return PASS;
}

/*****************************************************************************/
/*! @Function Name: api_wifi_scan
 *  @brief        : AP scan command of USART Wi-Fi module. This command
 *  				scans for nearby AP and parses for known AP RSSI values.
 *  				This function should be called before WM_KnownConnect.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_wifi_scan(void){

	uart_tx(AT_scan, strlen(AT_scan), WIFI_UART);

	HAL_Delay(3000);

	if( uart_rx_check(Resp_WIFI_OK, strlen(Resp_WIFI_OK), 20 * UART_1S_TIMEOUT) ){
		LOG("ERROR: No response.\r\n");
		uart_rx_print();
		return FAIL;
	}

	uart_rx_print();

	if( api_wifi_scanparse() ){
		LOG("ERROR: Known AP(s) not found.\r\n");
		uart_rx_print();
		return FAIL;
	}else{
		LOG("Known AP(s) found.\r\n");
		uart_rx_print();
		return PASS;
	}

}

/*****************************************************************************/
/*! @Function Name: api_wifi_scanparse
 *  @brief        : This function is used by api_wifi_scan function to parse for
 *  				AP RSSI values.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_wifi_scanparse(void)
{

	char Status = FAIL;
	char *str = NULL;
	const char s[2] = ",";
	char *token;
	uint8_t i = 0;

	str = rx_buff;

	token = strtok(str, s);

	// get RSSI of known AP
	while(token != NULL) {

		// if token is an SSID
		if(*token == '\"') {

			// check if SSID is in known AP list
			for(i = 0; i < AP_KNOWN_COUNT; i++) {

				// nonzero if SSID match
				if(strstr(token, AP_List_Known[i]->SSID)) {

					// go to RSSI value
					while (*token != '-') {
						token = strtok(NULL, s);
					}
					// store RSSI value
					token++;
					AP_List_Known[i]->RSSI = atoi(token);
					Status = PASS;	// Known AP found
				}
			}
		}

		// move to next token
		token = strtok(NULL, s);
	}

    // get index of strongest AP
    for (i = 1; i < AP_KNOWN_COUNT; i++) {

        if (AP_List_Known[strAP_idx]->RSSI > AP_List_Known[i]->RSSI) {
            strAP_idx = i;
        }

    }

	return Status;
}


/*****************************************************************************/
/*! @Function Name: api_wifi_known
 *  @brief        : Known AP connect command of USART Wi-Fi module. This command
 *  				connects to strongest known AP.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_wifi_known(void){

	uart_tx(AT_connect, strlen(AT_connect), WIFI_UART);

	if( uart_rx_check(Resp_WIFI_OK, strlen(Resp_WIFI_OK), 6 * UART_1S_TIMEOUT) ){
		LOG("ERROR: Wi-Fi connection may already be established.\r\n");
		uart_rx_print();
		return FAIL;
	}

	uart_rx_print();
	return PASS;
}

/*****************************************************************************/
/*! @Function Name: api_wifi_station
 *  @brief        : Echo disable command of USART Wi-Fi module.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_wifi_echodisable(void){

	uart_tx(AT_echodisable, strlen(AT_echodisable), WIFI_UART);

	if( uart_rx_check(Resp_WIFI_OK, strlen(Resp_WIFI_OK), UART_1S_TIMEOUT) ){
		LOG("ERROR: No response.\r\n");
		uart_rx_print();
		return FAIL;
	}

	uart_rx_print();
	return PASS;
}

/*****************************************************************************/
/*! @Function Name: api_wifi_check
 *  @brief        : Check for return response.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_wifi_check(void){

	uart_tx(AT_check, strlen(AT_check), WIFI_UART);

	if( uart_rx_check(Resp_WIFI_OK, strlen(Resp_WIFI_OK), UART_1S_TIMEOUT) ){
		LOG("ERROR: No response.\r\n");
		uart_rx_print();
		return FAIL;
	}

	uart_rx_print();
	return PASS;
}
/******************** WI-FI API END ******************************************/
