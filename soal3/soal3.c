#include <dirent.h>
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/stat.h>
#include <sys/wait.h>

int isDirectory(const char *path)
{
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISDIR(path_stat.st_mode);
}

int main() 
{
  chdir("/home/segara/modul2"); 

  int n1 = fork();
  int n2 = fork(); 
  int status;

  close(STDERR_FILENO);

  if (n1 > 0 && n2 > 0) 
  { 
    while ((wait(&status)) > 0);

    DIR *d;
    struct dirent *dir;
    chdir("/home/segara/modul2/jpg/"); 
    d = opendir(".");
    if (d)
    {
      while ((dir = readdir(d)) != NULL)
      {
        if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
          continue;

        if(isDirectory(dir->d_name))
        {
          if(fork() == 0)
          {
            char source_file[1000];
            sprintf(source_file, 
              "/home/segara/modul2/jpg/%s", 
              dir->d_name);

            char* argexec[] = {"mv", source_file,
              "/home/segara/modul2/indomie/", NULL};
            execv("/bin/mv", argexec);
          }
          else
          {
            while ((wait(&status)) > 0);
            
            if(fork() == 0)
            {
              if(fork() == 0)
              {
                char target_file[1000];
                FILE *target;
                sprintf(target_file, 
                  "/home/segara/modul2/indomie/%s/coba1.txt", 
                  dir->d_name);
                target = fopen(target_file, "w");
                fclose(target);

                sleep(3);
                
                FILE *second_target;
                sprintf(target_file, 
                  "/home/segara/modul2/indomie/%s/coba2.txt", 
                  dir->d_name);
                second_target = fopen(target_file, "w");
                fclose(second_target);
                exit(0);
              }
            }
            else
            {
              while ((wait(&status)) > 0);
              exit(0);
            }
          }
        }
        else
        {
          while ((wait(&status)) > 0);
          if(fork() == 0)
          {
            while ((wait(&status)) > 0);

            char source_file[1000];
            sprintf(source_file, 
              "/home/segara/modul2/jpg/%s", 
              dir->d_name);

            char* argexec[] = {"mv", source_file,
              "/home/segara/modul2/sedaap/", NULL};
            execv("/bin/mv", argexec);
          }
        }        
      }
    }
  } 
  else if (n1 == 0 && n2 > 0) 
  { 
    while ((wait(&status)) > 0);
    pid_t child = fork();
    if (child == 0) 
    {
      char *argexec[] = {"mkdir", "-p", 
        "/home/segara/modul2/indomie", NULL};
      execv("/bin/mkdir", argexec);
    }
    else
    {
      while ((wait(&status)) > 0);
      sleep(5);

      char *argexec[] = {"mkdir", "-p", 
        "/home/segara/modul2/sedaap", NULL};
      execv("/bin/mkdir", argexec);
    }
  } 
  else if (n1 > 0 && n2 == 0) 
  { 
    char* argexec[] = {"unzip", "-oq", 
      "/home/segara/modul2/jpg.zip", NULL};
    execv("/usr/bin/unzip", argexec);
  }
  return 0; 
} 
