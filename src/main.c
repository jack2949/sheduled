#include "log.h"

#define PID_FILE_PATH		"/tmp/shedule.pid"
#define CONFIG_FILE_PATH	"./config/shedule.config"

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

static void _main_loop()
{
	cJSON *config = NULL;
	cJSON *data = NULL;
	cJSON *elm = NULL;
	cJSON *startTime = NULL;
	cJSON *command = NULL;
	int cnt = 0;
	int index = 0;
	time_t timep;
	struct tm *p;
	char timeStr[16];

	config = _get_JSON_from_file(CONFIG_FILE_PATH);

	if (NULL == config)
	{
		LOG_ERR("get config errer");
		return;
	}

	/* attendtion free */
	PRINTF("data = %s", cJSON_Print(config));
	while (TRUE)
	{
		time(&timep);		
		p = localtime(&timep);
		sprintf(timeStr, "[%02d:%02d:%02d]", p->tm_hour, p->tm_min, p->tm_sec);

		PRINTF("%s", timeStr);
		
		data = cJSON_GetObjectItem(config, "data");
		cnt = cJSON_GetArraySize(data);
		
		for(index = 0; index < cnt; index++)
		{
			elm = cJSON_GetArrayItem(data, index);
			startTime = cJSON_GetObjectItem(elm, "startTime");
			if( startTime != NULL && startTime->type == cJSON_String )
			{
				if(0 == strncmp(startTime->valuestring, timeStr, strlen(timeStr)))
				{ 
					command = cJSON_GetObjectItem(elm, "command");
					if( command != NULL && command->type == cJSON_String )
					{
						_exec_cmd(command->valuestring);
					}
				}
			}
		}

		sleep(1);
		//usleep(800*1000);
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

	_main_loop();

exit:
	/* Destroy pid file */
	unlink(PID_FILE_PATH);

	return ret;

}

