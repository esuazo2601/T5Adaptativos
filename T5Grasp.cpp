//60 segundos para la entrega
#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>

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

pair<string, int> greedy(int largo, vector<string> entrada, int num_intentos) {
    string mejor_solucion = genera_random_sol(largo);
    int mejor_costo = hamming_cuadrado_a(mejor_solucion, entrada);

    for (int intento = 0; intento < num_intentos; intento++) {
        string sol_actual = genera_random_sol(largo);
        int costo_actual = hamming_cuadrado_a(sol_actual, entrada);

        while (true) {
            int index = rand() % largo;
            string nueva_sol = sol_actual;
            nueva_sol[index] = "AGTC"[rand() % 4];
            int nuevo_costo = hamming_cuadrado_a(nueva_sol, entrada);

            if (nuevo_costo < costo_actual) {
                costo_actual = nuevo_costo;
                sol_actual = nueva_sol;
            } else {
                break;  // Salir si no hay mejora
            }
        }

        if (costo_actual < mejor_costo) {
            mejor_costo = costo_actual;
            mejor_solucion = sol_actual;
        }
    }

    return make_pair(mejor_solucion, mejor_costo);
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


pair<string, int> grasp(int largo, vector<string> entrada, long tiempo_max_ms) {
    pair<string, int> solucion_greedy = greedy(largo, entrada, 10);
    string sol_greedy = solucion_greedy.first;

    // Aplicar búsqueda local a la solución Greedy
    pair<string, int> solucion_final = busqueda_local(sol_greedy, entrada,10);

    return solucion_final;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    if (argc != 5 || string(argv[1]) != "-i" || string(argv[3]) != "-t") {
        cout << "Uso incorrecto. Debe especificar una instancia de problema con '-i <nombre-archivo> -t <tiempo>'." << endl;
        return 1; // Código de error
    }
    
    string instancia = argv[2];
    long long tiempo_max_segundos = stoi(argv[4]); // Convierte el tiempo máximo a segundos
    long long tiempo_max_ms = tiempo_max_segundos * 1000; // Convierte a milisegundos

    vector<string> entrada = lee_instancia(instancia);
    int len = entrada[0].length();

    long long start_time = getCurrentTimeMillis();
    long long end_time = start_time + tiempo_max_ms; // Calcula el tiempo de finalización

    pair<string, int> mejor_solucion;
    bool primera_solucion = true;

    while (getCurrentTimeMillis() < end_time) {
        pair<string, int> solucion_actual = grasp(len, entrada, tiempo_max_ms);

        if (primera_solucion || solucion_actual.second < mejor_solucion.second) {
            mejor_solucion = solucion_actual;
            primera_solucion = false;

            long long tiempo_actual = getCurrentTimeMillis();
            cout << "Tiempo: " << (tiempo_actual - start_time) / 1000.0 << " segundos, Costo: " << mejor_solucion.second << endl;
        }
    }

    cout << "Mejor solución encontrada es: " << mejor_solucion.first << " Costo: " << mejor_solucion.second << endl;

    return 0;
}




