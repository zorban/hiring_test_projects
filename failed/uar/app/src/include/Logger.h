#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <pthread.h>

#include <vector>
#include <string>

struct logEntry {
  time_t time;
  int level;
  std::string message;
};

class Logger
{
public:
  static int size;
  static int entryIndex;
  static int level;
  static int DEBUG;
  static int INFO;
  static int LOG;
  static int WARNING;
  static int ERROR;
  static std::vector<struct logEntry> logs;
  static pthread_mutex_t loggerLock;

  Logger();
  ~Logger();

  static void init ();
  static void unInit ();
  static void setLevel (int);
  static void log (int, std::string);
  static void log (std::string);
  static void debug (std::string);
  static void info (std::string);
  static void warning (std::string);
  static void error (std::string);
};

#endif // _LOGGER_H_
