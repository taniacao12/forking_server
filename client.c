#include "pipe_networking.h"

int main() {
  int to_server;
  int from_server;

  from_server = client_handshake (&to_server);
  char data[BUFFER_SIZE];
  while (1) {
    // gets data user input
    printf("ENTER COMMAND: ");
    fgets(data, BUFFER_SIZE, stdin);
    // sends data to server
    int w = write(to_server, data, BUFFER_SIZE);
    // gets reponse from server
    int r = read(from_server, data, BUFFER_SIZE);
    // displays response
    printf("[CLIENT] SERVER MESSEGE: %s", data);
  }
}
