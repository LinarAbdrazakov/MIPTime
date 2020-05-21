#include <iostream>
#include <vector>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "opencv2/opencv.hpp"

#include "connection.cpp"
#include "camera.h"
#include "window.h"

int main(int argc, char *argv[])
{
  Camera camera;
  Window window;

  camera.open();
  std::cout << "Camera is opened" << std::endl;

  char buffer[MAX_SIZE];
  memset(buffer, 0, MAX_SIZE);
  Client c(25572);
  c.Connect("127.0.0.2");

  c.Read(buffer);
  std::cout << buffer;

  int pid = fork();

  while (true) {
    if (pid != 0) {
      memset(buffer, 0, MAX_SIZE);
      int size = c.Read(buffer);

      std::vector <unsigned char> buff_2(buffer, buffer + size);
      std::cout << "Size of recieved buffer: " << buff_2.size() << std::endl;

      int key = window.show_jpeg(buff_2, 10);

      if(key == 27) { // stop capturing by pressing ESC
        char pid_str[50];
        sprintf(pid_str, "%d", pid);
        execlp("kill", "kill", pid_str, (char *)NULL);
        break;
      } 

    } else {
      std::vector <unsigned char> buff = camera.get_jpeg_frame();
      c.Send(buff.data(), buff.size());
      std::cout << "Size of sent buffer: " << buff.size() << std::endl;
      usleep(40000);
    }
  }


  window.close();
}//93.175.1.18
