#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <sys/sem.h>

int semId1;

void sig(int signum){
    printf("Signal\n");
    exit(0);
}

void P(int semid, int semnum, int value)
{
    struct sembuf op = {semnum, -value, 0};
    semop(semid, &op, 1);
}

void V(int semid, int semnum, int value)
{
    struct sembuf op = {semnum, value, 0};
    semop(semid, &op, 1);
}

void* thread_function9all(void* thread_id){
    int id = *(int*)thread_id;
    if(id == 1){
        info(BEGIN, 9, 1);
        V(semId1, 0, 1);
        P(semId1, 1, 1);
        info(END, 9, 1);
    }else if(id == 2){
        P(semId1, 0, 1);
        info(BEGIN, 9, id);
        info(END, 9, id);
        V(semId1, 1, 1);
    }else{
        info(BEGIN, 9, id);
        info(END, 9, id);
    }
    return NULL;
}

void* thread_function7(void* thread_id){
    signal(SIGSEGV, sig);
    int id = *(int*) thread_id;
    info(BEGIN, 7, id);
    info(END, 7, id);
    pthread_exit(0);
}

void* thread_function8(void* thread_id){
    int id = *(int*) thread_id;
    info(BEGIN, 8, id);
    info(END, 8, id);
    pthread_exit(0);
}

int main(){

    init();

    semId1 = semget(1000, 2, IPC_CREAT | 0666);
    semctl(semId1, 0, SETVAL, 1);
    semctl(semId1, 1, SETVAL, 1);
    pthread_t threads9[5], threads8[5];
    
    info(BEGIN, 1, 0);
    int status = 0;
    int pid = fork();
    wait(&status);
    if(pid == 0){
        info(BEGIN, 2, 0);
            for(int  i = 3; i <= 6; i++){
                if(i != 5){
                    int status2;
                    int pid2 = fork();
                    wait(&status2);
                    if(pid2 == 0){
                        info(BEGIN, i, 0);
                        if(i == 3){
                            int pid3 = fork();
                            int status3;
                            wait(&status3);
                            if(pid3 == 0){
                                info(BEGIN, 5, 0);
                                for(int j = 7; j <=9; j++){
                                    int status4;
                                    int pid4 = fork();
                                    wait(&status4);
                                    if(pid4 == 0){
                                        info(BEGIN, j, 0);
                                        // if(j == 7){
                                        //     for(int l = 1; l < 51; l++){
                                        //         pthread_create(&threads7[l - 1], NULL, thread_function7, &l);
                                        //         pthread_join(threads7[l - 1], NULL);
                                        //     }
                                        // }
                                        if(j == 8){
                                            for(int m = 1; m < 6; m++){
                                                pthread_create(&threads8[m - 1], NULL, thread_function8, &m);
                                                pthread_join(threads8[m - 1], NULL);
                                            }
                                        }
                                        if(j == 9){
                                            for(int k = 1; k < 6; k++){
                                                pthread_create(&threads9[k-1], NULL, thread_function9all, &k);
                                                    pthread_join(threads9[k - 1], NULL);
                                            }
                                        }
                                        info(END, j, 0);
                                        exit(0);
                                    }
                                }
                                info(END,5, 0);
                                exit(0);
                            }else{
                                info(END, 3, 0);
                                exit(0);
                            }
                        }
                        if(i == 4 || i == 6){
                            info(END, i, 0);
                            exit(0);
                        }
                    }
                }
            }
            
    }else{
        info(END, 2, 0);
        info(END, 1, 0);
    }
    
   signal(SIGSEGV, sig);

    return 0;
}
