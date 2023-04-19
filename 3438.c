#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
int main(){
  int fd;
  int count;
  char string[100];
  while(1){
  printf("Input string\n");
  gets(string);
  
  fd = open("/dev/comp3438_exam", O_WRONLY | O_TRUNC);
  if (fd == -1){
    printf("Fail to open device exam.\n");
    exit(-1);
  }
  write(fd,string,strlen(string));
  }
  return 0;
}
