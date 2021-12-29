#include "my_printf.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "stdarg.h"
#include "main.h"

enum DBG_LEVEL _level = DBG_ALL;

void DBG_Print_Init(void)
{

}

static void DBG_My_Printf(uint8_t* data, uint16_t size)
{
	if(USE_UART_DBG)
	{
        HAL_UART_Transmit(&huart1, data, size, 10);
//		HAL_UART_Transmit_DMA(&huart1, data, size);
	}
}

void DBG_Print(enum DBG_LEVEL level, char *format, ...)
{
    while ( huart1.gState != HAL_UART_STATE_READY ) ;// 防止代码重入导致资源抢占
	if(level<_level)
		return;
	char  buf_str[1024 + 1]; 
	va_list   v_args;
	va_start(v_args, format); 
	vsnprintf((char       *)&buf_str[0], 
								 (size_t      ) sizeof(buf_str), 
								 (char const *) format, 
																v_args); 
    va_end(v_args);
	DBG_My_Printf((uint8_t*)(buf_str), strlen(buf_str));
}

