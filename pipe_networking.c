#include "pipe_networking.h"

/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *from_client, int *to_client, char *message) {
  // connect to private FIFO
  *to_client = open(msg, O_WRONLY);
  // send initial message
  int w = write(*to_client, ACK, HANDSHAKE_BUFFER_SIZE);
  // get response from client
  read(from_client, msg, HANDSHAKE_BUFFER_SIZE);
  printf("[SUBSERVER %d] CLIENT MESSAGE: %s\n", getpid(), msg);
  // return fd to wkp
  printf("[SUBSERVER %d] HANDSHAKE COMPLETE\n", getpid());
}

/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  // create private FIFO
  char pipe_name[HANDSHAKE_BUFFER_SIZE];
  sprintf(pipe_name, "%d", getpid());
  int private = mkfifo(pipe_name, 0644);
  if (private == - 1) {
    printf("error %d: %s\n", errno, strerror(errno));
    return 1;
  }
  // set to_server to well known pipe
  *to_server = open("main", O_WRONLY);
  if (*to_server == - 1) {// errno message
    printf("ERROR: %d --> %s\n", errno, strerror(errno));
    remove(pipe_name);
    return 1;
  }
  // send private FIFO to server
  int w = write(*to_server, pipe_name, HANDSHAKE_BUFFER_SIZE);
  printf("[CLIENT] SENT PRIVATE PIPE TO SERVER\n");
  // wait for response
  int receive = open(pipe_name, O_RDONLY);
  // receive server message
  char msg[HANDSHAKE_BUFFER_SIZE];
  int r = read(receive, msg, HANDSHAKE_BUFFER_SIZE);
  printf("[CLIENT] SERVER MESSAGE: %s\n", msg);
  // remove private FIFO
  remove(pipe_name);
  printf("[CLIENT] REMOVED PRIVATE PIPE\n");
  // send response to server
  w = write(*to_server, ACK, HANDSHAKE_BUFFER_SIZE);
  // return fd to private fifo
  return receive;
}
