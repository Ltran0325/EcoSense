/*****************************************************************************/
/*!
 * @project    EcoSense
 * @file       api_ltegps.h
 * @author     Long Tran
 * @version    0.0.2
 * @brief	   Camera Handler
 * @date       28/July/2021
 * @bug        NA

 * @note
 */
/*****************************************************************************/


/******************** HEADER FILES *******************************************/
#include "uart.h"
/******************** DEFINE MACROS ******************************************/

/******************** DEFINE GLOBAL VARIABLES  *******************************/

/******************** DEFINE ENUMS and STRUCT ********************************/

typedef struct
{
    float UTC_time;
    float latitude;
    float longitude;
    char NS_indicator[2];
    char EW_indicator[2];
    float speed;
    char date[7];

}LTEGPS_Struct;

LTEGPS_Struct GPS = {0};

/******************** DEFINE GLOBAL VARIABLES  *******************************/

static char fwswitch[] =      "AT#FWSWITCH=1\r\n";	// set f/w image to Verizon
static char signalquality[] = "AT+CESQ\r\n"; // check tower signal quality
static char pdpset[] =        "AT+CGDCONT=1,\"IPV4V6\",\"\"\r\n"; // set PDP context to CID = 1, PDP_Type = IPV4V6, APN =
static char pdpavailable[] =  "AT+CGDCONT?\r\n"; // check available PDP context types
static char wdsselect[] =     "AT+WS46=28\r\n"; // select WDS to be EU-TRAN (28)
static char epsmode[] =       "AT+CEMODE=2\r\n"; // set EPS mode of operation to CS/PS mode 2
char pdpactivate[] =          "AT#SGACT=X,1\r\n"; // activate pdp context set of pdpselect function
static char lteping[] =       "AT#PING=\"www.google.com\"\r\n"; // ping google.com

// LTE AT responses
static char Resp_LTEGPS_FWSwitch[] = "AT#FWSWITCH=1\r\n";
static char Resp_LTEGPS_PDPSet[]   = "";
static char Resp_LTEGPS_Ping[]     = "PING:";

// GPS AT commands
static char echodisable[] = "ATE0\r\n";	//disable echo
static char selectgnss[]  = "AT$GPSAT=1\r\n";// select GNSS antenna
static char powergnss[]   = "AT$GPSP=1\r\n"; // power up GNSS controller
static char startnmea[]   = "AT$GPSNMUN=1,0,0,0,0,1,0\r\n"; // start GPS NMEA data stream
static char endnmea[]     = "AT$GPSNMUN=0,0,0,0,0,1,0\r\n"; // start GPS NMEA data stream
// GPS AT responses
static char Resp_LTEGPS_NMEA[] =      "$GPRMC";
static char Resp_LTEGPS_VALID[] =     ",A,";
static char Resp_LTEGPS_INVALID[] =   ",V,";

// Misc. Commands
static char atcheck[] = "AT\r\n";

// Misc. Responses
static char Resp_LTEGPS_OK[] =        "OK\r\n";
static char Resp_LTEGPS_ERROR[] =     "ERROR\r\n";

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
char api_ltegps_lteconnect(void);

/*****************************************************************************/
/*! @Function Name: api_ltegps_gpsconnect
 *  @brief        : High level function to ping www.google.com via LTE.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_lteping(void);

/*****************************************************************************/
/*! @Function Name: LGM_GPSConnect
 *  @brief        : High level function to issue command sequence to
 *  				start NMEA data stream and parse. The EVB requires 20 ms
 *  				delay between command response and next command.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_gpsconnect(void);

/*****************************************************************************/
/*! @Function Name: LGM_GPSConnect
 *  @brief        : High level function to issue command sequence to
 *  				start NMEA data stream and parse. The EVB requires 20 ms
 *  				delay between command respone and next command.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_gpsconnect(void);


/******************** LTEGPS APPLICATION FUNCTIONS END ***********************/

/******************** LTE API START ******************************************/

/*****************************************************************************/
/*! @Function Name: api_ltegps_fwswitch
 *  @brief        : Set firmware image command of USART LTEGPS module.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_fwswitch(void);

/*****************************************************************************/
/*! @Function Name: api_ltegps_signalquality
 *  @brief        : Check signal quality command of USART LTEGPS module.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_signalquality(void);

/*****************************************************************************/
/*! @Function Name: api_ltegps_signalqualitycheck
 *  @brief        : Check rsrq and rsrp values of LGM_SignalQuality response.
 *  				Both values should be greater than 5 but should not be 255.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_signalqualitycheck(void);

/*****************************************************************************/
/*! @Function Name: api_ltegps_pdpset
 *  @brief        : Set PDP context command of USART LTEGPS module.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_pdpset(void);

/*****************************************************************************/
/*! @Function Name: LGM_PDPAvailable
 *  @brief        : Check available PDP context type command of USART LTEGPS module.
 *  				This function also parses for Verizon CID value.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_pdpavailable(void);

/*****************************************************************************/
/*! @Function Name: api_ltegps_pdpavailableparse
 *  @brief        : This function will parse for the CID value of "vzwinternet"
 *  				from api_ltegps_pdpavailable.
 *  @param        : apn, ex. "vzwadmin"
 *  @return       : CID of apn or 0 if fail
 */
/*****************************************************************************/
char api_ltegps_pdpavailableparse(char* apn);

/*****************************************************************************/
/*! @Function Name: api_ltegps_wdsselect
 *  @brief        : Set WDS setting to EU-TRAN (28).
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_wdsselect(void);

/*****************************************************************************/
/*! @Function Name: api_ltegps_epsmode
 *  @brief        : Set EPS mode of operation to CS/PS mode 2.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_epsmode(void);

/*****************************************************************************/
/*! @Function Name: api_ltegps_pdpactivate
 *  @brief        : Activate the PDP context found after calling
 *  				api_ltegps_pdpavailable.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_pdpactivate(void);

/******************** LTE API END ********************************************/

/******************** GPS API START ******************************************/

/*****************************************************************************/
/*! @Function Name: api_ltegps_startnmea
 *  @brief        : NMEA data stream start command of USART LTEGPS module.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_startnmea(void);

/*****************************************************************************/
/*! @Function Name: api_ltegps_endnmea
 *  @brief        : NMEA data stream end command of USART LTEGPS module.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_endnmea(void);

/*****************************************************************************/
/*! @Function Name: api_ltegps_selectgnss
 *  @brief        : GNSS antenna select command of USART LTEGPS module. This
 *  				command should be executed before calling api_ltegps_startnme
 *  				function.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_selectgnss(void);

/*****************************************************************************/
/*! @Function Name: api_ltegps_powergnss
 *  @brief        : GNSS power command of USART LTEGPS module. This command
 *  				should be executed before calling LGM_StartNMEA function.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_powergnss(void);

/*****************************************************************************/
/*! @Function Name: LGM_ParseNMEA
 *  @brief        : NMEA parsing function of USART LTEGPS module. Used after
 *  				api_ltegps_startnmea is called.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_parsenmea(void);

/******************** GPS API END ********************************************/

/*****************************************************************************/
/*! @Function Name: api_ltegps_check
 *  @brief        : Check for return response.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_check(void);

/*****************************************************************************/
/*! @Function Name: api_ltegps_echodisable
 *  @brief        : Check for return response.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_ltegps_echodisable(void);
