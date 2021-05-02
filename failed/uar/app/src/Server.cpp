#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "include/Server.h"
#include "include/Logger.h"
#include "include/RequestProcessor.h"

using std::to_string;

Server* Server::instance;

Server::Server () {}

Server::~Server () {
  Logger::log("Server destructor...");
  pthread_mutex_destroy(&connectionsLock);
}

Server::Server (int port) : port(port) {
  pthread_mutex_init(&connectionsLock, NULL);
}

void Server::addClientToList (struct connection client) {
  pthread_mutex_lock(&connectionsLock);
  connections[client.fd] = client;
  pthread_mutex_unlock(&connectionsLock);
}

void Server::removeClientFromList(int fd) {
  pthread_mutex_lock(&connectionsLock);
  connections.erase(fd);
  pthread_mutex_unlock(&connectionsLock);
}

Server* Server::getInstance (int port) {
  if (instance == NULL) {
    instance = new Server(port);
  }
  return instance;
}

void Server::logError (const char* source, const char* errorMessage, int _errno) {
  char message[128];
  if (_errno > -1) {
    sprintf(message, "%s: %s", source, strerror(_errno));
  } else {
    sprintf(message, "%s: %s", source, errorMessage);
  }
  Logger::error(message);
}

int Server::init () {
  memset(&addrinfoHints, 0, sizeof(struct addrinfo));
  addrinfoHints.ai_family = AF_UNSPEC; 
  addrinfoHints.ai_socktype = SOCK_STREAM;
  addrinfoHints.ai_flags = AI_PASSIVE;
  addrinfoHints.ai_protocol = 0;
  addrinfoHints.ai_canonname = NULL;
  addrinfoHints.ai_addr = NULL;
  addrinfoHints.ai_next = NULL;

  char strPort[6];
  sprintf(strPort, "%d", port);
  int s = getaddrinfo(NULL, strPort, &addrinfoHints, &addrinfoResults);
  if (s != 0) {
    logError("getaddrinfo", gai_strerror(s), -1);
    return EXIT_FAILURE;
  }

  for (addrinfoValid = addrinfoResults; addrinfoValid != NULL; 
    addrinfoValid = addrinfoValid->ai_next) {
    listenFd = socket(addrinfoValid->ai_family, addrinfoValid->ai_socktype,
      addrinfoValid->ai_protocol);
    if (listenFd == -1) {
      continue;
    }
    if (bind(listenFd, addrinfoValid->ai_addr, addrinfoValid->ai_addrlen) == 0) {
      break;
    }
    close(listenFd);
    logError("bind", "", errno);
    return EXIT_FAILURE;
  }

  if (addrinfoValid == NULL) {
    logError("getaddrinfo", "No valid results.", -1);
    return EXIT_FAILURE;
  }

  freeaddrinfo(addrinfoResults);

  if (listen(listenFd, LISTEN_BACKLOG) == -1) {
    logError("listen", "", errno);
    return EXIT_FAILURE;
  }

  Logger::log("Server initialization succeeded");

  return 0;
}

int Server::start () {
  Logger::log("Starting server on port " + to_string(port));
  clientAddressLength = sizeof(struct sockaddr_storage);

  while (1) {
    clientFd = accept(listenFd, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (clientFd == -1) {
      logError("accept", "", errno);
      return EXIT_FAILURE;      
    }

    struct connection* client = (struct connection*)malloc(sizeof(struct connection));
    client->fd = clientFd;
    client->address = clientAddress;
    client->server = (void*)this;

    pthread_create(&tid, NULL, &handleClient, (void*)client);

  }

  return 0;
}

void* Server::handleClient (void* _client) {
  struct connection* client = (struct connection*)_client;
  client->tid = pthread_self();
  int connFd = client->fd;
  Server* server = (Server*)client->server;
  int ptdRet;
  if ((ptdRet = pthread_detach(pthread_self())) != 0) {
    Logger::error("Server::handleClient thread: Cannot detach.");
    return (void *)NULL;
  }
  server->addClientToList(*client);

  char message[64];
  sprintf(message, "Handling connection, id: %ld, connection fd: %d.",
    client->tid, connFd);
  Logger::log(message);

  char requestBuffer[BUFFER_LENGTH];
  int receivedBytesLength = 0;
  if ((receivedBytesLength = recv(connFd, requestBuffer
      , sizeof(requestBuffer), 0)) == -1) {
    server->logError("recv", "", errno);
    return (void *)NULL;
  }
  requestBuffer[receivedBytesLength] = '\0';

  char sendBuffer[BUFFER_LENGTH];
  RequestProcessor* requestProcessor = new RequestProcessor();
  int size = requestProcessor->handleRequest(requestBuffer, sendBuffer);
  if (send(connFd, sendBuffer, size, 0) == -1) {
    server->logError("send", "", errno);
    return (void *)NULL;
  }

  close(connFd);
  sprintf(message, "Connection fd: %d closed.", connFd);
  Logger::log(message);
  free(_client);
  delete requestProcessor;
  server->removeClientFromList(client->fd);
  
  return (void *)NULL;
}
