#ifndef _REQUEST_PARSER_H_
#define _REQUEST_PARSER_H_
#include <string>
#include <map>

class RequestParser {
private:
  std::string request;
  std::string type;
  std::string query;
  std::string path;
  std::string queryString;
  std::map<std::string, std::string> queryStringPairs;

  void parse ();

public:
  RequestParser();
  ~RequestParser();
  RequestParser(std::string);

  std::string& getType();
  std::string& getPath();
  std::string& getQueryString();
  std::map<std::string, std::string>& getQueryStringPairs();
};

#endif // _REQUEST_PARSER_H_
