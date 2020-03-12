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
	int n1 = fork();
	int n2 = fork(); 
  int status;

  close(STDERR_FILENO);

	if (n1 > 0 && n2 > 0) 
  { 
    while ((wait(&status)) > 0);

    DIR *d;
    struct dirent *dir;
    chdir("/home/segara/SoalShiftSISOP20_modul2_E04/soal3/jpg/"); 
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
              "/home/segara/SoalShiftSISOP20_modul2_E04/soal3/jpg/%s", 
              dir->d_name);

            char* argexec[] = {"mv", source_file,
              "/home/segara/SoalShiftSISOP20_modul2_E04/soal3/indomie/", NULL};
            execv("/bin/mv", argexec);
            // puts(dir->d_name);
            // char* argexec[] = {"bash", "/home/segara/SoalShiftSISOP20_modul2_E04/soal3/move.sh", NULL};
            // execv("/bin/bash", argexec);
          }
          else
          {
            while ((wait(&status)) > 0);
            
            if(fork() == 0)
            {
              if(fork() == 0)
              {
                // char* argexec[] = {"bash", "/home/segara/SoalShiftSISOP20_modul2_E04/soal3/1.sh", NULL};
                // execv("/bin/bash", argexec);
                char target_file[1000];
                FILE *target;
                sprintf(target_file, 
                  "/home/segara/SoalShiftSISOP20_modul2_E04/soal3/indomie/%s/coba1.txt", 
                  dir->d_name);
                target = fopen(target_file, "w");
                fclose(target);
              }
              else
              {
                while ((wait(&status)) > 0);
                sleep(3);
                // char* argexec[] = {"bash", "/home/segara/SoalShiftSISOP20_modul2_E04/soal3/2.sh", NULL};
                // execv("/bin/bash", argexec);
                char target_file[1000];
                FILE *target;
                sprintf(target_file, 
                  "/home/segara/SoalShiftSISOP20_modul2_E04/soal3/indomie/%s/coba2.txt", 
                  dir->d_name);
                target = fopen(target_file, "w");
                fclose(target);
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
              "/home/segara/SoalShiftSISOP20_modul2_E04/soal3/jpg/%s", 
              dir->d_name);

            char* argexec[] = {"mv", source_file,
              "/home/segara/SoalShiftSISOP20_modul2_E04/soal3/sedaap/", NULL};
            execv("/bin/mv", argexec);
            // puts(dir->d_name);
            // char* argexec[] = {"bash", "/home/segara/SoalShiftSISOP20_modul2_E04/soal3/move.sh", NULL};
            // execv("/bin/bash", argexec);
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
        "/home/segara/SoalShiftSISOP20_modul2_E04/soal3/indomie", NULL};
      execv("/bin/mkdir", argexec);
      // char* argexec[] = {"bash", "/home/segara/SoalShiftSISOP20_modul2_E04/soal3/mkdiri.sh", NULL};
      // execv("/bin/bash", argexec);
    }
    else
    {
      while ((wait(&status)) > 0);
      sleep(5);

      char *argexec[] = {"mkdir", "-p", 
        "/home/segara/SoalShiftSISOP20_modul2_E04/soal3/sedaap", NULL};
      execv("/bin/mkdir", argexec);
      // char* argexec[] = {"bash", "/home/segara/SoalShiftSISOP20_modul2_E04/soal3/mkdir.sh", NULL};
      // execv("/bin/bash", argexec);
    }
  } 
	else if (n1 > 0 && n2 == 0) 
	{ 
    char* argexec[] = {"unzip", "-oq", 
      "/home/segara/SoalShiftSISOP20_modul2_E04/soal3/jpg.zip", NULL};
    execv("/usr/bin/unzip", argexec);

    // char* argexec[] = {"bash", "/home/segara/SoalShiftSISOP20_modul2_E04/soal3/unzip.sh", NULL};
    // execv("/bin/bash", argexec);
	}
	return 0; 
} 