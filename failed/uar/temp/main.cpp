#include "main.h"

static void *handle_connection (void*);
char* getHexHost (in_addr_t*, char*);

int main (int argc, char** argv) {

  int listenfd, *iptr;
  pthread_t tid;
  socklen_t addrlen, len;
  struct sockaddr *cliaddr;
  struct sockaddr_in addr;

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd == -1) {
    char c[32];
    sprintf(c, "Error creating socket. errno: %d", errno);
    handle_error(c);
  }

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl (INADDR_ANY);
  addr.sin_port = htons (SERV_PORT);

  if (bind(listenfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    char c[32];
    sprintf(c, "Error binding socket. errno: %d", errno);
    handle_error(c);
  }

  addrlen = sizeof(addr);

  if (listen(listenfd, CONNMAX) == -1) {
    char c[32];
    sprintf(c, "Error making socket listening. errno: %d", errno);
    handle_error(c);
  }

  cliaddr = (struct sockaddr *) malloc(addrlen);
  while (1) {
    len = addrlen;
    iptr = (int*)malloc(sizeof(int));
    *iptr = accept(listenfd, cliaddr, &len);

    char str[9];
    printf("client ip: %s\n", getHexHost(&((struct sockaddr_in*)cliaddr)->sin_addr.s_addr, str));
    printf("client port: %d\n", ((struct sockaddr_in*)cliaddr)->sin_port);

    pthread_create(&tid, NULL, &handle_connection, iptr);
  }

  return 0;
}

static void* handle_connection (void* arg) {
  int connfd;
  struct sockaddr_storage peerAddr;
  socklen_t peerAddrLength;
  char buffer[BUFFERMAX];

  // memset((void*)&peerAddr, 0, sizeof(peerAddr));
  connfd = *((int*) arg);
  free(arg);

  pthread_detach(pthread_self());
  peerAddrLength = sizeof(struct sockaddr_storage);
  if (recvfrom(connfd, buffer, BUFFERMAX, 0,
    (struct sockaddr*)&peerAddr, &peerAddrLength) == -1) {
    char c[32];
    sprintf(c, "Error reading (recvfrom). errno: %d", errno);
    handle_error(c);
  }

  char host[NI_MAXHOST], port[NI_MAXSERV];
  getnameinfo((struct sockaddr *)&peerAddr, peerAddrLength,
    host, NI_MAXHOST, port, NI_MAXSERV, NI_NUMERICSERV);

  printf("Peer host: %s, port: %s.\n", host, port);
  printf("Received: %s.\n", buffer);

  const char* poruka = "Kosovo je srce srbije!\r\n";
  write(connfd, poruka, strlen(poruka));
  
  close(connfd);
  return (NULL);
}

char* getHexHost (in_addr_t* ipAddr, char* str) {
  str[8] = '\0';
  char* ip;
  ip = (char*)ipAddr;
  for (int i = 0; i < 4; ++i) {
    sprintf(str + i * 2, "%02X", ip[i]);
    printf("%o.", ip[i]);
  }
  return str;
}