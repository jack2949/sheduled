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

#ifndef __TYPE_H__
#define __TYPE_H__

#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h> 
#include <time.h>
#include <pthread.h>

#include "cJSON.h"

#ifndef ERROR
#define ERROR	(-1)
#endif

#ifndef OK
#define OK		(0)
#endif

#ifndef TRUE
#define TRUE 	(1)
#endif

#ifndef FALSE
#define FALSE 	(0)
#endif

/* type */

//typedef unsigned int 		  UINT;
//typedef signed	int 		  INT;

#define INT     signed int
#define UINT    unsigned int
#define INT8	char
#define INT16	short
#define INT32	int
#define UINT8	unsigned char
#define UINT16	unsigned short
#define UINT32	unsigned int
#define ULONG   unsigned long
#define USHORT  UINT16

#ifndef BOOL
#define BOOL		int
#endif

#ifndef STATUS
#define STATUS int 
#endif

#ifndef LOCAL
#define LOCAL static
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef EOS
#define EOS		'\0'	/* C string terminator */
#endif

/* length */

#define	MAC_ADDR_SIZE 6

#define	IP_ADDRESS_LENGTH			16
#define	MAC_ADDRESS_LENGTH			18

#define	MAX_COMMAND_LENGTH			128

typedef	struct	_TASK_ENTRY_{
	UINT32	startTime;
	INT8	command[MAX_COMMAND_LENGTH];	
}TASK_ENTRY;

#endif /* __TYPE_H__ */
