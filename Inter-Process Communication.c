#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

#define RESP_NAME "RESP_PIPE_83579"
#define REQ_NAME "REQ_PIPE_83579"
#define CONNECT "CONNECT"
#define PING "PING"
#define PONG "PONG"
#define SHM_STR "CREATE_SHM"
#define SUCCESS "SUCCESS"
#define ERROR "ERROR"
#define MAP_FILE_STR "MAP_FILE"

unsigned int connect_size = strlen(CONNECT);
unsigned int size;
unsigned int variant = 83579;
unsigned int ping_size = strlen(PING);
unsigned int variant_size = sizeof(variant);
unsigned int success_size = strlen(SUCCESS);
unsigned int error_size = strlen(ERROR);
unsigned int shm_size = strlen(SHM_STR);
unsigned int map_file_size = strlen(MAP_FILE_STR);
char* data;
int loop = 1;

void ping(int fdw){
    write(fdw, &ping_size, 1);
    write(fdw, PING, ping_size);
    write(fdw, &ping_size, 1);
    write(fdw, PONG, ping_size);
    write(fdw, &variant, variant_size);
}

void exit_function(int fdw, int fdr,char* req){
    close(fdw);
    close(fdr);
    free(req);
    unlink(RESP_NAME);
    shm_unlink("/8m0v2W");
    loop = 0;
    exit(0);
}

void create_shm(int mem, int fdw){
	unsigned int shm_fd = shm_open("/8m0v2W", O_CREAT | O_RDWR, 0644);
	write(fdw, &shm_size, 1);
	write(fdw, SHM_STR, shm_size);
	if(shm_fd == -1){
		write(fdw, &error_size, 1);
		write(fdw, ERROR, error_size);
		shm_unlink("/8m0v2W");
		exit(0);
	}else{
		write(fdw, &success_size, 1);
		write(fdw, SUCCESS, success_size);
	}
	ftruncate(shm_fd, mem);
	data = (char*)mmap(NULL, mem, PROT_READ, MAP_SHARED, shm_fd, 0);
}

void map_file(char* file_name, int fdw){
	write(fdw, &map_file_size, 1);
	write(fdw, MAP_FILE_STR, map_file_size);
	unsigned int fd = open(file_name, O_RDONLY);
	unsigned int size = lseek(fd, 0, SEEK_END);
	data = (char*)mmap(NULL, size, O_RDONLY, MAP_PRIVATE, fd, 0);

	if(data == MAP_FAILED){
		write(fdw, &error_size, 1);
		write(fdw, ERROR, error_size);
        return;
	}
		
    write(fdw, &success_size, 1);
	write(fdw, SUCCESS, success_size);
	
}

int main(){
    if(mkfifo(RESP_NAME, 0666) != 0){
        printf("ERROR\ncannot create the response pipe\n");
        unlink(RESP_NAME);
    }

    unsigned int fdr = open(REQ_NAME, O_RDONLY);
    if(fdr == -1){
        printf("ERROR\ncannot open the request pipe\n");
        close(fdr);
        exit(2);
    }

    unsigned int fdw = open(RESP_NAME, O_WRONLY);
    if(fdw == -1){
        close(fdr);
        close(fdw);
        exit(1);
    }

    unsigned int check1 = write(fdw, &connect_size, 1);
    unsigned int check2 = write(fdw, CONNECT, connect_size); 
    if(check1 == 1 && check2 == connect_size){
        printf("SUCCESS\n");
    }

    while(loop){
        read(fdr, &size, 1);
        char* req = (char*)malloc(size * sizeof(char));
        read(fdr, req, size);

        if(strcmp(req, "PING") == 0){
           	ping(fdw);
        }

        if(strcmp(req, "EXIT") == 0){
            close(fdw);
            close(fdr);
            free(req);
            unlink(RESP_NAME);
            shm_unlink("/8m0v2W");
            loop = 0;
        }

        if(strcmp(req, "CREATE_SHM") == 0){
            unsigned int mem = 0;
  		    read(fdr, &mem, sizeof(mem));
  		    create_shm(mem, fdw);
        }

  	    if(strcmp(req, "MAP_FILE") == 0){
            unsigned int file_size = 0;
  		    read(fdr, &file_size, 1);
  		    char* file_name = (char*)malloc(file_size * sizeof(char));
            read(fdr, file_name, file_size);
  		    map_file(file_name, fdw);
  	    }

        exit(0);
    }

    return 0;
}
