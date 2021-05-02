1. Install required ubuntu packages:
AWS:
sudo apt-get install libcurl4-openssl-dev libssl-dev uuid-dev zlib1g-dev libpulse-dev

sox:
sudo apt install sox libsox-fmt-mp3

2. Build AWS SDK from source
(https://docs.aws.amazon.com/sdk-for-cpp/v1/developer-guide/setup.html)

3. Go to app/src and run:
make

4. Start server with:
bash run_server.sh -p PORT
or 
AWS_ACCESS_KEY_ID=AKIAJAE5BGTFOYZDVDUA AWS_SECRET_ACCESS_KEY=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 ./main -p PORT

Make api calls like:
curl "http://127.0.0.1:8089/wav-info?wavkey=test_102.wav"
curl "http://127.0.0.1:8089/wav-info?wavkey=/test_101_16kHz.wav"
curl "http://localhost:8089/wav-info?wavkey=test_103.wav"
curl "http://localhost:8089/mp3-to-wav?mp3key=test_102.mp3&wavkey=test_102conv.wav"
curl "http://localhost:8089/mp3-to-wav?mp3key=test_102.mp3&wavkey=converted/test_102conv.wav"

curl "http://localhost:8089/list-s3-objects"
