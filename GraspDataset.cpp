//60 segundos para la entrega
#include "greedy-probabilista.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>
#include <string>
#include <limits.h>
#include <iomanip> 

using namespace std;

long long getCurrentTimeMillis() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

int hamming_cuadrado_a(string sol_in, vector<string>entrada){
    int len = sol_in.length();
    int len_entrada = entrada.size();
    vector<int>costos;

    for (int i=0;i<len_entrada;i++){
        int costo = 0;
        for (int j=0 ; j<len ; j++){
            if(sol_in[j] != entrada[i][j]){
                costo++;
            }
        }
        costos.push_back(costo);
    }

    int costos_len = costos.size();
    int costo_total=0;

    for (int i=0 ;i<costos_len ;i++){
        costo_total += costos[i]*costos[i];
    }
    return costo_total;
}

vector<string> lee_instancia(string nombre){
    ifstream archivo_txt(nombre);
    vector<string> mat;
    string linea;
    while (getline(archivo_txt, linea)) {
        mat.push_back(linea);
    }
    archivo_txt.close();
    return mat;
}

string genera_random_sol(int largo){
    string alelos = "AGCT";
    string sol;
    for (int i=0;i<largo;i++){
        sol.push_back(alelos [rand()%5]);
    }
    return sol;
}

//Greedy_probabilista(entrada,det)

pair<string, int> busqueda_local(const string& solucion_actual, const vector<string>& entrada, int num_iteraciones) {
    string mejor_solucion = solucion_actual;
    int mejor_costo = hamming_cuadrado_a(solucion_actual, entrada);

    for (int iteracion = 0; iteracion < num_iteraciones; iteracion++) {
        string sol_actual = mejor_solucion;
        int costo_actual = mejor_costo;

        for (int i = 0; i < solucion_actual.length(); i++) {
            string nueva_solucion = sol_actual;
            nueva_solucion[i] = "AGTC"[rand() % 4];
            int nuevo_costo = hamming_cuadrado_a(nueva_solucion, entrada);

            if (nuevo_costo < costo_actual) {
                costo_actual = nuevo_costo;
                sol_actual = nueva_solucion;
            }
        }

        if (costo_actual < mejor_costo) {
            mejor_costo = costo_actual;
            mejor_solucion = sol_actual;
        }
    }

    return make_pair(mejor_solucion, mejor_costo);
}

int main(int argc, char* argv[]) {
    srand(time(nullptr));
    if (argc != 7 || string(argv[1]) != "-t" || string(argv[3]) != "-it" || string(argv[5]) != "-d") {
        std::cout << "Uso incorrecto. -t <tiempo> -it <intentos> -d <determinismo>" << endl;
        return 1; // Código de error
    }

    int m,l;
    std::cout<<"Tamaño de las instancias (m x l)"<<endl;
    cin>>m>>l;

    std::cout << "inst ;" << " m;" << " l;" << " mh;" << " mhtime" << endl;
    for (int i = 0; i < 100; i++) {
        string instancia = "inst_"+ to_string(m)+"_"+to_string(l)+"_4_" + to_string(i) + ".txt";
        long long tiempo_max_segundos = stoi(argv[2]); // Convierte el tiempo máximo a segundos
        int intentos = stoi(argv[4]);
        int determinismo = stoi(argv[6]);
        vector<string> entrada = lee_instancia(instancia);
        int len = entrada[0].length();

        auto tiempoInicio = chrono::high_resolution_clock::now();

        pair<string, int> mejor_global; // Solución global
        mejor_global.second = INT_MAX;
        while (true) {
            auto tiempoActual = chrono::high_resolution_clock::now();
            auto duracion = chrono::duration_cast<chrono::seconds>(tiempoActual - tiempoInicio);

            if (duracion.count() >= tiempo_max_segundos) {
                break; // Sal del bucle while y pasa a la siguiente iteración del bucle for
            }

            pair<string, int> greedy = Greedy_probabilista(entrada, determinismo);
            pair<string, int> bl = busqueda_local(greedy.first, entrada, intentos);

            if (bl.second < mejor_global.second) {
                mejor_global.second = bl.second;
                auto tiempoTranscurrido = chrono::high_resolution_clock::now() - tiempoInicio;
                float tiempoEnSegundos = chrono::duration<float>(tiempoTranscurrido).count();
                
                mejor_global.first = to_string(tiempoEnSegundos);
            }
        }
        
        std::cout << i <<" ;" << m << " ;" << l <<" ;"<< mejor_global.second << " ;" << mejor_global.first;
        std::cout<<endl;
    }   

    return 0;
}

