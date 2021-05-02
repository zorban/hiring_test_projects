#include "include/RequestParser.h"

using std::size_t;
using std::string;
using std::map;

RequestParser::RequestParser () {}

RequestParser::~RequestParser () {}

RequestParser::RequestParser (string _request) : request(_request) {
  parse();
}

void RequestParser::parse () {
  size_t pos = request.find(" ");
  type = request.substr(0, pos);
  query = request.substr(pos + 1, request.find(" ", pos + 1) - pos - 1);
  size_t qPos = query.find("?");
  path = query.substr(0, qPos);
  if (qPos != string::npos) {
    queryString = query.substr(qPos + 1);
  }

  pos = 0;
  size_t ampPos;
  do {
    ampPos = queryString.find("&", pos);
    string pairString = queryString.substr(pos, ampPos - pos);
    pos = ampPos + 1;
    size_t eqPos = pairString.find("=");
    string key = pairString.substr(0, eqPos);
    string value;
    if (eqPos != string::npos) {
      value = pairString.substr(eqPos + 1);
    }
    if (key.size()) {
      queryStringPairs[key] = value;
    }
  } while (ampPos != string::npos);
}

string& RequestParser::getType () {
  return type;
}

string& RequestParser::getPath () {
  return path;
}

string& RequestParser::getQueryString () {
  return queryString;
}

map<string, string>& RequestParser::getQueryStringPairs () {
  return queryStringPairs;
}
