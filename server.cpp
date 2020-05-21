#include "connection.cpp"

int main()
{
  Server s(25578);
  s.Start();
  /*
  while(1)
  {
    s.UsersWaiting();

    std::thread thread(&Server::UsersProcessing, &s, 0);
    s.UsersProcessing(1);
    thread.join();
    std::cout << "Cicle\n"
  }//*/

  
  while(1)
  {
    s.UsersWaiting();
    int f = fork();
    if (f == 0) 
    {
      s.UsersProcessing(0);
    }
    else
    {
      s.UsersProcessing(1);
      while(1);
    }
    std::cout << "User leaved\n";
  }//*/
}
