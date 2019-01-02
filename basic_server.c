#include "pipe_networking.h"

int main() {
  signal(SIGINT, sighandler);
  while (1) {
    // create well known pipe
    int wkp = mkfifo("main", 0644);
    if (wkp == - 1) // errno message
      printf("ERROR: %d --> %s\n", errno, strerror(errno));
    else printf("[SERVER] CREATED WELL KNOWN PIPE\n");
    // wait
    int receive = open("main", O_RDONLY);
    // receive message
    char msg[HANDSHAKE_BUFFER_SIZE];
    read(receive, msg, HANDSHAKE_BUFFER_SIZE);
    printf("[SERVER] CLIENT MESSAGE: %s\n", msg);
    // fork subserver
    int f = fork();
    if (f) { // parent
      // remove well known pipe
      remove("main");
      close(receive);
      printf("[SERVER] REMOVED WELL KNOWN PIPE\n");
    }
    else {
      int to_client;
      int from_client = wkp;
      char data[BUFFER_SIZE];

      server_handshake (from_client, &to_client, message);
      // get data from client
      while (read(from_client, data, BUFFER_SIZE)) {
        printf("[subserver %d] received: %s", getpid(), data);
        // process data
        to_upper(data);
        // reply to client
        int w = write(to_client, data, BUFFER_SIZE);
      }
    
      close(to_client);
      close(from_client);
      printf("[SUBSERVER %d] SESSION COMPLETE\n", getpid());
      return 0;
    }

    static void sighandler(int signo) {
      if (signo == SIGINT) {
        remove("main");
        printf("wkp removed\n");
        exit(0);
      }
    }
