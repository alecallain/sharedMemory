#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>


#define SHM_SIZE 1024

/**
* This class is for writing to shared
* memory using the lock step method.
*
* @author Allison Bolen
* @author Alec Allain
* @version 9/28/18
*/

/** Global variables */
char userInput;
int shmId;
char* shmPtr;
typedef struct {
  int turn;
  char message[500];
} memToken;
void sigintHandler (int sigNum);
/**
* Main method
*/
// writers trun is when its zero
int main() {
  key_t key = ftok("test.txt",65);
  printf("Key: %d\n",key);
  memToken token;
  signal(SIGINT, sigintHandler);
  token.turn = 0;

  if ((shmId = shmget (key, SHM_SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
     perror ("i can't get no..\n");
     exit (1);
  }
  // get the pointer
  if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
     perror ("can't attach\n");
     exit (1);
  }

  while(strcmp(token.message, "quit") != 0){
      while(token.turn == 1){
        // IT IS NOT THE WRITERS TURN
        // check for updates on turn so the writer can go again
        // if token.turn is updated to 0 by teh reader
        // we know that the reader read and we can write again
        //printf("Token message: '%s', Token turn: %d\n", token.message, token.turn);
        memcpy(&token, shmPtr, sizeof(memToken));
      }
        // writing this is the writers turn
        // critical section
        printf("Give me input: \n");
        fgets(token.message, sizeof(token.message), stdin);
        char *pos;
        if ((pos=strchr(token.message, '\n')) != NULL)
          *pos = '\0';
        token.turn = 1;
        // critical line
        memcpy(shmPtr, &token, sizeof(memToken));
        printf("We just wrote '%s' to the memory segment.\n", token.message);
   }
   kill(getpid(), SIGINT);
   return 0;
}
// detach and clean the shared memeory place
// detach and clean the shared memeory place
// this should be shut down first,  I did accidentally program in a "quit" functoinality so if a writer sends quit then the
// readers will quit before the writer will and they will detach and clean the memeory space before the writer does. so i commented out teh perrors because the
// memory does get detached but when we do it it wont work because its already been detached
// though so we should be good.
void sigintHandler (int sigNum){
  printf("Detaching and deleting.\n");
  // detach
  if (shmdt (shmPtr) < 0) {
     //perror ("just can't let go\n");
     exit (1);
  }
  // clean
  if (shmctl (shmId, IPC_RMID, 0) < 0) {
     //perror ("can't deallocate\n");
     exit(1);
  }
  exit(0);
}
