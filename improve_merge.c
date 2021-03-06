#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

char *stringReverse(char *str);

int main(int argc, char *argv[])
{     
  int file1, file2;     
  FILE *fout;     
  long line1 = 0, line2 = 0, lineout = 0;     
  struct timeval before, after;     
  struct stat file1_stat, file2_stat;     
  char *buf_file1 = 0, *buf_file2 = 0;     
  char *file1_str, *file2_str, *file1_token, *file2_token, *file1_ptr, *file2_ptr;     
  int duration;     
  int ret = 1;     
  
  if (argc != 4) {         
    fprintf(stderr, "usage : %s file1 file2 fout\n", argv[0]);         
    goto leave0;     
  }     
  
  if ((file1 = open(argv[1], O_RDONLY)) == -1) {         
    perror(argv[1]);         
    goto leave0;     
  }     
  
  if ((file2 = open(argv[2], O_RDONLY)) == -1) {         
    perror(argv[2]);         
    goto leave1;     
  }     
  
  if ((fout = fopen(argv[3], "wt")) == NULL) {         
    perror(argv[3]);         
    goto leave2;     
  }     
  
  //check file state     
  if(fstat(file1, &file1_stat) < 0) {         
    fprintf(stderr, "error : read file system information\n");        
    goto leave3;     
  }     
  
  if(fstat(file2, &file2_stat) < 0) {         
    fprintf(stderr, "error : read file system information\n");         
    goto leave3;     
  }      
  
  //allocate buffer     
  if((buf_file1 = (char *)malloc(file1_stat.st_size)) == NULL) {         
    fprintf(stderr, "error : error occured while allocating memory of file1's buffer\n");         
    goto leave3;     
  }     
  
  if((buf_file2 = (char *)malloc(file2_stat.st_size)) == NULL) {         
    fprintf(stderr, "error : error occured while allocating memory of file2's buffer\n");         
    goto leave4;     
  }     
  
  //read file     
  if(read(file1, buf_file1, malloc_usable_size(buf_file1)) < 1) {         
    fprintf(stderr, "error : error occured while reading file1\n");         
    goto leave5;     
  }     
  
  if(read(file2, buf_file2, malloc_usable_size(buf_file2)) < 1) {         
    fprintf(stderr, "error : error occured while reading file2\n");         
    goto leave3;     
  }    
  
  gettimeofday(&before, NULL);    
  
  //write file with reverse string    
  for(file1_str = buf_file1, file2_str = buf_file2; ; file1_str = NULL, file2_str = NULL) {         
    file1_token = strtok_r(file1_str, "\n", &file1_ptr);         
    file2_token = strtok_r(file2_str, "\n", &file2_ptr);         
    if(file1_token != NULL) {                 
      fprintf(fout, "%s\n", stringReverse(file1_token));                 
      ++line1;         
    }         
    if(file2_token != NULL) {                 
      fprintf(fout, "%s\n", stringReverse(file2_token));                 
      ++line2;         
    }         
    if(file1_token != NULL && file2_token == NULL)                 
      break;         
  }    
  
  gettimeofday(&after, NULL);    
  
  lineout = line1 + line2;    
  duration = (after.tv_sec - before.tv_sec) * 1000000 + (after.tv_usec - before.tv_usec);    
  printf("Processing time = %d.%06d sec\n", duration / 1000000, duration % 1000000);    
  printf("File1 = %ld, File2 = %ld, Total = %ld Lines\n", line1, line2, lineout);    
  ret = 0;
  
leave5:     
  free(buf_file1);
leave4:    
  free(buf_file2);
leave3:     
  fclose(fout);
leave2:     
  close(file2);
leave1:     
  close(file1);
leave0:     
  return ret;
}

char *stringReverse(char *str)
{         
  char tmp;         
  char *begin = str;         
  char *end = str + strlen(str) - 1;         
  
  while(begin < end) {                 
    tmp = *begin;                 
    *begin = *end;                 
    *end = tmp;                 
    ++begin;                 
    --end;         
  }         
  return str;
}                                                                                                                           
