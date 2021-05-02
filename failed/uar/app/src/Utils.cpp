#include <stdio.h>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

#include "include/Utils.h"

using std::ifstream;
using std::string;
using std::to_string;
using std::find_if;
using std::not1;
using std::ptr_fun;
using std::isspace;

ifstream::pos_type Utils::filesize(const char* filename) {
  ifstream in(filename, ifstream::ate | ifstream::binary);
  return in.tellg(); 
}

string Utils::getSysCmdOutput(const char* cmd, int* exitStatus) {
  char buffer[128];
  std::string output = "";
  FILE* pipe = popen(cmd, "r");
  if (!pipe) {
    *exitStatus = -1;
    return output;
  }

  while (!feof(pipe)) {
    if (fgets(buffer, 128, pipe) != NULL) {
      output += buffer;
    }
  }

  *exitStatus = pclose(pipe);
  return output;
}

string Utils::getConversionInfoJson (long fileSize, double executionTime) {
  return "{\"file_size\":" + to_string(fileSize)
      + ",\"execution_time\":" + to_string(executionTime) + "}";
}

string& Utils::ltrim(string& s) {
  s.erase(s.begin(), find_if(s.begin(), s.end(),
      not1(ptr_fun<int, int>(isspace))));
  return s;
}

string& Utils::rtrim(string& s) {
  s.erase(find_if(s.rbegin(), s.rend(),
      not1(ptr_fun<int, int>(isspace))).base(), s.end());
  return s;
}

string& Utils::trim(string& s) {
  return ltrim(rtrim(s));
}

string Utils::getHttpGetResponse (int status, string& body) {
  string response;

  response += "HTTP/1.1 " + to_string(status) + "OK\n"
            + "Content-type: text/*; charset=UTF-8\n"
            + "Content-Length: " + to_string(body.size()) + "\n\n"
            + body;

  return response;
}
