#include "connection.cpp"

int main()
{
  Server s(25572);
  s.Start();
  
  //while(1)
  {
    s.UsersWaiting();

    std::thread thread(&Server::UsersProcessing, &s, 0);
    s.UsersProcessing(1);
    thread.join();
  }




  /*
  while(1)
  {
    s.UsersWaiting();
    int f = fork();
    if (f == 0) 
    {
      while (s.GetCondition() == s.ALL_HERE)
      {
        char buffer[1024];
        memset(buffer, 0, 1024);
        if (s.Read(buffer, 0) == 0) s.SetCondition(s.SECOND_HERE);
        else s.Send(buffer, 1);
      }
      std::cout << "User leaved\n";
      return 0;
    }
    else while (s.GetCondition() == s.ALL_HERE)
    {
      char buffer[1024];
      memset(buffer, 0, 1024);
      if (s.Read(buffer, 1) == 0) s.SetCondition(s.FIRST_HERE);
      else s.Send(buffer, 0);
    }
    std::cout << "User leaved\n";
  }*/
}
