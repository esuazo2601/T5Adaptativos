//60 segundos para la entrega
#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>

using namespace std;

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
pair<string,int> greedy(int largo,vector<string>entrada){
    //Se crea una solucion actual
    string sol_ac = genera_random_sol(largo);

    // Se calcula el costo de la actual y se asume como mejor solucion
    int costo_ac = hamming_cuadrado_a(sol_ac, entrada);
    int mejor_costo = costo_ac;
     // Registra el tiempo actual una vez
    string sol_final = sol_ac;
    
    string base = "AGTC";
    int index = rand()%largo;
    sol_final[index] = base[rand()%4];
    int costo_final = hamming_cuadrado_a(sol_final,entrada);
    if(costo_final > costo_ac){
        mejor_costo = costo_final;
    }
    return make_pair(sol_final,mejor_costo);
}

/* string GRASP(int tiempo_max){
    string sol_in = genera_random_sol();
    while (1) {
    auto tiempo_transcurrido = std::chrono::duration_cast<std::chrono::milliseconds>(tiempo_actual - tiempo_inicio).count();
    if (tiempo_transcurrido >= tiempo_max * 1000) {
        // El tiempo ha transcurrido o se ha procesado toda la entrada, detén el algoritmo
        break;
    }

    tiempo_actual = std::chrono::steady_clock::now(); // Actualiza el tiempo actual en cada iteración
} */


int main(int argc, char*argv[]){
    srand(time(NULL));
    if (argc != 3 || string(argv[1]) != "-i") {
        cout << "Uso incorrecto. Debe especificar una instancia de problema con '-i <nombre-archivo> -t tiempo'." << endl;
        return 1; // Código de error
    }

    string instancia = argv[2];
    //string tiempo_max_str = argv[4]; // Cambia el nombre a tiempo_max_str
    //int tiempo_max = stoi(tiempo_max_str); // Convierte tiempo_max_str a entero

    vector<string> entrada = lee_instancia(instancia);
    int len = entrada[0].length();
    for (int i=0;i<entrada.size();i++){
        cout<<entrada[i]<<endl;
    }
    cout<<len<<endl;
    pair<string,int> res = greedy(entrada[0].length(),entrada); 
  
    cout<<"Solución encontrada es: " << res.first << " " << "Costo: "<<res.second<<endl;
    return 0;
}

