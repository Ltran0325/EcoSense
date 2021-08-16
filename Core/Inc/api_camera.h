/*****************************************************************************/
/*!
 * @project    EcoSense
 * @file       api_camera.h
 * @author     Long Tran
 * @version    1.1
 * @brief	   Camera Handler
 * @date       28/July/2021
 * @bug        NA

 * @note
 */
/*****************************************************************************/


/******************** HEADER FILES *******************************************/
#include "uart.h"
/******************** DEFINE MACROS ******************************************/

/******************** GLOBAL VARIABLES ***************************************/

/******************** DEFINE ENUMS and STRUCT ********************************/

/******************** DEFINE GLOBAL VARIABLES  *******************************/

uint16_t camera_buff[BUFF_MAX];

// Hex commands to test SC03MPA camera
static char stopcap[]    = {0x56, 0x00, 0x36, 0x01, 0x03};
static char imageres[]   = {0x56, 0x00, 0x54, 0x01, 0x22}; // set image resolution to 160x120 (smallest setting)
static char imagecomp[]  = {0x56, 0x00, 0x31, 0x05, 0x01, 0x01, 0x12, 0x04, 0x99}; // set compression ratio to 99 (most compressed)
static char imagebaud[]  = {0x56, 0x00, 0x24, 0x03, 0x01, 0x0D, 0xA6}; // set camera to 115200 baud
static char imageget[]   = {0x56, 0x00, 0x36, 0x01, 0x00};
static char imagelen[]   = {0x56, 0x00, 0x34, 0x01, 0x00};
extern char imagedata[];
static char imagereset[] = {0x56, 0x00, 0x26, 0x00};

// Hex responses
static char Resp_CAM_STOPCAP[]    = {0x76, 0x00, 0x36, 0x00, 0x00};
static char Resp_CAM_RESOLUTION[] = {0x76, 0x00, 0x54, 0x00, 0x00};
static char Resp_CAM_COMPRESS[]   = {0x76, 0x00, 0x31, 0x00, 0x00};
static char Resp_CAM_BAUD[]       = {0x76, 0x00, 0x24, 0x00, 0x00};
static char Resp_CAM_IMAGEGET[]   = {0x76, 0x00, 0x36, 0x00, 0x00};
static char Resp_CAM_LENGTH[]     = {0x76, 0x00, 0x34, 0x00, 0x04, 0x00, 0x00};
static char Resp_CAM_DATASTART[]  = {0xFF, 0xD8};
static char Resp_CAM_DATAEND[]    = {0xFF, 0xD9, 0x76, 0x00, 0x32, 0x00, 0x00};
static char Resp_CAM_RESET[]      = {0x76, 0x00, 0x26, 0x00};

/******************** CAMERA APPLICATION FUNCTIONS START *********************/

/*****************************************************************************/
/*! @Function Name: api_camera_connect
 *  @brief        : High level function to issue command sequence to
 *  				capture image to camera buffer.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_camera_connect(void);

/******************** CAMERA APPLICATION FUNCTIONS END ***********************/

/******************** CAMERA API START ***************************************/

/*****************************************************************************/
/*! @Function Name: api_camera_stopcap
 *  @brief        : Get image length command of USART camera module. This command
 *  				allows for new images to be taken.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_camera_stopcap(void);

/*****************************************************************************/
/*! @Function Name: api_camera_imageres
 *  @brief        : Set resolution of USART camera module.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_camera_imageres(void);
/*****************************************************************************/
/*! @Function Name: api_camera_imagecomp
 *  @brief        : Set compression ratio of USART camera module.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_camera_imagecomp(void);

/*****************************************************************************/
/*! @Function Name: api_camera_imageget
 *  @brief        : Get image command of USART camera module. The CM_ImageGet
 *  				and CM_ImageLength commands are still needed to store image
 *  				into buffer.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_camera_imageget(void);

/*****************************************************************************/
/*! @Function Name: api_camera_imagelen
 *  @brief        : Get image length command of USART camera module. This command
 *  				returns image data length bytes which are necessary for
 *  				CM_ImageLength command.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_camera_imagelen(void);

/*****************************************************************************/
/*! @Function Name: api_camera_imagedata
 *  @brief        : Read image data command of USART camera module. This command
 *  				stores image data into camera_buff.
 *  @return       : pass or fail
 */
/*****************************************************************************/
char api_camera_imagedata(void);
/******************** CAMERA API END *****************************************/

