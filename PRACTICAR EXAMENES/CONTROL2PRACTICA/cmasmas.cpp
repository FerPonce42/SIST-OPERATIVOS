


/*
Main crea 10 threads en desorden (del 9 al 0) e imprime "10 threads ready".
*/

#include <iostream>       // std::cout
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

void pause_thread(int n)
{
  std::this_thread::sleep_for(std::chrono::seconds(n));
  std::cout << "pause of " << n << " seconds ended\n";
}

int main()
{

    for(int i = 0 ; i < 9 ; i++){

        std::thread(pause_thread, 1).detach();  
    }


  pause_thread(5);   
  return 0;
}