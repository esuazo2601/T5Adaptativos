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

/* pair<string,int> busqueda_local(string sol_in){

}
 */
pair<string, int> greedy(int largo, vector<string> entrada, long tiempo_max_ms) {
    string sol_ac = genera_random_sol(largo);
    int costo_ac = hamming_cuadrado_a(sol_ac, entrada);
    int mejor_costo = costo_ac;
    string sol_final = sol_ac;
    long long start_time = getCurrentTimeMillis();

    while (getCurrentTimeMillis() - start_time < tiempo_max_ms) {
        int index = rand() % largo;
        string nueva_sol = sol_final;
        nueva_sol[index] = "AGTC"[rand() % 4];
        int nuevo_costo = hamming_cuadrado_a(nueva_sol, entrada);

        if (nuevo_costo < mejor_costo) {
            mejor_costo = nuevo_costo;
            sol_final = nueva_sol;
            cout << "Nueva mejor solución encontrada -> " << " Costo: " << mejor_costo << endl;
        }
    }

    return make_pair(sol_final, mejor_costo);
}


/* string GRASP(int tiempo_max){

}  */


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

    while (getCurrentTimeMillis() < end_time) {
        pair<string, int> res = greedy(len, entrada, tiempo_max_ms);
        cout << "Solución encontrada es: " << res.first << " Costo: " << res.second << endl;
    }
    
    return 0;
}



