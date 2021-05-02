#include <iostream>
#include <time.h>
#include "include/Logger.h"

int Logger::size = 512;
int Logger::entryIndex = 0;
int Logger::DEBUG = 5;
int Logger::INFO = 4;
int Logger::LOG = 3;
int Logger::WARNING = 2;
int Logger::ERROR = 1;

pthread_mutex_t Logger::loggerLock;


int Logger::level = Logger::LOG;

std::vector<struct logEntry> Logger::logs(Logger::size);

Logger::Logger () {}

Logger::~Logger () {}

void Logger::init () {
  pthread_mutex_init(&loggerLock, NULL);
}

void Logger::unInit () {
  pthread_mutex_destroy(&loggerLock);
}

void Logger::setLevel (int _level) { level = _level; }

void Logger::log (int _level, std::string message) {
  pthread_mutex_lock(&loggerLock);
  struct logEntry entry;
  entry.time = time(NULL);
  entry.level = _level;
  entry.message = message;
  logs[entryIndex] = entry;
  if (_level <= level) {
    std::cout << entry.time << ": " << message << std::endl;
  }
  if (++entryIndex == size) {
    entryIndex = 0;
  }
  pthread_mutex_unlock(&loggerLock);
}

void Logger::log (std::string message) {
  log(LOG, message);
}

void Logger::debug (std::string message) {
  log(DEBUG, message);
}

void Logger::info (std::string message) {
  log(INFO, message);
}

void Logger::warning (std::string message) {
  log(WARNING, message);
}

void Logger::error (std::string message) {
  log(ERROR, message);
}
