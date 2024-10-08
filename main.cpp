#include <iostream>
#include <thread>
#include <random>
#include <chrono>
#include <array>
#include <vector>
#include <mutex>
using namespace std;

random_device dev;
mt19937 rng(dev());
uniform_int_distribution<mt19937::result_type> metros_avance(1,10);
uniform_int_distribution<mt19937::result_type> ms_espera(100,500);

const int cantidad_autos = 5;
const int distancia_carrera = 100;

int lugar = 1;
vector<string> lugares;
mutex mtx;

void avanzar(string nombre_auto){
    int distancia = 0;
    
    while(distancia < distancia_carrera)
    {   
        //descansar (velocidad)
        int ms_tiempo = ms_espera(rng);
        this_thread::sleep_for(chrono::milliseconds(ms_tiempo));
        //avanzar
        int distancia_avanzada =  metros_avance(rng);
        distancia += distancia_avanzada;
        if(distancia > distancia_carrera){
            distancia = distancia_carrera;
        }
        {   
            // asegurase que no se impriman varios en la misma linea
            lock_guard<mutex> lock(mtx);
            cout << nombre_auto << ": avanza " << distancia_avanzada  << " mts.   Distancia: " << distancia << endl;
        }
    }
    {
        // proteger las variables de ser accedidas por varias threads
        lock_guard<mutex> lock(mtx);
        cout << nombre_auto << ": Termina en " <<  lugar << "° Lugar" << endl;
        lugares.push_back(nombre_auto);
        lugar +=1;
    }
    
}

int main(int argc, char **argv){
    array<thread, cantidad_autos> hilos;
    cout << "Carrera de " << distancia_carrera << " mts. Con " << cantidad_autos << " autos." << endl;
    for (size_t i = 0; i < cantidad_autos; i++)
    {   
        string nombre_auto = "Auto" + to_string(i+1);
        hilos[i] = thread(avanzar, nombre_auto);
    }

    for (int j = 0; j < cantidad_autos; j++) {
        hilos[j].join();
    }
    cout << endl << "-------------------Termino de la carrera---------------------" << endl;
    for (size_t i = 0; i < lugares.size(); i++) {
        cout << (i+1) << "°: " << lugares[i] << endl; 
    }
    return 0;
}
