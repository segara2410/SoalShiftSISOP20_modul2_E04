#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

int isDirectory(const char *path)
{
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISDIR(path_stat.st_mode);
}

int main() {
  pid_t child_1;
  int status;

  child_1 = fork();
  
  if (child_1 < 0) 
    exit(EXIT_FAILURE);

  if (child_1 == 0) 
  {
    pid_t child_2 = fork();

    if (child_2 < 0) 
      exit(EXIT_FAILURE);

    if (child_2 == 0) 
    {
      char *argexec[] = {"mkdir", "-p", 
        "/home/segara/SoalShiftSISOP20_modul2_E04/indomie", NULL};
      execv("/bin/mkdir", argexec);
    }
    else
    {
      while ((wait(&status)) > 0);
      sleep(5);

      char *argexec[] = {"mkdir", "-p", 
        "/home/segara/SoalShiftSISOP20_modul2_E04/sedaap", NULL};
      execv("/bin/mkdir", argexec);
    }
  } 
  else 
  {
    while ((wait(&status)) > 0);

    pid_t child_3 = fork();

    if (child_3 < 0) 
      exit(EXIT_FAILURE);

    if (child_3 == 0)
   	{
     	char* argexec[] = {"unzip", 
        "/home/segara/SoalShiftSISOP20_modul2_E04/jpg.zip", NULL};
     	execv("/usr/bin/unzip", argexec);
    }
    else
    {
      while ((wait(&status)) > 0);

      DIR *d;
      struct dirent *dir;
      chdir("/home/segara/SoalShiftSISOP20_modul2_E04/jpg/"); 
      d = opendir(".");
      if (d)
      {
        while ((dir = readdir(d)) != NULL)
        {
          pid_t child_4;
          child_4 = fork();

          if (child_4 == 0)
          {
            char source_file[1000];
            sprintf(source_file, 
              "/home/segara/SoalShiftSISOP20_modul2_E04/jpg/%s", 
              dir->d_name);

            if(isDirectory(dir->d_name))
            {
              if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0);
              else
              {
                char* argexec[] = {"mv", source_file,
                  "/home/segara/SoalShiftSISOP20_modul2_E04/indomie/", NULL};
                execv("/bin/mv", argexec);
              }
            }
            else
            {
              char* argexec[] = {"mv", source_file,
                "/home/segara/SoalShiftSISOP20_modul2_E04/sedaap/", NULL};
              execv("/bin/mv", argexec);
            }
          }
          // else
          // {
          //   while ((wait(&status)) > 0);
            
          //   if(isDirectory(dir->d_name))
          //   {
          //     pid_t child_5;
          //     child_5 = fork();

          //     char target_file[1000];
          //     FILE *target;

          //     if (child_5 == 0)
          //     {    
          //       sprintf(target_file, 
          //         "/home/segara/SoalShiftSISOP20_modul2_E04/indomie/%s/coba1.txt", 
          //         dir->d_name);
          //       target = fopen(target_file, "w");
          //       fclose(target);
          //     }
          //     else
          //     {
          //       while ((wait(&status)) > 0);
          //       sleep(3);
                
          //       sprintf(target_file, 
          //         "/home/segara/SoalShiftSISOP20_modul2_E04/indomie/%s/coba2.txt", 
          //         dir->d_name);
          //       target = fopen(target_file, "w");
          //       fclose(target);
          //     }
          //   }
          // }
        }
        closedir(d);
      }      
    }
  }
}