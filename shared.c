#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <string.h>

#define SHM_SIZE 1024

/**
* This class is for practice with reading
* and writing to shared memory
*
* @author Allison Bolen
* @author Alec Allain
* @version 9/28/18
*/

/** Instanciate methods */
void* reader (void* input);
void* writer (void* input);

/** Global variables */
pthread_t thread[50];
int status;
char userInput;
int shmId;
char* shmPtr;
struct shmid_ds buffer;

/**
* Main method
*/
int main() {

    while (1) {
        printf("\n\nUser: ");
        scanf("%s", &userInput);

        if (strcmp(&userInput, "quit") == 0) {
            exit(0);
        }

        if ((shmId = shmget(IPC_PRIVATE, sizeof(userInput), SHM_SIZE | IPC_CREAT)) < 0) {
            fprintf(stderr, "shmget error");
            exit(1);
        }
        // detatches memory segment
        if (shmctl(shmId, IPC_RMID, 0) < 0) {
            fprintf(stderr, "Cannot deallocate memory");
            exit(1);
        }
    }
    return 0;
}

/**
* This method allows processes to read the latest
* changes to shared memory
*/
void* reader (void* input) {
    return NULL;
}

/**
* This method writes new information into
* shared memeory
*/
void* writer (void* input) {

    return NULL;
}
