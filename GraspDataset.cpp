//60 segundos para la entrega
#include "greedy-probabilista.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>
#include <string>

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
    srand(time(NULL));
    if (argc != 7 || string(argv[1]) != "-t" || string(argv[3]) !=  "-it" || string (argv[5]) != "-d") {
        cout << "Uso incorrecto. -t <tiempo> -it <intentos>" << endl;
        return 1; // Código de error
    }

    for(int i=0;i<100;i++){
        string instancia = "inst_200_15_4_"+to_string(i)+".txt";
        long long tiempo_max_segundos = stoi(argv[2]); // Convierte el tiempo máximo a segundos
        int intentos = stoi(argv[4]);
        int determinismo = stoi(argv[6]);
        long long tiempo_max_ms = tiempo_max_segundos * 1000; // Convierte a milisegundos
        vector<string> entrada = lee_instancia(instancia);
        int len = entrada[0].length();
        long long start_time = getCurrentTimeMillis();
        long long end_time = start_time + tiempo_max_ms; // Calcula el tiempo de finalización

        pair<int,int> mejor_global; // Tiempo, Costo
        while (getCurrentTimeMillis() < end_time) {
            pair<string,int> greedy = Greedy_probabilista(entrada,determinismo);
            pair<string,int> primera_sol = greedy;

            pair<string,int> bl = busqueda_local(greedy.first,entrada,intentos);
            pair<string,int> mejor_sol = bl;
            cout<<mejor_sol.second<<" "<<primera_sol.second<<endl;
            if (mejor_sol.second < primera_sol.second) {
                mejor_global.second = mejor_sol.second;

                long long tiempo_actual = getCurrentTimeMillis();
                mejor_global.first = (tiempo_actual-start_time)/1000.0;
            }
        }
        cout << "Mejor solución para la instancia " << instancia << "es: " << mejor_global.second << "en tiempo: "<<mejor_global.first<<endl;
    }
    return 0;
}
