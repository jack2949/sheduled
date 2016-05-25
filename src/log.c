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

#include "log.h"

int _systemEx (char *command)
{
	int pid = 0, status = 0;
	char *newCommand = NULL;

	if ( command == 0 )
	{
		return 0;
	}
	
 	pid = fork();
	if ( pid == -1 )
	{
		return -1;
	}
	
	if ( pid == 0 ) 
	{
		char *argv[4];
		argv[0] = "sh";
		argv[1] = "-c";
		argv[2] = command;
		argv[3] = 0;

		//execve("/bin/sh", argv, environ);
		execve("/bin/sh", argv, NULL);
		exit(127);
	}

	/* wait for child process return */
	////////
	do 
	{
		if ( waitpid(pid, &status, 0) == -1 )
		{
			if ( errno != EINTR )
			{
	         	return -1;
			}
	   	}
		else
		{
	     	return status;
		}
	} while ( 1 );

	return 0;
}

void * _exec_command(void *argv)
{
	char *command = (char *)argv;

	if(0 == system(command))
	{
		_log_to_file(LOG_FILE_PATH, "exec command OK:%s",	command);
	}
	else
	{
		_log_to_file(ERROR_LOG_FILE_PATH, "exec command Fail:%s",	command);
	}
	
	return NULL;
}

int _exec_by_pthread(char *command)
{
	pthread_t pid;
	pthread_attr_t a; 
	pthread_attr_init(&a);	
	pthread_attr_setdetachstate(&a, PTHREAD_CREATE_DETACHED);	  

	return pthread_create( &pid, &a, _exec_command, (void*)command);			
}

int _exec_cmd(char* cmd, ...)
{
    char buf[2048] = {0};
    va_list vaList;

    va_start (vaList, cmd);
    vsprintf (buf, cmd, vaList);
    va_end (vaList);

    PRINTF("%s", buf);
    
    return system(buf);
}

void _log_format(char* cmd, ...)
{
	char buf[2048] = {0};
	char buf1[2048] ={0};
	time_t timep;
	struct tm *p;
	va_list vaList;
	va_start (vaList, cmd);
	vsprintf (buf, cmd, vaList);
	va_end (vaList);
	time(&timep);		
	p = localtime(&timep);

	snprintf(buf1, sizeof(buf1), "[%02d:%02d:%02d] %s", p->tm_hour, p->tm_min, p->tm_sec, buf);
	printf("%s", buf1);
}

void _log_to_file(char* file_name, char* cmd, ...)
{
	char buf[2048] = {0};
	char buf1[2048] ={0};
	time_t timep;
	struct tm *p;
	va_list vaList;
	va_start (vaList, cmd);
	vsprintf (buf, cmd, vaList);
	va_end (vaList);
	time(&timep);		
	p = localtime(&timep);

	snprintf(buf1, sizeof(buf1), "echo \"[%02d-%02d %02d:%02d:%02d] %s\" >> %s", p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, buf, file_name);
	system(buf1);
}

