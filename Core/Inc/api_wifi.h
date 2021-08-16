/*****************************************************************************/
/*!
 * @project    EcoSense
 * @file       api_wifi.h
 * @author     Long Tran
 * @version    1.1
 * @brief	   Wi-Fi handler
 * @date       28/July/2021
 * @bug        NA

 * @note
 */
/*****************************************************************************/

/******************** HEADER FILES *******************************************/

/******************** DEFINE MACROS ******************************************/
#define AP_KNOWN_COUNT 3
#define RSSI_DEFAULT 255

/******************** DEFINE ENUMS and STRUCT ********************************/

typedef struct
{
	 char SSID[33];     /* Buffer to hold the SSID for WiFi N/W connection     		      */
	 char Password[33]; /* Buffer to hold the Password for WiFi N/W connection 			  */
	 uint8_t RSSI;	    /* Buffer to hold the absolute RSSI value for WiFi N/W connection */
}WiFi_Struct;

/******************** DEFINE GLOBAL VARIABLES  *******************************/
extern uint8_t strAP_idx;

extern WiFi_Struct AP_1;

extern WiFi_Struct AP_2;

extern WiFi_Struct AP_3;

extern WiFi_Struct* AP_List_Known[3];
// WiFi AT commands
static char AT_check[]  		= "AT\r\n";
static char AT_station[] 		= "AT+WNI=0\r\n";
static char AT_scan[]   	    = "AT+WS=1\r\n";
static char AT_connect[] 		= "AT+WNCN=1,\"Trans 5G\",\"2232portal\"\r\n";
static char AT_ping[]    		= "AT+NPING=8.8.8.8,64,1\r\n";
static char AT_echodisable[] 	= "ATE0\r\n";

// WiFi AT responses
static char Resp_WIFI_OK[]      = "OK\r\n";
static char Resp_WIFI_ERROR[]   = "ERROR";
static char Resp_WIFI_SCAN[]    = "Trans 5G"; // change for different known AP
static char Resp_WIFI_SUCCESS[] = "SUCCESS";

/******************** FUNCTION DECLARATION************************************/

/******************** WI-FI APPLICATION FUNCTIONS START **********************/
/*****************************************************************************/
/*! @Function Name: api_wifi_connect
 *  @brief        : High level function to issue command sequence to allow
 *  				ping to google.com
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_wifi_connect(void);

/*****************************************************************************/
/*! @Function Name: api_wifi_ping
 *  @brief        : High level function to ping to google.com
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_wifi_ping(void);

/******************** WI-FI APPLICATION FUNCTIONS END ************************/

/******************** WI-FI API START ****************************************/

/*****************************************************************************/
/*! @Function Name: api_wifi_station
 *  @brief        : Station mode command of USART Wi-Fi module. This command
 *  				sets Wi-Fi module in Station mode.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_wifi_station(void);

/*****************************************************************************/
/*! @Function Name: api_wifi_scan
 *  @brief        : AP scan command of USART Wi-Fi module. This command
 *  				scans for nearby AP and parses for known AP RSSI values.
 *  				This function should be called before WM_KnownConnect.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_wifi_scan(void);

/*****************************************************************************/
/*! @Function Name: api_wifi_scanparse
 *  @brief        : This function is used by api_wifi_scan function to parse for
 *  				AP RSSI values.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_wifi_scanparse(void);

/*****************************************************************************/
/*! @Function Name: api_wifi_connect
 *  @brief        : Known AP connect command of USART Wi-Fi module. This command
 *  				connects to strongest known AP.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_wifi_known(void);

/*****************************************************************************/
/*! @Function Name: api_wifi_echodisable
 *  @brief        : Echo disable command of USART Wi-Fi module.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_wifi_echodisable(void);

/*****************************************************************************/
/*! @Function Name: api_wifi_check
 *  @brief        : Check for return response.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_wifi_check(void);

/******************** WI-FI API END ******************************************/
