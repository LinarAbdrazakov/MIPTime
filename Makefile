#==============================================================================
CC=g++
C=gcc
#==============================================================================
chat: server client
	 
server: main_server.cpp server.cpp server.hpp common.hpp
	$(CC)	main_server.cpp server.cpp -pthread -o serv_app
	
client: main_client.cpp client.cpp client.hpp camera.cpp window.cpp camera.h window.h common.hpp
	$(CC)	client.cpp main_client.cpp camera.cpp window.cpp audio.cpp -fpermissive -pthread -lalut -lopenal -o client_app `pkg-config --cflags --libs opencv`
	
#==============================================================================
clean:
	rm -rf *.o *.gch
#==============================================================================
