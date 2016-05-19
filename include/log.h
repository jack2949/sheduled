#ifndef		_LOG_H_
#define		_LOG_H_

#include "type.h"
int _exec_cmd(char* cmd, ...);

void _log_format(char* cmd, ...);

void _log_to_file(char* file_name, char* cmd, ...);

#define LOG_DEBUG
#define PRINTF	LOG_PRINT

#ifdef LOG_DEBUG
#define LOG_PRINT(fmt, args...) do { \
        _log_format("[%s:%d]: " fmt "\n", __func__, __LINE__, ## args); \
} while (0)
#else 
#define LOG_PRINT(fmt, args...)
#endif

#define LOG_OUT(fmt, args...) do { \
        _log_format("[%s:%d]: " fmt "\n", __func__, __LINE__, ## args); \
} while (0)

#define LOG_ERR(fmt, args...) do { \
        _log_format("[ERROR][%s:%d]: " fmt "\n", __func__, __LINE__, ## args); \
} while (0)

#define LOG_FILE_PATH		"./reports/run.log"
#define ERROR_LOG_FILE_PATH	"./reports/error.log"

#endif
