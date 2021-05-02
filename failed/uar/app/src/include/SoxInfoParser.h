#ifndef _SOX_INFO_PARSER_H_
#define _SOX_INFO_PARSER_H_

#include <string>

class SoxInfoParser {
private:
  std::string rawInfo;
  int channelCount;
  int sampleRate;
  double sampleCount;
  long fileSize;
  double executionTime;

  void parse ();

public:
  SoxInfoParser ();
  ~SoxInfoParser ();
  SoxInfoParser (std::string&, double);

  std::string getInfoJson ();  
};

#endif // _SOX_INFO_PARSER_H_
