SRCS:=$(wildcard *.cpp)

FtpServer:$(SRCS)
	g++ $^ -o $@ -pthread -lmysqlclient

clean:
	rm -rf FtpServer
