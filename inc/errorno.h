#ifndef __errorno_H__
#define __errorno_H__


typedef enum _eCloudAccessErrorCode
{
    ERROR_CODE_SUCCESS,

    ERROR_CODE_GENERAL_ERROR_START = 0xf0000,
    ERROR_CODE_GENERAL_ERROR,
    ERROR_CODE_NETWORK_ERROR,
    ERROR_CODE_SERVER_RETURN_FORMAT_ERROR,
    ERROR_CODE_NULL_POINT_EXCEPTION,
    ERROR_CODE_NONEXPECT_ERROR,
    ERROR_CODE_READ_BIG_FILE,
    ERROR_CODE_CNT,
    ERROR_CODE_PATH_NOT_EXIST,

}eCloudAccessErrorCode;

#endif

