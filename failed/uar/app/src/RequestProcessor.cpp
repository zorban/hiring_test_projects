#include <stdio.h>
#include <string.h>
#include <sys/timeb.h>
#include <pthread.h>

#include "include/Utils.h"
#include "include/RequestProcessor.h"
#include "include/Logger.h"
#include "include/SoxInfoParser.h"
#include "include/RequestParser.h"
#include "include/AwsS3Manager.h"

using std::map;
using std::vector;
using std::string;
using std::to_string;

RequestProcessor::RequestProcessor () {}

RequestProcessor::~RequestProcessor () {}

int RequestProcessor::handleRequest (const char* request, char* response) {
  RequestParser* requestParser = new RequestParser(request);
  int httpStatusCode = 200;
  string _apiResponse = process(requestParser->getType(), requestParser->getPath(),
        requestParser->getQueryStringPairs(), &httpStatusCode);
  string apiResponse = Utils::getHttpGetResponse(httpStatusCode, _apiResponse);
  delete requestParser;
  sprintf(response, "%s", apiResponse.c_str());
  return apiResponse.size();
}

string RequestProcessor::process(const string& requestType, const string& path,
        map<string, string>& queryStringPairs, int* httpStatusCode) {
  if (requestType == "GET") {
    return handleGet(path, queryStringPairs, httpStatusCode);
  }
  else if (requestType == "POST") {
    goto p;
  }
  else {
p:
    return "Request type " + requestType + " not handled.\n";  
  }
  return "Unreachable place.\n";
}

string RequestProcessor::handleGet (const string& path,
      map<string, string>& queryStringPairs, int* httpStatusCode) {

  if (path == "/wav-info") {
    string wavFileName = queryStringPairs["wavkey"];
    if (wavFileName.size() == 0) {
      Logger::error("Wav file name is not defined.");
      *httpStatusCode = 400;
      return "";
    }
    return getWavInfo(wavFileName, httpStatusCode);
  
  } else if (path == "/mp3-to-wav") {
    return convertToWav(queryStringPairs["mp3key"], queryStringPairs["wavkey"],
        httpStatusCode);
  
  } else if(path == "/list-s3-objects") {
    return listS3Objects(httpStatusCode);
  
  } else {
    return "Path " + path + " not handled.\n";
  }
  return "";
}

string RequestProcessor::listS3Objects (int* httpStatusCode) {
  AwsS3Manager s3("uar-patrick-code-test", "AKIAJAE5BGTFOYZDVDUA",
      "X6vJwxiMJ6XXA0/FrQm4OU2gwgJ/GRMJA7ethhpl", "us-east-2",
      "cdt-03272018b");
  vector<string> objectList = s3.getObjectList();
  string outList;
  for (auto const &s3Object : objectList) {
    outList += s3Object + "\n";
  }
  return outList;
}

string RequestProcessor::getWavInfo (const string& wavKey,
      int* httpStatusCode) {
  string output;
  AwsS3Manager s3;
  string wavFileName(to_string(pthread_self()) + wavKey.substr(wavKey.rfind("/") + 1));
  string path("objectsStorage/" + wavFileName);
  Logger::log("Calling s3 get object api.");
  struct timeb startMoment, endMoment;
  ftime(&startMoment);
  bool status = s3.getObject(wavKey, path);
  if (status) {
    string cmd = "sox --i " + path;
    int exitStatus = 0;
    Logger::log("Running system command: \n" + cmd + "\n");
    string cmdOutput = Utils::getSysCmdOutput(cmd.c_str(), &exitStatus);
    ftime(&endMoment);
    double executionTime = (endMoment.time + (1.0 * endMoment.millitm) / 1000)
        - (startMoment.time + (1.0 * startMoment.millitm) / 1000);
    if (exitStatus == 0) {
      SoxInfoParser infoParser(cmdOutput, executionTime);
      output = infoParser.getInfoJson();
    } else {
      Logger::error("File info getting error.");
      *httpStatusCode = 500;
      output = "Error getting info for " + wavFileName + ".\n";
    }
  } else {
    *httpStatusCode = 404;
    output = "Error getting s3 object: " + wavKey + ".\n";
    Logger::error("Error getting s3 object: " + wavKey);
  }
  return output;
}

string RequestProcessor::convertToWav (const string& mp3Key,
    const string& wavKey, int* httpStatusCode) {
  string mp3FileName = mp3Key.substr(mp3Key.rfind("/") + 1);
  string wavFileName = wavKey.substr(wavKey.rfind("/") + 1);
  string output;
  string mp3Path("objectsStorage/" + to_string(pthread_self()) + mp3FileName);
  string wavPath("objectsStorage/" + to_string(pthread_self()) + wavFileName);
  AwsS3Manager s3;
  struct timeb startMoment, endMoment;
  ftime(&startMoment);
  bool status = s3.getObject(mp3Key, mp3Path);
  if (status) {
    int exitStatus = 0;
    string cmd("sox " + mp3Path + " " + wavPath);
    string cmdOutput = Utils::getSysCmdOutput(cmd.c_str(), &exitStatus);
    if (exitStatus == 0) {
      Logger::log("File conversion successfull.");
      bool uploadStatus = s3.putObject(wavPath, wavKey);
      if (uploadStatus) {
        Logger::log("File upload successfull.");
        long fileSize = Utils::filesize(wavPath.c_str());
        ftime(&endMoment);
        double executionTime = (endMoment.time + (1.0 * endMoment.millitm) / 1000)
            - (startMoment.time + (1.0 * startMoment.millitm) / 1000);
        output = Utils::getConversionInfoJson(fileSize, executionTime);
      } else {
        Logger::error("Error uploading file.");
        output = "Error uploading file.\n";
        *httpStatusCode = 500;
      }
    } else {
      *httpStatusCode = 500;
      output = "Error converting mp3 to wav.\n";
    }
  }
  return output;  
}
