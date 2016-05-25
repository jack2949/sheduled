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

#define PID_FILE_PATH		"/tmp/shedule.pid"
#define CONFIG_FILE_PATH	"./config/shedule.config"

static TASK_ENTRY	*task = NULL;
static int			task_index = 0;
static int			task_MAX = 0;

static void _daemonize()
{
    int fd = 0;

    if(0 != fork())
        exit(0);

    if(setsid() < 0)
        exit(0);
    
    umask(0);

    if ((fd = open("/dev/null", O_RDWR, 0)) != -1)
    {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);

        if(fd >  STDERR_FILENO)
        {
            close(fd);
        }
    }
}

cJSON* _get_JSON_from_file(char* filename)
{
	cJSON *data = NULL;
	int fd = 0;
	char *config = NULL;
	int totalsize = 0;
	struct stat st;
	           
	fd = open(filename, O_RDWR);
	if (fd < 0)
	{
		goto leave;
	}

    fstat(fd, &st);
    totalsize = st.st_size;
	config = malloc(totalsize);
	if (config == NULL)
	{
		LOG_ERR("Malloc fail.\n");
		goto leave;	
	}

    if(totalsize != read(fd, config, totalsize))
	{
		LOG_ERR("read %s errer\n", filename);
   		goto leave;
	}
	PRINTF("%s", config);
	
  	data = cJSON_Parse(config);
    if (!data)
	{
		LOG_ERR("str to json failed\n");
		goto leave;
	}

leave:
	if (fd > 0)
		close(fd);
    if (config)
		free(config);

	return data;
}

static void _close(int signo)
{
	LOG_OUT("recvd SIGTERM, exit now.");
	_log_to_file(LOG_FILE_PATH, "recvd SIGTERM, exit now!");
	unlink(PID_FILE_PATH);
	exit(0);
}

static void _signal_process(void)
{
	signal(SIGTERM, _close);
}

static int _create_pid_file(void)
{
	int ret = ERROR;
	FILE* pidfile;
	
	/* Check whether this process is running */
	if ((pidfile = fopen(PID_FILE_PATH, "r"))) {
		LOG_OUT("sheduled has been started");

		fclose(pidfile);
		return ERROR;
	}

	/* Create pid file */
	if ((pidfile = fopen(PID_FILE_PATH, "w"))) {
		fprintf(pidfile, "%d\n", getpid());
		fclose(pidfile);
		ret = OK;
	}
	else {
		perror("pidfile");
		ret = ERROR;
	}
	
	return ret;
}

static INT32 _current_time()
{
	time_t now;
	struct tm *time_tm;
	time(&now);

	time_tm = localtime(&now);

	return time_tm->tm_hour*60*60 + time_tm->tm_min*60 + time_tm->tm_sec;
}

static void _quick_sort(int head, int end, TASK_ENTRY *task)
{
    int first, last;
	TASK_ENTRY tmp;
    if(NULL == task || head >= end)
        return ;

    first = head;
    last = end;
    tmp = task[head];
    while (head < end)
    {
        /* The "=" is must, or loop lock */
        while(head < end && task[end].startTime >= tmp.startTime) end --;
        task[head] = task[end];
        /* The "=" is must. */
        while(head < end && task[head].startTime <= tmp.startTime) head ++;
        task[end] = task[head];
    }
    task[head] = tmp;

    _quick_sort(first, head - 1, task);
    _quick_sort(head + 1, last, task);
}

static int _load_config()
{
	cJSON *config = NULL;
	cJSON *data = NULL;
	cJSON *elm = NULL;
	cJSON *startTime = NULL;
	cJSON *command = NULL;
	int cnt = 0;
	int index = 0;
	int hour,min,second;
	int ret = OK;
	
	if (NULL != task)
	{
		free(task);
		task = NULL;
	}

	config = _get_JSON_from_file(CONFIG_FILE_PATH);
	if (NULL == config)
	{
		LOG_ERR("get config errer");
		ret = ERROR;
		goto leave;
	}

	/* attendtion free */
	PRINTF("data = %s", cJSON_Print(config));	
	data = cJSON_GetObjectItem(config, "data");
	cnt = cJSON_GetArraySize(data);

	task = (TASK_ENTRY *)malloc(sizeof(TASK_ENTRY)*cnt);
	if (NULL == task)
	{
		LOG_ERR("malloc error!");
		ret = ERROR;
		goto leave;
	}
	
	for(index = 0; index < cnt; index++)
	{
		elm = cJSON_GetArrayItem(data, index);
		startTime = cJSON_GetObjectItem(elm, "startTime");
		if( startTime != NULL && startTime->type == cJSON_String )
		{
			sscanf(startTime->valuestring, "%d:%d:%d", &hour, &min, &second);
			task[index].startTime = hour * 60 * 60 + min * 60 + second;
		}
		
		command = cJSON_GetObjectItem(elm, "command");
		if( command != NULL && command->type == cJSON_String )
		{
			if(strlen(command->valuestring) < MAX_COMMAND_LENGTH)
			{
				strcpy(task[index].command, command->valuestring);
			}
			else
			{
				LOG_ERR("command %s size %d error!", command->valuestring, strlen(command->valuestring));
				_log_to_file(ERROR_LOG_FILE_PATH, "command %s size %d error!",	command->valuestring, strlen(command->valuestring));
			}
		}
	}

	_quick_sort(0, cnt - 1, task);
	task_index = 0;
	task_MAX = cnt;
	
#ifdef LOG_DEBUG
	for(index = 0; index < cnt; index++)
	{
		PRINTF("%02d:%02d:%02d	%s", task[index].startTime/3600, (task[index].startTime % 3600)/60, task[index].startTime%60, task[index].command);
	}
#endif

leave:
	if(config)
		cJSON_Delete(config);
	
	return ret;
}

static void _main_loop()
{
	INT32 sleepTime = 0;
	INT32 currentTime = 0;
	int i;

	_log_to_file(LOG_FILE_PATH, "load config ok, start now!");
	currentTime = _current_time();
	for(i = 0; i < task_MAX; i++)
	{
		if(currentTime <= task[i].startTime)
		{
			break;
		}
	}
	task_index = i%task_MAX;

	PRINTF("task_MAX = %d, i = %d", task_MAX, i);
	PRINTF("%02d:%02d:%02d	%s", task[task_index].startTime/3600, (task[task_index].startTime % 3600)/60, task[task_index].startTime%60, task[task_index].command);

	while (TRUE)
	{
		if(task_index == 0)
		{
			sleepTime = task[task_index].startTime + 24*60*60 - _current_time();
		}
		else
		{
			sleepTime = task[task_index].startTime - _current_time();			
		}

		if (sleepTime >= 0)
		{
			sleep(sleepTime);
			if (OK == _exec_by_pthread(task[task_index].command))
			{
				_log_to_file(LOG_FILE_PATH, "exec task OK:%s",	task[task_index].command);
			}
			else
			{
				_log_to_file(ERROR_LOG_FILE_PATH, "exec task Fail:%s",	task[task_index].command);
			}
		}
		else
		{
			_log_to_file(ERROR_LOG_FILE_PATH, "%02d:%02d:%02d late exec:%s", task[task_index].startTime/3600, (task[task_index].startTime % 3600)/60, task[task_index].startTime%60, task[task_index].command);
			if (OK == _exec_by_pthread(task[task_index].command))
			{
				_log_to_file(LOG_FILE_PATH, "exec task OK:%s",	task[task_index].command);
			}
			else
			{
				_log_to_file(ERROR_LOG_FILE_PATH, "exec task Fail:%s",	task[task_index].command);
			}
		}

		task_index = (task_index + 1) % task_MAX ; 
	}
}

int main(int argc, char *argv[])
{
	int ret = OK;

	//daemonize();
	
	_signal_process();
	
	if (_create_pid_file() == ERROR)
	{
		LOG_ERR("Create pid file error!\n");
    	ret = ERROR;
        goto exit;
	}

	if (_load_config() == ERROR)
	{
		LOG_ERR("load config error!\n");
    	ret = ERROR;
        goto exit;
	}
	
	_main_loop();

exit:
	/* Destroy pid file */
	unlink(PID_FILE_PATH);

	return ret;

}

