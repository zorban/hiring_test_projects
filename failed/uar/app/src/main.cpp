#include <stdlib.h>
#include <string.h>

#include "include/Server.h"
#include "include/Logger.h"

#define PORT 8089

int main (int argc, char** argv) {
  Logger::init();
  int port = PORT;

  if (argc > 2 && strcmp(argv[1], "-p") == 0) {
    port = atoi(argv[2]);
    if (port < 1 || port > 65534) {
      port = PORT;
      Logger::warning("Invalid port number supplied. Default will be used.");
    }
  }

  Server* server = Server::getInstance(port);
  if (server->init() != 0) {
    Logger::error("Error initialising server.");
    return EXIT_FAILURE;
  }
  server->start();

  Logger::unInit();
  return 0;
}
