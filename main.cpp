#include <iostream>
#include <thread>
using namespace std;

void tarea(){
    cout << "pussy" << endl;
}

int main(int argc, char **argv){
    thread hilo(tarea);
    hilo.join();
    return 0;
}
