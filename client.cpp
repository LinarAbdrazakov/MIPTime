//=============================================================================
#include "Client.hpp"
//=============================================================================
Client::Client(int32_t port)
{
  port_ = port;
  fd_   = socket(AF_INET, SOCK_STREAM, 0);
  if (fd_ < 0) std::cout << "ERROR : Could not create socket \n";

  memset(&addr_, '0', sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_port   = htons(port_);

  std::cout << "Client created!\n";
}
//=============================================================================
Client::~Client()
{
  std::cout << "Client destroyed!\n";
}
//=============================================================================
int32_t Client::Connect(const char *address)
{ 
  if(inet_pton(AF_INET, address, &addr_.sin_addr) <= 0)
  {
    std::cout << "ERROR : inet_pton error occured\n";
    return -1;
  }
 
  if (connect(fd_, (struct sockaddr *)&addr_, sizeof(addr_)) < 0)
  {
    std::cout << "ERROR : Connect Failed \n";
    return -1;
  }

  return 0;
}
//=============================================================================
int32_t Client::Send(const char *msg, int32_t length = -1)
{
  if (length == -1) length = strlen(msg);
  send(fd_, msg, length, 0); 
  std::cout << "Message sent\n";   
}
//=============================================================================
int32_t Client::Read(char *msg)
{
  int32_t value;
  value = read( fd_ , msg, MAX_SIZE); 
  return value;
}
//=============================================================================