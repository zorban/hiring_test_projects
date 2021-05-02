%.o : %.cpp
	g++ -Wall -c -o $@ $< --std=c++11

objects = main.o Utils.o Server.o Logger.o RequestProcessor.o RequestParser.o AwsS3Manager.o SoxInfoParser.o

main: $(objects)
	g++ -Wall -o main $(objects) -lpthread -laws-cpp-sdk-core -laws-cpp-sdk-s3 --std=c++11

Utils.o: include/Utils.h

main.o: include/Server.h include/Logger.h

Server.o: include/Server.h include/Logger.h include/RequestProcessor.h

Logger.o: include/Logger.h

RequestProcessor.o: include/RequestProcessor.h include/Utils.h include/SoxInfoParser.h

RequestParser.o: include/RequestParser.h

AwsS3Manager.o: include/AwsS3Manager.h include/Logger.h

SoxInfoParser.o: include/SoxInfoParser.h

.PHONY: clean
clean:
	-rm main $(objects)

.PHONY: run
run:
	AWS_ACCESS_KEY_ID=AKIAJAE5BGTFOYZDVDUA AWS_SECRET_ACCESS_KEY=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 ./main &
