#ifndef _REQUEST_PROCESSOR_H_
#define _REQUEST_PROCESSOR_H_

#include <map>
#include <string>

class RequestProcessor
{
private:
  std::string process(const std::string&, const std::string&,
      std::map<std::string, std::string>&, int*);

  std::string handleGet (const std::string&,
      std::map<std::string, std::string>&, int*);
  std::string getWavInfo (const std::string&, int*);
  std::string convertToWav (const std::string&, const std::string&, int*);
  std::string listS3Objects (int*);

public:
  RequestProcessor();
  ~RequestProcessor();

  int handleRequest(const char*, char*);
};

#endif // _REQUEST_PROCESSOR_H_
