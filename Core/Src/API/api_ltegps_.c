/*****************************************************************************/
/*!
 * @project    EcoSense
 * @file       api_wifi.c
 * @author     Long Tran
 * @version    0.0.1
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
#include "api_ltegps.h"
#include "uart.h"

/******************** DEFINE GLOBAL VARIABLES  *******************************/


/******************** FUNCTION DECLARATION************************************/


/******************** LTEGPS APPLICATION FUNCTIONS START *********************/

/*****************************************************************************/
/*! @Function Name: api_ltegps_gpsconnect
 *  @brief        : High level function to issue command sequence to
 *  				connect to LTE. The EVB requires 20 ms
 *  				delay between command response and next command.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_lteconnect(void){

	LOG_BOX("\r\nBeginning LTE connection sequence.\r\n");

	HAL_Delay(20);
	if( api_ltegps_fwswitch() ){
		//return FAIL;
	}

	HAL_Delay(20);
	if( api_ltegps_signalquality() ){
		return FAIL;
	}

	HAL_Delay(20);
	if( api_ltegps_pdpset() ){
		return FAIL;
	}

	HAL_Delay(20);
	if( api_ltegps_pdpavailable() ){
		return FAIL;
	}

	HAL_Delay(20);
	if( api_ltegps_wdsselect() ){
		return FAIL;
	}

	HAL_Delay(20);
	if( api_ltegps_epsmode() ){
		return FAIL;
	}

	HAL_Delay(20);
	if( api_ltegps_pdpactivate() ){
		return FAIL;
	}

	LOG_BOX("\r\nSUCCESS: LTE connection successful.\r\n");

	return PASS;

}

/*****************************************************************************/
/*! @Function Name: api_ltegps_gpsconnect
 *  @brief        : High level function to ping www.google.com via LTE.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_lteping(void){

	LOG_BOX("SEND: Ping to www.google.com");

	uart_tx(lteping, strlen(lteping), LTEGPS_UART);

	uint32_t timeout = 0;

	while( timeout < 30 * UART_1S_TIMEOUT){

		HAL_Delay(UART_DELAY); // 20 ms delay

		if( uart_rx_find(Resp_LTEGPS_Ping, strlen(Resp_LTEGPS_Ping) ) ){
			uart_rx_print();
			return PASS;
		}

		if( uart_rx_find(Resp_LTEGPS_ERROR, strlen(Resp_LTEGPS_ERROR) ) ){
			LOG("ERROR: Ping failed.\r\n");
			uart_rx_print();
			return PASS;
		}

		timeout += UART_DELAY;
	}

		uart_rx_print();
		LOG("ERROR: No response.\r\n");
		return FAIL;

}

/*****************************************************************************/
/*! @Function Name: api_ltegps_gpsconnect
 *  @brief        : High level function to issue command sequence to
 *  				start NMEA data stream and parse. The EVB requires 20 ms
 *  				delay between command response and next command.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_gpsconnect(void){

	LOG_BOX("\r\nBeginning GPS connection sequence.\r\n");

	HAL_Delay(20);
	if( api_ltegps_echodisable() ){
		return FAIL;
	}

	HAL_Delay(20);
	if( api_ltegps_endnmea() ){
		return FAIL;
	}

	HAL_Delay(20);
	if( api_ltegps_selectgnss() ){
		return FAIL;
	}

	HAL_Delay(20);
	if( api_ltegps_powergnss() ){
		return FAIL;
	}

	HAL_Delay(20);
	if( api_ltegps_startnmea() ){
		return FAIL;
	}

	HAL_Delay(20);
	if( api_ltegps_endnmea() ){
		return FAIL;
	}

	LOG_BOX("\r\nSUCCESS: GPS data succesfully retrieved.\r\n");

	return PASS;
}

/******************** LTEGPS APPLICATION FUNCTIONS END ***********************/

/******************** LTE API START ******************************************/

/*****************************************************************************/
/*! @Function Name: api_ltegps_fwswitch
 *  @brief        : Set firmware image command of USART LTEGPS module.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_fwswitch(void){

	LOG_BOX("SEND: F/W Switch to Verizon");

	uart_tx(fwswitch, strlen(fwswitch), LTEGPS_UART);

	if( uart_rx_check(Resp_LTEGPS_OK, strlen(Resp_LTEGPS_OK), UART_1S_TIMEOUT) ){
		LOG("ERROR: No response.\r\n");
		uart_rx_print();
		return FAIL;
	}

	uart_rx_print();
	return PASS;

}

/*****************************************************************************/
/*! @Function Name: api_ltegps_signalquality
 *  @brief        : Check signal quality command of USART LTEGPS module.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_signalquality(void){

	LOG_BOX("SEND: Signal quality test");

	uart_tx(signalquality, strlen(signalquality), LTEGPS_UART);

	if( uart_rx_check(Resp_LTEGPS_OK, strlen(Resp_LTEGPS_OK), UART_1S_TIMEOUT) ){
		LOG("ERROR: No response.\r\n");
		uart_rx_print();
		return FAIL;
	}

	if( api_ltegps_signalqualitycheck() ){
		LOG("ERROR: Weak tower signal\r\n");
		uart_rx_print();
		return FAIL;
	}

	uart_rx_print();
	return PASS;

}

/*****************************************************************************/
/*! @Function Name: api_ltegps_signalqualitycheck
 *  @brief        : Check rsrq and rsrp values of LGM_SignalQuality response.
 *  				Both values should be greater than 5 but should not be 255.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_signalqualitycheck(void){

	// +CESQ: 99,99,255,255,<rsrq>,<rsrp>
	// +CESQ: 99,99,255,255,19,55

	char Status = PASS ;
	char *str = NULL;
	const char s[2] = ",";
	char *token;

	str = rx_buff;

	token = strtok(str, s);

	while( token != NULL){

        // if token is CESQ response
        if (strstr(token, "+CESQ: 99")) {

        	token = strtok(NULL, s);
        	token = strtok(NULL, s);
        	token = strtok(NULL, s);
        	token = strtok(NULL, s);

        	// error is rsrq is 255
            if(strstr(token, "255")){
            	return FAIL;
            }

            // error is rsrp is 255
        	token = strtok(NULL, s);
        	if(strstr(token, "255")){
        		return FAIL;
            }
        }

    token = strtok(NULL, s);

	}

	return Status;

}

/*****************************************************************************/
/*! @Function Name: api_ltegps_pdpset
 *  @brief        : Set PDP context command of USART LTEGPS module.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_pdpset(void){

	LOG_BOX("SEND: Set PDP context");

	uart_tx(pdpset, strlen(pdpset), LTEGPS_UART);

	if( uart_rx_check(Resp_LTEGPS_OK, strlen(Resp_LTEGPS_OK), UART_1S_TIMEOUT) ){
		LOG("ERROR: No response.\r\n");
		uart_rx_print();
		return FAIL;
	}

	uart_rx_print();
	return PASS;

}

/*****************************************************************************/
/*! @Function Name: LGM_PDPAvailable
 *  @brief        : Check available PDP context type command of USART LTEGPS module.
 *  				This function also parses for Verizon CID value.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_pdpavailable(void){

	char cid = 0;

	uart_tx(pdpavailable, strlen(pdpavailable), LTEGPS_UART);

	if( uart_rx_check(Resp_LTEGPS_OK, strlen(Resp_LTEGPS_OK), UART_1S_TIMEOUT) ){
		LOG("ERROR: No response.\r\n");
		uart_rx_print();
		return FAIL;
	}

	// scan for CID value of "vzwadmin"
	cid = api_ltegps_pdpavailableparse("vzwinternet");

	// cid must be from 1 to max
	if(cid == '0'){
		uart_rx_print();
		return FAIL;
	}

	pdpactivate[9] = cid;

	uart_rx_print();

	return PASS;

}

/*****************************************************************************/
/*! @Function Name: api_ltegps_pdpavailableparse
 *  @brief        : This function will parse for the CID value of "vzwinternet"
 *  				from api_ltegps_pdpavailable.
 *  @param        : apn, ex. "vzwadmin"
 *  @return       : CID of apn or 0 if fail
 */
/*****************************************************************************/
char api_ltegps_pdpavailableparse(char* apn)
{
	/*
	   AT+CGDCONT?
	   +CGDCONT: 1,"IPV4V6","","0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0",0,0,0,0
	   +CGDCONT: 2,"IPV4V6","vzwadmin","0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0",0,0,0,0
	   +CGDCONT: 3,"IPV4V6","vzwinternet","0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0",0,0,0,0
	   +CGDCONT: 4,"IPV4V6","vzwapp","0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0",0,0,0,0
	   +CGDCONT: 5,"IPV4V6","vzw800","0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0",0,0,0,0
	   +CGDCONT: 6,"IPV4V6","vzwclass6","0.0.0.0.0.0.0.0.0.0.0.0.0.0.0.0",0,0,0,0
	   OK
	*/

	// Blank AP at CID = 1 is necessary
	if ( !uart_rx_find(",\"\",", strlen(",\"\",") ) ){
		return '0'; // ERROR
	}
	char *str = NULL;
	const char s[2] = ",";
	char *token;
	char cid = '0';

	str = rx_buff;

	token = strtok(str, s);

	while( token != NULL){


        if (strstr(token, "+CGDCONT: ")) {
        	cid++;
        }


        if (strstr(token, apn)) {
        	return cid;
        }
        token = strtok(NULL, s);

	}

	return '0';
}

/*****************************************************************************/
/*! @Function Name: api_ltegps_wdsselect
 *  @brief        : Set WDS setting to EU-TRAN (28).
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_wdsselect(void){

	LOG_BOX("SEND: Set WDS setting to EU-TRAN (28)");

	uart_tx(wdsselect, strlen(wdsselect), LTEGPS_UART);

	if( uart_rx_check(Resp_LTEGPS_OK, strlen(Resp_LTEGPS_OK), UART_1S_TIMEOUT) ){
		LOG("ERROR: No response.\r\n");
		uart_rx_print();
		return FAIL;
	}

	uart_rx_print();
	return PASS;

}

/*****************************************************************************/
/*! @Function Name: api_ltegps_epsmode
 *  @brief        : Set EPS mode of operation to CS/PS mode 2.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_epsmode(void){

	LOG_BOX("SEND: Set EPS mode of operation to CS/PS mode 2");

	uart_tx(epsmode, strlen(epsmode), LTEGPS_UART);

	if( uart_rx_check(Resp_LTEGPS_OK, strlen(Resp_LTEGPS_OK), UART_1S_TIMEOUT) ){
		LOG("ERROR: No response.\r\n");
		uart_rx_print();
		return FAIL;
	}

	uart_rx_print();
	return PASS;

}

/*****************************************************************************/
/*! @Function Name: api_ltegps_pdpactivate
 *  @brief        : Activate the PDP context found after calling
 *  				api_ltegps_pdpavailable.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_pdpactivate(void){

	LOG_BOX("SEND: Activate PDP context");

	uart_tx(pdpactivate, strlen(pdpactivate), LTEGPS_UART);

	if( uart_rx_check(Resp_LTEGPS_OK, strlen(Resp_LTEGPS_OK), UART_1S_TIMEOUT) ){
		LOG("ERROR: No response.\r\n");
		uart_rx_print();
		return FAIL;
	}

	uart_rx_print();
	return PASS;

}
/******************** LTE API END ********************************************/

/******************** GPS API START ******************************************/

/*****************************************************************************/
/*! @Function Name: api_ltegps_startnmea
 *  @brief        : NMEA data stream start command of USART LTEGPS module.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_startnmea(void){

	LOG_BOX("SEND: NMEA data stream start ");
	LOG("Waiting for valid GPS response. (1 minute timeout)");
	uart_tx(startnmea, strlen(startnmea), LTEGPS_UART);

	if( uart_rx_check(Resp_LTEGPS_OK, strlen(Resp_LTEGPS_OK), UART_1S_TIMEOUT) ){
		LOG("ERROR: No response.\r\n");
		uart_rx_print();
		return FAIL;
	}

	if( uart_rx_check(Resp_LTEGPS_VALID, strlen(Resp_LTEGPS_VALID), 60 * UART_1S_TIMEOUT) ){
		LOG("ERROR: Valid GPS response not found.\r\n");
		uart_rx_print();
		return FAIL;
	}

	api_ltegps_parsenmea(); // Populate GPS struct

	uart_rx_print();
	return PASS;


}

/*****************************************************************************/
/*! @Function Name: api_ltegps_endnmea
 *  @brief        : NMEA data stream end command of USART LTEGPS module.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_endnmea(void){

	LOG_BOX("SEND: NMEA data stream end ");

	uart_tx(endnmea, strlen(endnmea), LTEGPS_UART);

	if( uart_rx_check(Resp_LTEGPS_OK, strlen(Resp_LTEGPS_OK), UART_1S_TIMEOUT) ){
		LOG("ERROR: No response.\r\n");
		uart_rx_print();
		return FAIL;
	}

	uart_rx_print();
	return PASS;

}


/*****************************************************************************/
/*! @Function Name: api_ltegps_selectgnss
 *  @brief        : GNSS antenna select command of USART LTEGPS module. This
 *  				command should be executed before calling api_ltegps_startnme
 *  				function.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_selectgnss(void){

	LOG_BOX("SEND: GNSS select antenna");

	uart_tx(selectgnss, strlen(selectgnss), LTEGPS_UART);

	if( uart_rx_check(Resp_LTEGPS_OK, strlen(Resp_LTEGPS_OK), UART_1S_TIMEOUT) ){
		LOG("ERROR: No response.\r\n");
		uart_rx_print();
		return FAIL;
	}

	uart_rx_print();
	return PASS;

}

/*****************************************************************************/
/*! @Function Name: api_ltegps_powergnss
 *  @brief        : GNSS power command of USART LTEGPS module. This command
 *  				should be executed before calling LGM_StartNMEA function.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_powergnss(void){

	LOG_BOX("SEND: GNSS controller power up");

	uart_tx(powergnss, strlen(powergnss), LTEGPS_UART);

	uint32_t timeout = 0;

	while( timeout < UART_1S_TIMEOUT){

		HAL_Delay(UART_DELAY); // 20 ms delay

		if( uart_rx_find(Resp_LTEGPS_OK, strlen(Resp_LTEGPS_OK) ) ){
			uart_rx_print();
			return PASS;
		}

		if( uart_rx_find(Resp_LTEGPS_ERROR, strlen(Resp_LTEGPS_ERROR) ) ){
			uart_rx_print();
			return PASS;
		}

		timeout += UART_DELAY;
	}

		uart_rx_print();
		LOG("ERROR: No response.\r\n");
		return FAIL;


}

/*****************************************************************************/
/*! @Function Name: LGM_ParseNMEA
 *  @brief        : NMEA parsing function of USART LTEGPS module. Used after
 *  				api_ltegps_startnmea is called.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_parsenmea(void){

	// $GPRMC,161229.487,A,3723.2475,N,12158.3416,W,0.13,309.62,120598,,*10<CR><LF>

	char Status = PASS;
	char *str = NULL;
	const char s[2] = ",";
	char *token;

	str = rx_buff;

	token = strtok(str, s);

	while( token != NULL && Status == PASS){

        // if token is start of RMC message
        if (strstr(token, "$GPRMC")) {

        	// store RMC data
        	token = strtok(NULL, s);
        	 GPS.UTC_time = strtof(token, NULL);

            // store latitude
            token = strtok(NULL, s);

        	// if valid stream
        	if(*token == 'A'){

        		Status = PASS;

            	// store latitude
                token = strtok(NULL, s);
                GPS.latitude = strtof(token, NULL);

                // store N/S indicator
                token = strtok(NULL, s);
                strcpy(GPS.NS_indicator, token);

                // store longitude
                token = strtok(NULL, s);
                GPS.longitude = strtof(token, NULL);

                // store E/W indicator
                token = strtok(NULL, s);
                strcpy( GPS.EW_indicator, token);

                // store speed over ground as km/h
                token = strtok(NULL, s);
                GPS.speed = strtof(token, NULL);
                GPS.speed *= 1.852;

                // store date
                token = strtok(NULL, s); // skip course over ground
                token = strtok(NULL, s);
                strcpy( GPS.date, token);
        	}

        }

    token = strtok(NULL, s);

	}

	return Status;
}

/******************** GPS API END ********************************************/


/*****************************************************************************/
/*! @Function Name: api_ltegps_check
 *  @brief        : Check for return response.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_check(void){

	LOG_BOX("SEND: response check");
	uart_tx(atcheck, strlen(atcheck), LTEGPS_UART);

	if( uart_rx_check(Resp_LTEGPS_OK, strlen(Resp_LTEGPS_OK), UART_1S_TIMEOUT) ){
		LOG("ERROR: No response.\r\n");
		uart_rx_print();
		return FAIL;
	}

	uart_rx_print();
	return PASS;
}

/*****************************************************************************/
/*! @Function Name: api_ltegps_echodisable
 *  @brief        : Check for return response.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_echodisable(void){

	LOG_BOX("SEND: echo disable");
	uart_tx(echodisable, strlen(echodisable), LTEGPS_UART);

	if( uart_rx_check(Resp_LTEGPS_OK, strlen(Resp_LTEGPS_OK), UART_1S_TIMEOUT) ){
		LOG("ERROR: No response.\r\n");
		uart_rx_print();
		return FAIL;
	}

	uart_rx_print();
	return PASS;
}
