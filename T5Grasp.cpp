#include <iostream>
#include <vector>
#include <fstream>

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

string cambia_letras(string& a, string& b){
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
    if(hamming_cuadrado_a(ret,b) < costo_original){
        return ret;
    }
    //de lo contrario el original
    return original;
}

int main(int argc, char*argv[]){
    srand(time(NULL));
    /*  if (argc != 6 || string(argv[1]) != "-i") {
        cout << "Uso incorrecto. Debe especificar una instancia de problema con '-i <nombre-archivo> -t tiempo'." << endl;
        return 1; // Código de error
    } 
    */
    //string instancia = argv[2];

    string instancia = "inst_200_15_4_0.txt";
    string tiempo = argv[5];

    vector<string> motifs = lee_instancia(instancia);
    string init = genera_random_sol();

    return 0;
}

