#include <sstream>
#include "include/Utils.h"
#include "include/SoxInfoParser.h"

using std::string;
using std::stringstream;
using std::to_string;
using std::stoi;
using std::stod;
using std::size_t;

SoxInfoParser::SoxInfoParser () {}

SoxInfoParser::~SoxInfoParser () {}

SoxInfoParser::SoxInfoParser (string& rawInfo, double executionTime) :
    rawInfo(rawInfo), executionTime(executionTime) {
      sampleRate = -1;
      channelCount = -1;
      parse();
    }

void SoxInfoParser::parse () {
  stringstream ss;
  string line;
  ss.str(rawInfo);
  bool channelCountSet = false;
  bool sampleRateSet = false;
  while (!(channelCountSet && sampleRateSet) && getline(ss, line)) {
    if (line.find("Channels") != string::npos) {
      channelCount = stoi(line.substr(line.find(": ") + 2));
      channelCountSet = true;
    } else if (line.find("Sample Rate") != string::npos) {
      sampleRate = stoi(line.substr(line.find(": ") + 2));
      sampleRateSet = true;
    }
  }
}

string SoxInfoParser::getInfoJson () {
  return "{\"channel_count\":" + to_string(channelCount) 
      + ",\"sample_rate\":" + to_string(sampleRate)
      + ",\"execution_time\":" + to_string(executionTime) + "}";
}

/*
Input File     : 'objectsStorage/test_102.mp3'
Channels       : 1
Sample Rate    : 22050
Precision      : 16-bit
Duration       : 00:00:40.05 = 883080 samples ~ 3003.67 CDDA sectors
File Size      : 160k
Bit Rate       : 32.0k
Sample Encoding: MPEG audio (layer I, II or III)
*/
