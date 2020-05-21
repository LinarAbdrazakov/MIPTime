#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"

#include "connection.cpp"
#include "camera.h"
#include "window.h"

int main(int argc, char *argv[])
{
  Camera camera;
  Window window;

  if (argc < 2) {
    camera.open();
    std::cout << "Camera opened" << std::endl;
  }

  char buffer[MAX_SIZE];
  memset(buffer, 0, MAX_SIZE);
  Client c(25572);
  c.Connect("127.0.0.2");

  c.Read(buffer);
  std::cout << buffer;
  
  /*while (1)
  {
    memset(buffer, 0, 1024);
    fgets(buffer, sizeof(buffer), stdin);
    c.Send(buffer);
    memset(buffer, 0, 1024);
    c.Read(buffer);
    std::cout << buffer << "\n";
  }*/

  while (true) {
    int key;

    if (argc < 2) {
      std::vector <unsigned char> buff = camera.get_jpeg_frame();
      //buff.push_back('\0');
      c.Send(buff.data(), buff.size());
      std::cout << "Buff size: " << buff.size() << std::endl;
      key = window.show_jpeg(buff, 10);
    } else {
      memset(buffer, 0, MAX_SIZE);
      int size = c.Read(buffer);
      std::vector <unsigned char> buff_2(buffer, buffer + size);
      std::cout << "Got size: " << size << std::endl;
      //buff_2.pop_back();
      std::cout << "Buff_2 size: " << buff_2.size() << std::endl;

      key = window.show_jpeg(buff_2, 10);
    }
    //std::cout << buff.size() << std::endl;
    if(key == 27) break; // stop capturing by pressing ESC 
  }


  window.close();
}//93.175.1.18
