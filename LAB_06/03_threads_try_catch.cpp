#include <iostream>       // std::cout
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds


#include <mutex> // BIBLIOTECA


#include <stdexcept>  // TRY CATCH

std::mutex mtx; // CREAR LA VARIABLE GLOBAL
 
void pause_thread(int s, int n) 
{
  std::this_thread::sleep_for (std::chrono::seconds(n));


// AQUI ES LO QUE ARREGLA

  mtx.lock();


  try{

  std::cout << "Soy el Thread " << s << " seconds ended\n";

    

  }catch(){

    std::cout << "[excepcion agarrada]"<<endl;
  }







  mtx.unlock();

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


AHHORA
la bibliotemca mutex soluciona ese problema de interlingnis (que bugeaba los prints)

Pero no arregla el tema del " orden", se imprimen cuando quieren, entonces susaremos masa adelante variables de condicion

*/