#ifndef _AWS_S3_MANAGER_H_
#define _AWS_S3_MANAGER_H_

#include <string>
#include <vector>

class AwsS3Manager {
private:
  std::string bucketName;
  std::string id;
  std::string key;
  std::string region;
  std::string subDirectory;

public:
  static int useCount;

  AwsS3Manager();
  ~AwsS3Manager();
  AwsS3Manager(std::string, std::string, std::string, std::string,
      std::string);

  std::vector<std::string> getObjectList ();
  bool getObject (std::string, std::string);
  bool putObject (std::string, std::string);
  bool deleteObject (std::string);
};

#endif // _AWS_S3_MANAGER_H_
