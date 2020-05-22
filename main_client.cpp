//=============================================================================
#include "client.hpp"
//=============================================================================
#include "opencv2/opencv.hpp"
#include "camera.h"
#include "window.h"
#include "audio.hpp"



int main(int argc, char *argv[])
{
  Camera camera;
  Window window;
  Microphone mic;
  AudioPlayer player;

  if (camera.open() == 0) {
    std::cout << "Camera is opened" << std::endl;
  } else {
    std::cout << "Camera failed to open" << std::endl;
    return -1;
  }

  mic.OpenDevice();
  player.OpenDevice();
  player.CreateContext();

  Client c(25591);
  c.Connect("109.71.10.212");

  //c.Read(buffer);
  //std::cout << buffer;

  mic.CaptureStart();

  unsigned char buffer_recv[100000];
  unsigned char buffer_send[100000];

  int32_t samplesCaptured;
  int32_t bytes;

  int pid = fork();


  while (true) {
    if (pid != 0) {
      int32_t *metaData = c.Read(buffer_recv);

      int32_t type = metaData[0];  
      int32_t size = metaData[1];

      if (type == Client::VID) {
        std::vector <unsigned char> buff_2(buffer_recv, buffer_recv + size);
        std::cout << "Size of recieved buffer: " << buff_2.size() << std::endl;

        int key = window.show_jpeg(buff_2, 10);

        if(key == 27) { // stop capturing by pressing ESC
          char pid_str[50];
          sprintf(pid_str, "%d", pid);
          execlp("kill", "kill", pid_str, (char *)NULL);
          break;
        }
      } else if (type == Client::AUD) {
        player.Play(buffer_recv, size / 2);
      }

    } else {
      // Send frame
      std::vector <unsigned char> buff = camera.get_jpeg_frame(20);
      c.Send(buff.data(), Client::VID, buff.size());
      std::cout << "Size of sent video-buffer: " << buff.size() << std::endl;
      // Send sound
      mic.GetAvailableSamples(buffer_send, samplesCaptured, bytes);
      c.Send(buffer_send, Client::AUD, bytes);
      usleep(40000);
    }
  }

  mic.CaptureStop();

  mic.CloseDevice();
  player.CloseDevice();

  window.close();
}//93.175.1.18
//=============================================================================
