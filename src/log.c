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

	snprintf(buf1, sizeof(buf1), "echo \"[%02d:%02d:%02d] %s\" >> %s", p->tm_hour, p->tm_min, p->tm_sec, buf, file_name);
	system(buf1);
}

