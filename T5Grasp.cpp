#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>

using namespace std;

int hamming_cuadrado_a(string a, string b){
    int len = a.length();
    int costo = 0;
    
    for (int i=0 ; i<len ; i++){
        if(a[i] != b[i]){
            costo++;
        }
    }

    return costo * costo;
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

string genera_random_sol(){
    string alelos = "AGCT";
    string sol;
    for(int i=0;i<15;i++){
        sol.push_back(alelos [rand()%4] );
    }
    return sol;
}

pair<string,int> cambia_letras(string& a, string& b){

    //Se cambia una cantidad aleatoria de letras
    int cant_letras = 1 + rand()%16;
    string original = a;
    string ret = a;
    
    int costo_original = hamming_cuadrado_a(a,b);
    
    //en un indice random
    for(int i=0;i<cant_letras;i++){
        int random_index = rand()%15;
        ret[random_index] = b[random_index];
    }
    //Si el costo del nuevo string es menor se retorna ese
    int costo_string_n = hamming_cuadrado_a(ret,b);
    if(costo_string_n < costo_original){

        return make_pair(ret,costo_string_n);
    }
    //de lo contrario el original
    return make_pair(original,costo_original);
}

int main(int argc, char*argv[]){
    srand(time(NULL));
    if (argc != 5 || string(argv[1]) != "-i") {
        cout << "Uso incorrecto. Debe especificar una instancia de problema con '-i <nombre-archivo> -t tiempo'." << endl;
        return 1; // Código de error
    }

    string instancia = argv[2];
    string tiempo_max_str = argv[4]; // Cambia el nombre a tiempo_max_str
    int tiempo_max = stoi(tiempo_max_str); // Convierte tiempo_max_str a entero

    vector<string> motifs = lee_instancia(instancia);
    string init = genera_random_sol();
    auto tiempo_inicio = chrono::steady_clock::now();
    int len = motifs.size();
    pair<string,int> sol_actual = make_pair(init,hamming_cuadrado_a(init, motifs[0]));

    int i = 1;
    auto tiempo_actual = std::chrono::steady_clock::now(); // Registra el tiempo actual una vez
    while (1) {
        auto tiempo_transcurrido = std::chrono::duration_cast<std::chrono::milliseconds>(tiempo_actual - tiempo_inicio).count();
        if (tiempo_transcurrido >= tiempo_max * 1000 || i == len) {
            // El tiempo ha transcurrido o se ha procesado toda la entrada, detén el algoritmo
            break;
        }
        sol_actual = cambia_letras(init, motifs[i]);
        i++;
        if(i==len) i=0;
        tiempo_actual = std::chrono::steady_clock::now(); // Actualiza el tiempo actual en cada iteración
    }
    
    cout<<"Solución encontrada en "<<tiempo_max<<" segundos"<<endl;
    cout<<(sol_actual.first)<<" Costo: "<<(sol_actual.second)<<endl;    
    return 0;
}

