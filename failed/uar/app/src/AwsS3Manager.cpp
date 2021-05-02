#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/Object.h>
#include <aws/s3/model/ListObjectsRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <fstream>

#include "include/AwsS3Manager.h"
#include "include/Logger.h"

using std::string;
using std::vector;

int AwsS3Manager::useCount = 0;

AwsS3Manager::AwsS3Manager() {
  bucketName = "uar-patrick-code-test";
  id = "AKIAJAE5BGTFOYZDVDUA";
  key = "X6vJwxiMJ6XXA0/FrQm4OU2gwgJ/GRMJA7ethhpl";
  region = "us-east-2";
  subDirectory = "cdt-03272018b";
}

AwsS3Manager::~AwsS3Manager() {}

AwsS3Manager::AwsS3Manager (string bucketName, string id, string key, 
    string region, string subDirectory) : bucketName(bucketName),
    id(id), key(key), region(region), subDirectory(subDirectory) {}

vector<string> AwsS3Manager::getObjectList () {
  vector<string> objectList;
  Aws::SDKOptions options;
  if (useCount == 0) {
    Aws::InitAPI(options);
  }
  ++useCount;
  {
    const Aws::String bucket_name = bucketName.c_str();

    Aws::Client::ClientConfiguration clientConfig;
    clientConfig.region = region.c_str();
    Aws::S3::S3Client s3_client(clientConfig);

    Aws::S3::Model::ListObjectsRequest objects_request;
    objects_request.WithBucket(bucket_name);

    auto list_objects_outcome = s3_client.ListObjects(objects_request);

    if (list_objects_outcome.IsSuccess()) {
      Aws::Vector<Aws::S3::Model::Object> object_list =
      list_objects_outcome.GetResult().GetContents();
      for (auto const &s3_object : object_list) {
        objectList.push_back(s3_object.GetKey().c_str());
      }
    } else {
      string exceptionName(list_objects_outcome.GetError().GetExceptionName().c_str());
      string exceptionMessage(list_objects_outcome.GetError().GetMessage().c_str());
      Logger::error("listObjects error:\n" + exceptionName + " " + exceptionMessage);
    }
  }
  if (useCount == 1) {
    Aws::ShutdownAPI(options);
  }
  --useCount;
  return objectList;
}

bool AwsS3Manager::getObject (string remoteFileName, string localFileName) {
  bool status = true;
  Aws::SDKOptions options;
  if (useCount == 0) {
    Aws::InitAPI(options);
  }
  ++useCount;
  {
    const Aws::String bucket_name = bucketName.c_str();
    const Aws::String key_name = (subDirectory + "/" + remoteFileName).c_str();
    const Aws::String awsLocalFileName = localFileName.c_str();

    Aws::Client::ClientConfiguration clientConfig;
    clientConfig.region = region.c_str();
    Aws::S3::S3Client s3_client(clientConfig);

    Aws::S3::Model::GetObjectRequest object_request;
    object_request.WithBucket(bucket_name).WithKey(key_name);

    auto get_object_outcome = s3_client.GetObject(object_request);

    if (get_object_outcome.IsSuccess()) {
      Aws::OFStream local_file;
      local_file.open(awsLocalFileName.c_str(), std::ios::out | std::ios::binary);
      local_file << get_object_outcome.GetResult().GetBody().rdbuf();
      Logger::log("getObject: Done!");
    } else {
      string exceptionName(get_object_outcome.GetError().GetExceptionName().c_str());
      string exceptionMessage(get_object_outcome.GetError().GetMessage().c_str());
      Logger::error("getObject error:\n" + exceptionName + " " + exceptionMessage);
      status = false;
    }
  }
  if (useCount == 1) {
    Aws::ShutdownAPI(options);
  }
  --useCount;
  return status;
}

bool AwsS3Manager::putObject (string localFileName, string remoteFileName) {
  bool status = true;
  Aws::SDKOptions options;
  if (useCount == 0) {
    Aws::InitAPI(options);
  }
  ++useCount;
  {
    const Aws::String bucket_name = bucketName.c_str();
    const Aws::String key_name = (subDirectory + "/" + remoteFileName).c_str();
    const Aws::String file_name = localFileName.c_str();

    Aws::Client::ClientConfiguration clientConfig;
    clientConfig.region = region.c_str();
    Aws::S3::S3Client s3_client(clientConfig);

    Aws::S3::Model::PutObjectRequest object_request;
    object_request.WithBucket(bucket_name).WithKey(key_name);

    // Binary files must also have the std::ios_base::bin flag or'ed in
    auto input_data = Aws::MakeShared<Aws::FStream>("PutObjectInputStream",
      file_name.c_str(), std::ios_base::in | std::ios_base::binary);

    object_request.SetBody(input_data);
    auto put_object_outcome = s3_client.PutObject(object_request);

    if (put_object_outcome.IsSuccess()) {
      Logger::log(localFileName + " successfully uploaded as remote " 
        + subDirectory + "/" + remoteFileName);
    } else {
      string exceptionName(put_object_outcome.GetError().GetExceptionName().c_str());
      string exceptionMessage(put_object_outcome.GetError().GetMessage().c_str());
      Logger::error("putObject error:\n" + exceptionName + " " + exceptionMessage);
      status = false;
    }
  }
  if (useCount == 1) {
    Aws::ShutdownAPI(options);
  }
  --useCount;
  return status;
}

bool AwsS3Manager::deleteObject (string remoteFileName) {
  bool status = true;
  Aws::SDKOptions options;
  if (useCount == 0) {
    Aws::InitAPI(options);
  }
  ++useCount;
  {
    const Aws::String bucket_name = bucketName.c_str();
    const Aws::String key_name = (subDirectory + "/" + remoteFileName).c_str();

    Aws::Client::ClientConfiguration clientConfig;
    clientConfig.region = region.c_str();
    Aws::S3::S3Client s3_client(clientConfig);

    Aws::S3::Model::DeleteObjectRequest object_request;
    object_request.WithBucket(bucket_name).WithKey(key_name);

    auto delete_object_outcome = s3_client.DeleteObject(object_request);

    if (delete_object_outcome.IsSuccess()) {
      Logger::log(remoteFileName + " successfully deleted.");
    } else {
      string exceptionName(delete_object_outcome.GetError().GetExceptionName().c_str());
      string exceptionMessage(delete_object_outcome.GetError().GetMessage().c_str());
      Logger::error("putObject error:\n" + exceptionName + " " + exceptionMessage);
      status = false;
    }
  }
  if (useCount == 1) {
    Aws::ShutdownAPI(options);
  }
  --useCount;
  return status;
}
