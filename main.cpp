#include <iostream>
#include <thread>
#include <random>
#include <chrono>
#include <vector>
#include <mutex>
using namespace std;

// Se utiliza random_device para obtener una semilla no determinística
random_device dev;
// utilizando la semilla obtenida de random_device
mt19937 rng(dev());
// Se define una distribución uniforme de enteros entre 1 y 10, luego de 100 al 500 que representan las milisimas.
uniform_int_distribution<mt19937::result_type> metros_avance(1, 10);
uniform_int_distribution<mt19937::result_type> ms_espera(100, 500);

int cantidad_autos = 0;
int distancia_carrera = 0;
int lugar = 1;
vector<string> resultados;
mutex mtx;

void avanzar(string nombre_auto){
    int distancia = 0;
    while(distancia < distancia_carrera)
    {   
        //descansar (simula cuanto se demora el auto en recorrer la distancia)
        int ms_tiempo = ms_espera(rng);
        this_thread::sleep_for(chrono::milliseconds(ms_tiempo));
        //avanzar 
        int distancia_avanzada =  metros_avance(rng);
        distancia += distancia_avanzada;
        if(distancia > distancia_carrera){
            distancia = distancia_carrera;
        }
        {   
            // asegurase que no se impriman varios mensajes en la misma linea
            lock_guard<mutex> lock(mtx);
            cout << nombre_auto << ": avanza " << distancia_avanzada  << " mts.   Distancia: " << distancia << endl;
        }
    }
    {
        // protege la variable resultados (donde se guardan los nombres de los autos que han terminado) de ser accedidas por varias hebras
        lock_guard<mutex> lock(mtx);
        cout << nombre_auto << ": Termina en " <<  lugar << "° Lugar" << endl;
        resultados.push_back(nombre_auto);
        lugar +=1; //se usa un contador para mantener el orden de llegada
    }
    
}

int main(int argc, char **argv){
    //validacion de argumentos
    if (argc < 3) {
        cerr << "Uso: " << argv[0] << " <distancia_carrera> <cantidad_autos>" << endl;
        return 1;
    }   
    try {
        // convertir strings en int
        distancia_carrera = stoi(argv[1]); 
        cantidad_autos = stoi(argv[2]);
    } catch (const invalid_argument& e) {
        cerr << "Argumento invalido: No es un numero Entero." << endl;
        return 1;
    }
    if(distancia_carrera<10 && distancia_carrera > 100000){
        cerr << "La distancia de carrera tiene que ser mayor de 10 metros y menor de 100000 metros" << endl;
        return 1;
    }
    if(cantidad_autos < 0 && cantidad_autos < 100 ){
        cerr << "La cantidad de autos tiene que ser mayor a 0 y menor a 100" << endl;
        return 1;
    }
    cout << "Carrera de " << distancia_carrera << " mts. Con " << cantidad_autos << " autos." << endl;
    vector<thread> hilos;
    //se inicializan todas las hebras
    for (size_t i = 0; i < cantidad_autos; i++)
    {   
        string nombre_auto = "Auto" + to_string(i+1);
        hilos.emplace_back(avanzar, nombre_auto);
    }
    //se requiere sincronizar todas hebras cuando terminan
    for (int j = 0; j < cantidad_autos; j++) {
        hilos[j].join();
    }
    cout << endl << "-------------------Termino de la carrera---------------------" << endl << endl;
    //imprimir los resultados en orden de llegada
    for (size_t i = 0; i < resultados.size(); i++) {
        cout << (i+1) << "°: " << resultados[i] << endl; 
    }
    return 0;
}
