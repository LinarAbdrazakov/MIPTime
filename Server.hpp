//=============================================================================
#include "Common.hpp"
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
