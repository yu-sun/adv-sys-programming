#include <stdio.h>#include <unistd.h>#include <string.h>#include <sys/time.h>#include <fcntl.h>#include <stdlib.h>#include <sys/types.h>#include <sys/stat.h>#define BUFSIZE 1024int readaline_and_out(int fin, int fout);intmain(int argc, char *argv[]){    int file1, file2, fout;    int eof1 = 0, eof2 = 0;    long line1 = 0, line2 = 0, lineout = 0;    struct timeval before, after;    int duration;    int ret = 1;    if (argc != 4) {        fprintf(stderr, "usage: %s file1 file2 fout\n", argv[0]);        goto leave0;    }    if ((file1 = open("argv[1]", O_RDONLY)) == -1) {        perror(argv[1]);        goto leave0;    }    if ((file2 = open("argv[2]", O_RDONLY)) == -1) {        perror(argv[2]);        goto leave1;    }    if ((fout = open("argv[3]", O_WRONLY)) == -1) {        perror(argv[3]);        goto leave2;    }        gettimeofday(&before, NULL);    do {        if (!eof1) {            if (!readaline_and_out(file1, fout)) {                line1++; lineout++;            } else                eof1 = 1;        }        if (!eof2) {            if (!readaline_and_out(file2, fout)) {                line2++; lineout++;            } else                eof2 = 1;        }    } while (!eof1 || !eof2);    gettimeofday(&after, NULL);        duration = (after.tv_sec - before.tv_sec) * 1000000 + (after.tv_usec - before.tv_usec);    printf("Processing time = %d.%06d sec\n", duration / 1000000, duration % 1000000);    printf("File1 = %ld, File2= %ld, Total = %ld Lines\n", line1, line2, lineout);    ret = 0;    leave3:    fclose(fout);leave2:    fclose(file2);leave1:    fclose(file1);leave0:    return ret; }/* Read a line from fin and write it to fout *//* return 1 if fin meets end of file */intreadaline_and_out(int fin, int fout){        int count = 0;    char buf[BUFSIZE];    char *a = "\n";    int arr[200], i=0;       memset(buf, 0x00, BUFSIZE);    do {        if ((read(fin, buf, 1)) == EOF) {            if (!count)                return 1;            else {                write(fout, "0x0a", 1);                break;            }        }        arr[count]=*buf;        count++;    } while (*buf != 0x0a);    for(i=count-1; i>0; i--){        write(fout, arr+i-1, 1);    }    write(fout, a, 1);    return 0;}
