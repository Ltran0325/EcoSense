/*****************************************************************************/
/*!
 * @project    EcoSense
 * @file       round_robin.c
 * @author     Long Tran
 * @version    1.1
 * @brief	   Round Robin handler
 * @date       28/July/2021
 * @bug        NA

 * @note
 */
/*****************************************************************************/

/******************** HEADER FILES *******************************************/

/******************** DEFINE MACROS ******************************************/


/******************** DEFINE ENUMS and STRUCT ********************************/

/* Enumeration for Round Robin States */
typedef enum
{
	START,
	WIFI,
	//LTE,
	CAMERA,
	DATA_AQ,
}rr_States;


/******************** DEFINE GLOBAL VARIABLES  *******************************/

rr_States rrCurrentState = START;
rr_States rrPreviousState = START;

/******************** FUNCTION DECLARATION************************************/
