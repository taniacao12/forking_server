#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <signal.h>

#ifndef NETWORKING_H
#define NETWORKING_H
#define ACK "HOLA"

#define HANDSHAKE_BUFFER_SIZE 10
#define BUFFER_SIZE 1000

static void sighandler (int signo);
int server_handshake(int *from_client, int *to_client, char *message);
int client_handshake (int *to_server);

void to_upper (char * data) {
  int i = 0;
  while (data[i]) {
    if (i%2) {
      // convert to uppercase
      data[i] = toupper(data[i]);
    }
    i++;
  }
}

#endif
