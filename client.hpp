//=============================================================================
#include "common.hpp"
//=============================================================================
class Client
{
  private:
    struct sockaddr_in addr_;
    int32_t            fd_;
    int32_t            port_;

  public:
  	enum {TXT, VID, AUD};
    Client(int32_t port);
    ~Client();
    
    int32_t Connect(const char *address);
    int32_t Send   (const char *msg, int32_t type, int32_t length);
    int32_t *Read  (char *msg);
};
//=============================================================================
