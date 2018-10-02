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
// writers trun is when its zero
int main() {
  key_t key = ftok("test.txt",65);
  printf("Key: %d\n",key);
  memToken token;
  int shmId;
  char *shmPtr;

  token.turn = 0;
  if ((shmId = shmget (key, sizeof(SHM_SIZE), IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
     perror ("i can't get no..\n");
     exit (1);
  }
  // get the pointer
  if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
     perror ("can't attach\n");
     exit (1);
  }

  while(strcmp(token.message, "quit") != 0){
      printf("Give me input: \n");
      fgets(token.message, sizeof(token.message), stdin);

      char *pos;
      if ((pos=strchr(token.message, '\n')) != NULL)
        *pos = '\0';

      while(token.turn == 1){
        // IT IS NOT THE WRITERS TURN
        // check for updates on turn so the writer can go again
        // if token.turn is updated to 0 by teh reader
        // we know that the reader read and we can write again
        memcpy(&token, shmPtr, sizeof(memToken));
        printf("Token message: %s, Token turn: %d\n", token.message, token.turn);
      }
        // writing this is the writers turn
        // critical section
        token.turn = 1;
        memcpy(shmPtr, &token, sizeof(memToken));
        printf("We just wrote %s to the memory segment.\n", token.message);
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
