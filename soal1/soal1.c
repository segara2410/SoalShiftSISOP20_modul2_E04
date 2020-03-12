#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

int isAsterisk(char source[])
{
  if (strlen(source) == 1)
    if (source[0] == '*')
      return 1;

  return 0;
}

int isNumber(char source[])
{
  for (int i = 0; i < strlen(source); i++)
    if (source[i] < '0' || source[i] > '9')
      return 0;

  return 1;
}

int main(int argc, char **argv) 
{
  if(argc == 5)
  {
    int arg[4];
    for (int i = 1; i < 4; i++)
    {
      if (isNumber(argv[i]))
      {
        arg[i] = atoi(argv[i]);
        if(arg[i] < 0)
          printf("argument %d is not valid\n", i);
      }
      else if (isAsterisk(argv[i]))
        arg[i] = -1;
      else
      {
        printf("argument %d is not valid\n", i);
        exit(EXIT_FAILURE);
      }
    }

    if (arg[1] > 59 || arg[2] > 59 || arg[3] > 23)
    {
      printf("argument is not valid\n");
      exit(EXIT_FAILURE);
    }

    pid_t pid, sid;

    pid = fork();

    if (pid < 0)
      exit(EXIT_FAILURE);

    if (pid > 0) 
      exit(EXIT_SUCCESS);

    umask(0);

    sid = setsid();
    if (sid < 0)
      exit(EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1) 
    {
      time_t timer;
      struct tm* tm_info;  

      timer = time(NULL);
      tm_info = localtime(&timer);

      if ((tm_info->tm_sec == arg[1] || arg[1] == -1) && (tm_info->tm_min == arg[2] || arg[2] == -1) 
        && (tm_info->tm_hour || arg[3] == -1))
      {
        pid_t child_id;
        child_id = fork();

        int status;

        if (child_id == 0)
        {    
          char *argexec[] = {"bash", argv[4], NULL};
          execv("/bin/bash", argexec);
        }
      }

      sleep(1);
    }
  }
  else
  {
    printf("argument is not valid!\n");
    exit(EXIT_FAILURE);
  }
}
