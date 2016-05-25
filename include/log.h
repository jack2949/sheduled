#ifndef		_LOG_H_
#define		_LOG_H_

/*
  Copyright (c) 2016 Jack2949
 
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
 
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
 
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include "type.h"
int _exec_cmd(char* cmd, ...);

void _log_format(char* cmd, ...);

void _log_to_file(char* file_name, char* cmd, ...);

int _exec_by_pthread(char *command);


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
