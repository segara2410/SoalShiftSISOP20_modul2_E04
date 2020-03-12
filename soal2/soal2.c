#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

int main() 
{
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

  // close(STDIN_FILENO);
  // close(STDOUT_FILENO);
  // close(STDERR_FILENO);

  time_t timer;
  struct tm* tm_info;

  while (1) 
  {
    timer = time(NULL);
    tm_info = localtime(&timer);

    char folder_name[100];
    strftime(folder_name, 100, "/home/segara/SoalShiftSISOP20_modul2_E04/soal2/%Y-%m-%d_%H:%M:%S", tm_info);

    pid_t child_id;
    child_id = fork();

    int status;

    if (child_id < 0) 
      exit(EXIT_FAILURE);

    if (child_id == 0)
    { 
      if (fork() == 0)
      {
        char *argv[] = {"mkdir", "-p", folder_name, NULL};
        execv("/bin/mkdir", argv);
      }
      else 
      {
        while ((wait(&status)) > 0);
        for (int i = 0; i < 20; i++)
        {
          if (fork() == 0)
          {
            chdir(folder_name);
            time_t file_timer;
            struct tm* file_tm_info;
        
            file_timer = time(NULL);
            file_tm_info = localtime(&file_timer);
        
            int t = (int)time(NULL);
            t = (t % 1000) + 100;
            
            char target[100];
            sprintf(target, "https://picsum.photos/%d", t);

            char file_name[100];
            strftime(file_name, 100, "%Y-%m-%d_%H:%M:%S", file_tm_info);
            char *argv[] = {"wget", target, "-qO", file_name, NULL};
            execv("/usr/bin/wget", argv);
          }
          sleep(5);
        }
        if (fork() == 0)
        {
          char folder_name_zip[100];
          sprintf(folder_name_zip, "%s.zip", folder_name);
          char *argv[] = {"zip", "-qrm", folder_name_zip, folder_name, NULL};
          execv("/usr/bin/zip", argv);
        }
        exit(0);
      }
    }
    else
      sleep(30);
      
  }
}