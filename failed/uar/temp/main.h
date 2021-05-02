#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
// #include <sys/un.h>
#include <string.h>

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define SERV_PORT 8080
#define CONNMAX 10
#define BUFFERMAX 1024
