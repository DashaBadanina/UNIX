#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <signal.h>
#include <sys/sem.h>
#define SEGSIZE	100

void writeshm (int shmid, char *segptr, char *text);
void readshm(int shmid, char *segptr);
void removeshm(int shmid);
void changemode(int shmid, char *mode);
void usage(void);
void workwithsem(void);

key_t key;
int keysem;
int id;
int shmid;
char *segptr;
struct sembuf sem_lock;
struct sembuf sem_lock2;
int F;
int main(int argc, char *argv[]) {

 if (argc == 1)
   {
     printf("Введите количество потокв\n");
     exit(1);
   }
  else
  {
  F=atoi(argv[1]);
  }
        const int ESC_CODE = 27;
        const int RETURN_CODE = 10;
        const int MESSAGE_LENGTH = 80;
        char buf[MESSAGE_LENGTH + 1];
        int c = 0;
        int i = 0;
        pid_t pid; 
	/* Create unique key via call to ftok() */
	key = ftok(".", 'S');
        keysem = ftok(".",'l');
        /*Create semaphor*/
        id = semget(keysem, 2, 0666 | IPC_CREAT);
            // if (F==1)
     //semctl(id, 1, SETVAL, 0);     
	sem_lock.sem_num = 0;
	sem_lock.sem_op = 0;
	sem_lock.sem_flg = 0;

        sem_lock2.sem_num = 1;
	sem_lock2.sem_op = 1;
	sem_lock2.sem_flg = 0;
        printf("Привет\n");
	/* Open the shared memory segment - create if necessary */
	if ((shmid = shmget(key, SEGSIZE, IPC_CREAT | IPC_EXCL | 0666)) == -1) {

		/* Segment probably already exists - try as a client */
		if ((shmid = shmget(key, SEGSIZE, 0)) == -1) {
			perror("shmget");
			exit(1);
		}
	} else {
		printf("Creating new shared memory segment\n");
	}

	/* Attach (map) the shared memory segment into the current process */
	if ((segptr = shmat(shmid, 0, 0)) == (char *) -1) {
		perror("shmat");
		exit(1);
	}

       pid = fork();
       if (pid == 0) {
        workwithsem();
       } else 
     {
        
  	while (c != ESC_CODE) 
       {
   	 c = getchar();
	if (i < MESSAGE_LENGTH) {
     	 buf[i++] = c;
		} else {
      	printf("Very big message!\n");
		}
	if (c == RETURN_CODE) 
        {
	   if (i > 1) 
          {
	    	buf[i] = 0;
               sem_lock.sem_op = 0;
               semop(id, &sem_lock, 1);
               sem_lock.sem_op = 1;
               semop(id, &sem_lock, 1);
               writeshm(shmid, segptr, buf);
               sem_lock2.sem_op = -2;
               semop(id, &sem_lock2, 1);
               sem_lock.sem_op = -1;
               semop(id, &sem_lock, 1);
	  }
	  i = 0;
	}
    }
     kill(pid, SIGKILL);
     sem_lock2.sem_op = -1;
 semop(id, &sem_lock2, 1);
  }
	return (0);
}

void workwithsem()
{
sem_lock2.sem_op = 1;
 semop(id, &sem_lock2, 1);
 sem_lock2.sem_op = 0;
printf("Plus Odin\n");
  while(1)
  {
  semop(id, &sem_lock2, 1);
  readshm(shmid,segptr);
  sem_lock2.sem_op = 1;
  semop(id, &sem_lock2, 1);
  sem_lock2.sem_op = 0;
  }
}

void writeshm(int shmid, char *segptr, char *text) {
	strcpy(segptr, text);
}

void readshm(int shmid, char *segptr) {
	printf(" %s\n", segptr);
}

void removeshm(int shmid) {
	shmctl(shmid, IPC_RMID, 0);
	printf("Shared memory segment marked for deletion\n");
}

