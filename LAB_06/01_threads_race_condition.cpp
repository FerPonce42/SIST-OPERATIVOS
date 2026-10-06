#include <iostream>       // std::cout
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
 
void pause_thread(int s, int n) 
{
  std::this_thread::sleep_for (std::chrono::seconds(n));
  std::cout << "Soy el Thread " << s << " seconds ended\n"; 
}
 
int main() 
{
  std::cout << "Spawning and detaching 3 threads...\n";

  for(int i=0 ; i < 100 ; i++){

    std::thread (pause_thread, i ,2).detach(); 

  }



  std::cout << "Done spawning threads.\n";

  std::cout << "(the main thread will now pause for 5 seconds)\n";
  // give the detached threads time to finish (but not guaranteed!):
  pause_thread(666, 100);
  return 0;
}

/*
El thread principal que es el main, tiene que estar vivo. Por eso le ponemos pausa de 5.
cuando llamaos trehead lo ponemos como tal.

pero cuando queremos llamarlo normal ahi si llamo normal una funcion normal p. como toda la vida.

ubuntu como mostrar los threads con el comando ps
ps -e -T



LA SECCION CRITICA FUE:

  std::cout << "Soy el Thread " << s << " seconds ended\n"; 


  porque accede / manipula al terminal.

*/