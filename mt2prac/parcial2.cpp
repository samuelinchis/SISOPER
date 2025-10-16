//samuel rueda 
//8975105

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;


class bloque{
    public:
        string tag = "Libre";             //libre o con procesos
        int almacenamiento;

         bloque(int almacenamiento = 0)
        :  almacenamiento(almacenamiento) {}
};

vector <bloque> memoria; //memoria del sistema

void mostrar_memoria(){ //muestra la memoria
    for(int i = 0 ; i < memoria.size(); i ++){
        cout << "[" << memoria[i].tag << ":" << memoria[i].almacenamiento << "]";
        }     
        cout << "\n";
}

void particion_estatica(vector<bloque> &memoria, int tamano_memoria){
    int bloques = 5;
    int tamano_bloque = tamano_memoria/bloques;
    
    for (int i = 0; i < bloques; i++){
        memoria.push_back(bloque(tamano_bloque));
    }

}

void asignar_FirstFit(string tag, int tamano) {
    for (int i = 0; i < memoria.size(); i++) {
        // Si el bloque está libre y cabe el proceso
        if (memoria[i].tag == "Libre" && memoria[i].almacenamiento >= tamano) {
            int espacio_restante = memoria[i].almacenamiento - tamano;

            // Asigna el proceso al bloque actual
            memoria[i].tag = tag;
            memoria[i].almacenamiento = tamano;

            // Si queda espacio libre, crea un nuevo bloque a continuación
            if (espacio_restante > 0) {
                memoria.insert(memoria.begin() + i + 1, bloque(espacio_restante));
            }
            return;
        }
    }
}

void liberar_memoria(string tag) {
    for (int i = 0; i < memoria.size(); i++) {
        if (memoria[i].tag == tag) { // Buscar el bloque que tiene asignado el proceso que quiero liberar y liberarlo
            memoria[i].tag = "Libre";

            // Fusionar bloques libres contiguos
            // Si el bloque anterior existe y también está libre
            if (i > 0 && memoria[i - 1].tag == "Libre") {
                // Fusiona el bloque actual con el anterior:
                // Suma el tamaño del bloque actual al bloque anterior.
                memoria[i - 1].almacenamiento += memoria[i].almacenamiento;

                // Elimina el bloque actual (ya fue absorbido por el anterior)
                memoria.erase(memoria.begin() + i);

                // Retrocede el índice para que el siguiente if
                // analice correctamente la nueva posición actual.
                i--;
            }

            // Si el bloque siguiente existe y también está libre
            if (i < memoria.size() - 1 && memoria[i + 1].tag == "Libre") {
                // Fusiona el bloque actual con el siguiente:
                // Suma el tamaño del siguiente bloque al actual.
                memoria[i].almacenamiento += memoria[i + 1].almacenamiento;

                // Elimina el siguiente bloque (ya fue absorbido por el actual)
                memoria.erase(memoria.begin() + i + 1);
            }
            return;
        }
    }
}

void asignar_BestFit(string tag, int tamano) { 
    int mejor = -1;
    int menor_espacio = 1e9;

    for (int i = 0; i < memoria.size(); i++) { //se busca entre los bloques que estan libres
        if (memoria[i].tag == "Libre" && memoria[i].almacenamiento >= tamano) {
            int sobrante = memoria[i].almacenamiento - tamano;
            if (sobrante < menor_espacio) {
                menor_espacio = sobrante;
                mejor = i;
            }
        }
    }

    if (mejor == -1) { //no hay mas espacios para mas procesos
        return;
    }

    int espacio_restante = memoria[mejor].almacenamiento - tamano;
    memoria[mejor].tag = tag;                                   //se pone el proceso en el mejor espacio
    memoria[mejor].almacenamiento = tamano;                     //

    if (espacio_restante > 0)                                   //se crea un bloque libre con el espacio restante
        memoria.insert(memoria.begin() + mejor + 1, bloque(espacio_restante));
}

void asignar_WorstFit(string tag, int tamano) {
    int peor = -1;
    int mayor_espacio = -1;

    for (int i = 0; i < memoria.size(); i++) {
        if (memoria[i].tag == "Libre" && memoria[i].almacenamiento >= tamano) { //busca entre todos los bloques libres el peor candidato
            if (memoria[i].almacenamiento > mayor_espacio) {
                mayor_espacio = memoria[i].almacenamiento;
                peor = i;
            }
        }
    }

    if (peor == -1) { //no hay espacio para el proceso 
        return;
    }

    int espacio_restante = memoria[peor].almacenamiento - tamano; //el espacio restante en el bloque
    memoria[peor].tag = tag;                                      //se asigna el proceso al peor bloque
    memoria[peor].almacenamiento = tamano;                        //se asigna el tamaño del proceso al bloque 

    //si queda espacio libre, se crea un nuevo bloque libre con el espacio restante
    if (espacio_restante > 0)
        memoria.insert(memoria.begin() + peor + 1, bloque(espacio_restante));
}

void procesar_linea(string linea, string alocacion){

    stringstream ss(linea);
    
    char operacion;
    string tag = "";
    int memoria_solicitada = 0;
    
    //se lee la linea y se parsea
    ss >> operacion;
    
    if(operacion == 'A'){
        ss >> tag;
        ss >> memoria_solicitada;
        if (alocacion == "FF") {
            asignar_FirstFit(tag, memoria_solicitada);
        }
        else if (alocacion == "BF") {
            asignar_BestFit(tag, memoria_solicitada);
        }
        else if (alocacion == "WF") {
            asignar_WorstFit(tag, memoria_solicitada);
        }

    }
    if(operacion == 'L'){
        ss >> tag;
        liberar_memoria(tag);
    }
    if(operacion == 'M'){
        mostrar_memoria();
    }

}

int main(){
    int tam_memoria;
    string entrada;
    int particion;
    string alocacion;

    cout << "Ingrese las unidades de la memoria total" << endl; //memoria total configurable por linea de comandos
    cin >> tam_memoria; 
    if (tam_memoria < 100){
        cout << "Tamaño de memoria demasiado pequeño, reajustando a 100 unidades de memoria." << endl;
        tam_memoria = 100; 
    }

    //algorimo de particion por linea de comandos
    
    cout << "Ingrese 1 para particionar la memoria de forma estatica" << endl;
    cout << "Ingrese 2 para particionar la memoria de forma dinamica"<< endl;
    cin >>  particion;
    if (particion == 1){
        particion_estatica(memoria, tam_memoria);
        mostrar_memoria();
    }
    else if (particion == 2)
    {
        memoria.push_back(bloque(tam_memoria)); // toda la memoria es un solo bloque libre
        mostrar_memoria();
    }
    
    //algoritmo de alocacion por linea de comandos
    cout << "Elija las siglas del algorimto de alocacion a utilizar" << endl;
    cout << "First Fit (FF)\nBest Fit (BF)\nWorst Fit (WF)" << endl;
    cin >> alocacion;

    //archivo de entrada establecido a traves de linea de comandos
    cout << "Ingrese el nombre exacto del archivo de entrada, incluya la extension del archivo" << endl;
    cin >> entrada; 

    //lectura del archivo de entrada
    ifstream archivo(entrada);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo" << entrada << endl;
        return 1;
    }
    string linea;
    while(getline(archivo, linea)){
        procesar_linea(linea, alocacion);
    }
    return 0;
}