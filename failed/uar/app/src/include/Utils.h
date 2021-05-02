#ifndef _UTILS_H_
#define _UTILS_H_

#include <fstream>
#include <string>

class Utils {
public:
  Utils();
  ~Utils();
  static std::ifstream::pos_type filesize(const char*);
  static std::string getSysCmdOutput(const char*, int*);
  static std::string getConversionInfoJson (long, double);
  static std::string& ltrim(std::string&);
  static std::string& rtrim(std::string&);
  static std::string& trim(std::string&);
  static std::string getHttpGetResponse(int, std::string&);
};

#endif // _UTILS_H_
