#ifndef _SERVER_H_
#define _SERVER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
#include <map>

#define LISTEN_BACKLOG 10
#define BUFFER_LENGTH 4096

struct connection {
  int fd;
  pthread_t tid;
  void* server;
  struct sockaddr_storage address;
};

class Server
{
private:
  int port;
  struct addrinfo addrinfoHints;
  struct addrinfo *addrinfoResults;
  struct addrinfo *addrinfoValid;
  int listenFd;
  struct sockaddr_storage address;
  struct sockaddr_storage clientAddress;
  int clientFd;
  socklen_t addressLength;
  socklen_t clientAddressLength;
  std::map<int, struct connection> connections;
  pthread_t tid;
  static Server* instance;

  Server ();
  Server (int);
  void logError (const char*, const char*, int);
  static void* handleClient (void*);

public:
  pthread_mutex_t connectionsLock;
  ~Server ();
  void addClientToList (struct connection);
  void removeClientFromList (int);
  static Server* getInstance (int);
  int init ();
  int start ();
};

#endif // _SERVER_H_
