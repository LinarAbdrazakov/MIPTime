//=============================================================================
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>

#include <iostream>
#include <map>
#include <thread>
#include <vector>
#include <string>

const int MAX_SIZE = 1024 * 1024;
//=============================================================================
class Server
{
  private:
    struct sockaddr_in addr_;
    int32_t            fd_; 
    int32_t            port_;
    int32_t           *shmData_;
    int32_t            count_;
    std::map<int32_t, int32_t> users_;

  public:
    enum {USERS_NUM, CONDITION, NOT_INIT, FIRST_EXIT, SECOND_EXIT, ALL_HERE};

    std::vector<int32_t> keys_;
    Server(int32_t port);
    ~Server();

    int32_t GetUsersNum();
    int32_t GetCondition();
    void    SetCondition(int32_t condition);

    int32_t Start();
    int32_t Send (const char *msg, int32_t num, int32_t length);
    int32_t Read (char *msg, int32_t num);
    void AddUser ();
    void DelUser (int32_t num);
    void UsersWaiting();
    void Wait    ();
    void Update  ();
    void UsersProcessing(int32_t num);
};
//=============================================================================
Server::Server(int32_t port)
{
  int32_t shm_key = ftok("test.txt", 1);
  if (shm_key == -1) std::cout << "ERROR : Could not create sshm_key \n";
        
  int32_t shm_id  = shmget(shm_key, 2 * sizeof(*shmData_), 0666 | IPC_CREAT);
  if (shm_id  == -1) std::cout << "ERROR : Could not create shm_id \n";
  
  shmData_ = (int32_t *)shmat(shm_id, nullptr, 0);
  if (shmData_  == nullptr) std::cout << "ERROR : Could not chmat buffer \n";
  shmData_[USERS_NUM] = 0;
  shmData_[CONDITION] = NOT_INIT;

  port_  = port;
  count_ = 0;

  fd_   = socket(AF_INET, SOCK_STREAM, 0);  
  if (fd_ < 0) std::cout << "ERROR : Could not create socket \n";

  memset(&addr_, '0', sizeof(addr_)); 
  addr_.sin_family      = AF_INET;
  addr_.sin_addr.s_addr = htonl(INADDR_ANY);
  addr_.sin_port        = htons(port_); 

  std::cout << "Server created!\n";
}
//=============================================================================
Server::~Server()
{
  shmdt(shmData_);
  for (auto u : users_) close(u.first);
  std::cout << "Server destroyed!\n";
}
//=============================================================================
int32_t Server::GetUsersNum()
{
  return shmData_[USERS_NUM];
}
//=============================================================================
int32_t Server::GetCondition()
{
  return shmData_[CONDITION];
}
//=============================================================================
void Server::SetCondition(int32_t condition)
{
  shmData_[CONDITION] = condition;
}
//=============================================================================
int32_t Server::Start()
{
  bind(fd_, (struct sockaddr*)&addr_, sizeof(addr_));
  listen(fd_, 10); // max connections
}
//=============================================================================
int32_t Server::Send(const char *msg, int32_t num, int32_t length = -1)
{
  if (length == -1) length = strlen(msg);
  send(users_[keys_[num]], msg, length, 0 ); 
}
//=============================================================================
int32_t Server::Read(char *msg, int32_t num)
{ 
  int32_t valread = read(users_[keys_[num]], msg, MAX_SIZE); 
  return valread;
}
//=============================================================================
void Server::AddUser()
{
  int32_t user = accept(fd_, (struct sockaddr*)nullptr, nullptr);
  users_.insert(std::pair<int32_t, int32_t>(count_, user));
  keys_.push_back(count_++);
  ++shmData_[USERS_NUM];
}
//=============================================================================
void Server::DelUser(int32_t num)
{
  close(users_[keys_[num]]);
  users_.erase(keys_[num]);
  keys_.erase(keys_.begin() + num);
  --shmData_[USERS_NUM];
  if (shmData_[USERS_NUM] != 0)
  {
    Send("Other user leaved\n", 0);
  }
  std::cout << "User leaved\n"; 
}
//=============================================================================
void Server::UsersWaiting()
{
  Update();
  
  while (shmData_[USERS_NUM] < 2)
  {
    if (shmData_[USERS_NUM] == 0) 
    {
      AddUser();
      std::cout << "User joined\n"; 
      Send("Hello, you are the first one, wait for the second user\n", keys_[0], 100);    
    }

    AddUser();
    std::cout << "User joined\n"; 
    Send("Hello, you are the second one\n", keys_[1]); 
    Update();
  }

  shmData_[CONDITION] = ALL_HERE;
}
//=============================================================================
void Server::Update()
{
  switch (shmData_[CONDITION])
  {
    case FIRST_EXIT:
      DelUser(0);
      break;
    case SECOND_EXIT:
      DelUser(1);
      break;
  }
}
//=============================================================================
void Server::Wait()
{
  while (GetUsersNum() == 1)
  {
    char buffer[1];
    memset(buffer, 0, 1);
    if (Read(buffer, 0) <= 0)
    { 
      SetCondition(FIRST_EXIT);
      return;
    }
  }
}
//=============================================================================
void Server::UsersProcessing(int32_t num)
{
  int32_t num1 = 0;
  int32_t num2 = 1;
  int32_t exit = FIRST_EXIT;
  if (num == 1)
  {
    exit = SECOND_EXIT;
    std::swap(num1, num2);
  }

  while (GetCondition() == ALL_HERE)
  {
    char buffer[MAX_SIZE];
    memset(buffer, 0, MAX_SIZE);

    int32_t length = Read(buffer, num1);
    if (length <= 0)
    {
      SetCondition(exit);
      return;    
    }
    Send(buffer, num2, length);
  }
}
//=============================================================================
class Client
{
  private:
    struct sockaddr_in addr_;
    int32_t            fd_; 
    int32_t            port_;

  public:
    Client(int32_t port);
    ~Client();
    int32_t Connect(const char *address);
    int32_t Send   (const char *msg, int32_t length);
    int32_t Read   (char *msg);
};
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
