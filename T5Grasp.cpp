//60 segundos para la entrega
#include "greedy-probabilista.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>
#include <limits.h>

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
    if (argc != 9 || string(argv[1]) != "-i" || string(argv[3]) != "-t" || string(argv[5]) != "-it" || string(argv[7]) != "-d") {
        std::cout << "Uso incorrecto. -i <instancia> -t <tiempo> -it <intentos> -d <determinismo>" << endl;
        return 1; // Código de error
    }

    string instancia = argv[2];
    long long tiempo_max_segundos = stoi(argv[4]); // Convierte el tiempo máximo a segundos
    int intentos = stoi(argv[6]);
    int determinismo = stoi(argv[8]);
    vector<string> entrada = lee_instancia(instancia);
    int len = entrada[0].length();

    auto tiempoInicio = chrono::high_resolution_clock::now();

    tuple<int, string, string> mejor_global; // Costo ,Tiempo, Resultado
    mejor_global = make_tuple(INT_MAX,"","");
    while (true) {
        auto tiempoActual = chrono::high_resolution_clock::now();
        auto duracion = chrono::duration_cast<chrono::milliseconds>(tiempoActual - tiempoInicio);

        if (duracion.count() >= tiempo_max_segundos) {
            break; // Sal del bucle while y pasa a la siguiente iteración del bucle for
        }

        pair<string, int> greedy = Greedy_probabilista(entrada, determinismo);
        pair<string, int> bl = busqueda_local(greedy.first, entrada, intentos);

        if (bl.second < get<0>(mejor_global)) {
            auto tiempoTranscurrido = chrono::high_resolution_clock::now() - tiempoInicio;
            float tiempoEnSegundos = chrono::duration<float>(tiempoTranscurrido).count();
            mejor_global = make_tuple(bl.second,to_string(tiempoEnSegundos),bl.first);
        }
    }
    
    std::cout <<"Solución encontrada: "<<get<2>(mejor_global)<<endl; 
    std::cout <<"Costo: "<<get<0>(mejor_global)<<endl; 
    std::cout << "En tiempo: " << get<1>(mejor_global) <<" segundos"<<endl;

    return 0;
}



