//=============================================================================
#include "client.hpp"
//=============================================================================
int SendAll(int32_t fd, const char *buffer, int32_t length, int32_t flags) 
{
	int32_t total = 0;
	int32_t n     = 0;

	while (total < length) 
  {
		n = send(fd, buffer + total, length - total, flags);
		if (n == -1) break;
		total += n;
	}

	return (n == -1 ? -1 : total);
}
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
int32_t Client::Send(const char *msg, int32_t type, int32_t length = -1)
{
  if (length == -1) length = strlen(msg);
  
  char metaData[5];
  metaData[0] = type;
  metaData[1] = (length >> 24) & 0xFF;
  metaData[2] = (length >> 16) & 0xFF;
  metaData[3] = (length >> 8 ) & 0xFF;
  metaData[4] =  length        & 0xFF;

  SendAll(fd_, metaData, 5, 0);
  SendAll(fd_, msg, length, 0 ); 
  std::cout << "Message sent\n";   
}
//=============================================================================
int32_t *Client::Read(char *msg)
{
  int32_t *metaData = new int32_t [2];
  char metaBuffer[5];
  int32_t value = read(fd_, metaBuffer, 5);

  metaData[0] = metaBuffer[0];
  metaData[1] = (int32_t)(metaBuffer + 1);

  value = 0;
  int32_t increment = 0;
  int32_t length    = metaData[1];
  while (value < length)
  {
  	increment = read(fd_, msg, length - value); 
  	value += increment;
  	msg   += increment;
  }
  
  return metaData;
}
//=============================================================================
