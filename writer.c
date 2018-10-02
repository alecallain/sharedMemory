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

/** Global variables */
char userInput;
int shmId;
char* shmPtr;
struct shmid_ds buffer;
typedef struct {
  int turn;
  char message[500];
} memToken;

/**
* Main method
*/
int main() {
  key_t key = ftok("shmfile",65);
  printf("Key: %d",key);
  memToken token;
  int shmId;
  char *shmPtr;
  printf("give me input\n");
  fgets(token.message, sizeof(token.message), stdin);
  token.turn = 0;

    while(1){
     // set up a segment
     if ((shmId = shmget (key, sizeof(memToken), IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
        perror ("i can't get no..\n");
        exit (1);
     }
     // get the pointer
     if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
        perror ("can't attach\n");
        exit (1);
     }
     memcpy(shmPtr, &token, sizeof(memToken));
     memToken tok2;
     memcpy(&tok2, shmPtr, sizeof(memToken));

     printf("STUFF%s\n", tok2.message);


   }
   // detach
   if (shmdt (shmPtr) < 0) {
      perror ("just can't let go\n");
      exit (1);
   }
   // clean
   if (shmctl (shmId, IPC_RMID, 0) < 0) {
      perror ("can't deallocate\n");
      exit(1);
   }

   return 0;
}
