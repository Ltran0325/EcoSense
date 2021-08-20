/*****************************************************************************/
/*!
 * @project    EcoSense
 * @file       api_camera.c
 * @author     Long Tran
 * @version    0.0.1
 * @brief	   Camera handler
 * @date       28/July/2021
 * @bug        NA

 * @note
 */
/*****************************************************************************/

/******************** INCLUDE FILES ******************************************/
#include "stdint.h"
#include "stm32l476xx.h"
#include "api_camera.h"
#include "uart.h"

/******************** GLOBAL VARIABLES ***************************************/
char imagedata[] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00,
		            0x00, 0x00, 0x00, 0x00, 0xBE, 0xEF, 0x00, 0x0A};


/******************** CAMERA APPLICATION FUNCTIONS START *********************/

/*****************************************************************************/
/*! @Function Name: api_camera_connect
 *  @brief        : High level function to issue command sequence to
 *  				capture image to camera buffer.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_camera_connect(void){

	LOG_BOX("\r\nBeginning camera capture image sequence.\r\n");

	if( api_camera_stopcap() ){
		return FAIL;
	}

	if( api_camera_imageres() ){
		return FAIL;
	}

	if( api_camera_imagecomp() ){
		return FAIL;
	}

	if( api_camera_imageget() ){
		return FAIL;
	}

	if( api_camera_imagelen() ){
		return FAIL;
	}

	if( api_camera_imagedata() ){
		return FAIL;
	}

	if( api_camera_stopcap() ){
		return FAIL;
	}

	LOG_BOX("\r\nSUCCESS: Camera image captured successfully.\r\n");

	return PASS;
}


/******************** CAMERA APPLICATION FUNCTIONS END ***********************/

/******************** CAMERA API START ***************************************/

/*****************************************************************************/
/*! @Function Name: api_camera_stopcap
 *  @brief        : Get image length command of USART camera module. This command
 *  				allows for new images to be taken.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_camera_stopcap(void){

	LOG_BOX("SEND: camera stop capture");
	uart_tx(stopcap, 5, CAMERA_UART);

	if( uart_rx_check(Resp_CAM_STOPCAP, 5, UART_1S_TIMEOUT) ){
		LOG("ERROR: Bad response");
		uart_rx_print();
		return FAIL;
	}else{
		uart_rx_print();
		return PASS;
	}

	return FAIL; // Out of bounds
}

/*****************************************************************************/
/*! @Function Name: api_camera_imageres
 *  @brief        : Set resolution of USART camera module.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_camera_imageres(void){

	LOG_BOX("SEND: camera image resolution to 160x120");
	uart_tx(imageres, 5, CAMERA_UART);

	if( uart_rx_check(Resp_CAM_RESOLUTION, 5, UART_1S_TIMEOUT) ){
		LOG("ERROR: Bad response");
		uart_rx_print();
		return FAIL;
	}else{
		uart_rx_print();
		return PASS;
	}

	return FAIL; // Out of bounds
}

/*****************************************************************************/
/*! @Function Name: api_camera_imagecomp
 *  @brief        : Set compression ratio of USART camera module.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_camera_imagecomp(void){

	LOG_BOX("SEND: camera image compression ratio to 99");
	uart_tx(imagecomp, 9, CAMERA_UART);

	if( uart_rx_check(Resp_CAM_COMPRESS, 5, UART_1S_TIMEOUT) ){
		LOG("ERROR: Bad response");
		uart_rx_print();
		return FAIL;
	}else{
		uart_rx_print();
		return PASS;
	}

	return FAIL; // Out of bounds
}

/*****************************************************************************/
/*! @Function Name: api_camera_imageget
 *  @brief        : Get image command of USART camera module. The CM_ImageGet
 *  				and CM_ImageLength commands are still needed to store image
 *  				into buffer.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_camera_imageget(void){

	LOG_BOX("SEND: camera image get");
	uart_tx(imageget, 5, CAMERA_UART);

	if( uart_rx_check(Resp_CAM_IMAGEGET, 5, UART_1S_TIMEOUT) ){
		LOG("ERROR: Bad response");
		uart_rx_print();
		return FAIL;
	}else{
		uart_rx_print();
		return PASS;
	}

	return FAIL; // Out of bounds
}

/*****************************************************************************/
/*! @Function Name: api_camera_imagelen
 *  @brief        : Get image length command of USART camera module. This command
 *  				returns image data length bytes which are necessary for
 *  				CM_ImageLength command.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_camera_imagelen(void){

	uint16_t i = 0;

	LOG_BOX("SEND: camera image length");
	uart_tx(imagelen, 5, CAMERA_UART);

	if( uart_rx_check(Resp_CAM_LENGTH, 7, UART_1S_TIMEOUT) ){
		LOG("\r\nERROR: Bad response.\r\n");
		uart_rx_print();
		return FAIL;
	}

	i = uart_rx_find(Resp_CAM_LENGTH, 7);
	imagedata[12] = rx_buff[i];
	imagedata[13] = rx_buff[i+1];

	LOG("\r\nSuccess: image length \r\n");
	uart_rx_print();

	return PASS;
}

/*****************************************************************************/
/*! @Function Name: api_camera_imagedata
 *  @brief        : Read image data command of USART camera module. This command
 *  				stores image data into camera_buff.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_camera_imagedata(void){

	uint16_t camera_idx = 0, i = 0, offset = 0;

	LOG_BOX("SEND: camera image data");
	uart_tx(imagedata, 16, CAMERA_UART);

	if( uart_rx_check(Resp_CAM_DATAEND, 7, 3 * UART_1S_TIMEOUT) ){
		LOG("\r\nERROR: Bad response.\r\n");
		uart_rx_print();
		return FAIL;
	}

	offset = uart_rx_find(Resp_CAM_DATASTART, 2) - 2;
	i = offset;

	while( i < rx_idx - offset){
		camera_buff[camera_idx] = rx_buff[i];
		camera_idx++;
		i++;
	}

	uart_rx_print();

	return PASS;
}
/******************** CAMERA API END *****************************************/
