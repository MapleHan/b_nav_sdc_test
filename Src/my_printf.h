#ifndef __MY_PRINTF_H__
#define __MY_PRINTF_H__
#define USE_UART_DBG 1
enum DBG_LEVEL
{
	DBG_ALL = 0,
	DBG_INFO,
	DBG_WARNING,
	DBG_ERROR,
	DBG_NONE
};

extern enum DBG_LEVEL _level;
void DBG_Print_Init(void);
void DBG_Print(enum DBG_LEVEL level, char *format, ...);
#endif


