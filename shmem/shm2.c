#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <string.h>
int main(){
    int shm12=shm_open("/shmem12",O_RDWR|O_CREAT,S_IRWXU);
    if(shm12==-1){
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    if(ftruncate(shm12,10)==-1){
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }
    char* str12=mmap(NULL,10,PROT_READ,MAP_SHARED,shm12,0);
    sem_t* sem12=sem_open("/sem12",O_RDWR|O_CREAT,S_IRWXU,0);
    sem_wait(sem12);
    printf("%s\n",str12);
    sem_post(sem12);
    int shm21=shm_open("/shmem21",O_RDWR|O_CREAT,S_IRWXU);
    if(shm21==-1){
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    if(ftruncate(shm21,10)==-1){
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }
    char* str21=mmap(NULL,10,PROT_WRITE,MAP_SHARED,shm21,0);
    sem_t* sem21=sem_open("/sem21",O_RDWR|O_CREAT,S_IRWXU,0);
    strcpy(str21,"Hi!");
    sem_post(sem21);
    sem_close(sem21);
    sem_unlink("/sem12");
    munmap(str12,10);
    munmap(str21,10);
    shm_unlink("/shmem12");
    exit(EXIT_SUCCESS);
}