//samuel rueda 

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>

using namespace std;


// Función para dibujar una barra horizontal que representa el disco
void dibujar_disco(int posicion_actual, int posicion_destino, int tamanio_disco = 5000) {
    int escala = tamanio_disco / 50; // Escalar a 50 caracteres
    int pos_actual_escalada = posicion_actual / escala;
    int pos_destino_escalada = posicion_destino / escala;
    
    cout << "0";
    for(int i = 0; i <= 50; i++) {
        if(i == pos_actual_escalada && i == pos_destino_escalada) {
            cout << "="; // Mismo punto
        } else if(i == pos_actual_escalada) {
            cout << "$"; // Posición actual
        } else if(i == pos_destino_escalada) {
            cout << "&"; // Destino
        } else if(i > min(pos_actual_escalada, pos_destino_escalada) && 
                  i < max(pos_actual_escalada, pos_destino_escalada)) {
            cout << "-"; // Camino
        } else {
            cout << "*";
        }
    }
    cout << "4999" << endl;
    cout << "  " << posicion_actual << " -> " << posicion_destino 
         << " (movimiento: " << abs(posicion_destino - posicion_actual) << ")" << endl;
}

// Función para mostrar el recorrido completo
void mostrar_recorrido(vector<int>& secuencia, string algoritmo) {
    cout << "\n=== Recorrido del cabezal (" << algoritmo << ") ===" << endl;
    cout << "Secuencia: ";
    for(int i = 0; i < secuencia.size(); i++) {
        cout << secuencia[i];
        if(i < secuencia.size() - 1) cout << " -> ";
    }
    cout << endl << endl;
}

// Función para mostrar gráfico detallado movimiento por movimiento
void grafico_detallado(vector<int>& secuencia) {
    cout << "\n=== Movimientos detallados ===" << endl;
    cout << "Leyenda:" << endl;
    cout << "=: Mismo punto" << endl;
    cout << "$: Posición actual del cabezal" << endl;
    cout << "&: Posición destino" << endl;
    cout << "-: Camino recorrido por el cabezal" << endl << endl;
    for(int i = 0; i < secuencia.size() - 1; i++) {
        cout << "Paso " << (i + 1) << ": ";
        dibujar_disco(secuencia[i], secuencia[i + 1]);
        cout << endl;
    }
}

vector<int> generar_peticiones(){
    vector<int> peticiones(1000);
    srand(time(NULL));
    for(int i = 0; i < 1000; i++){
        peticiones[i] = rand() % 5000; //generar un numero aleatorio entre 0 y 4999
    }
    
    return peticiones;
}

void FCFS(int head){
    //generar peticiones aleatorias y mostrarlas
    vector<int> peticiones = generar_peticiones();
    cout << "Primeras 20 peticiones generadas (FCFS): ";
    for(int i = 0; i < 20; i++){
        cout << peticiones[i] << " ";
    }
    cout << endl; 
    cout << "posicion actual del cabezal: " << head << endl;
    int movimientos_totales = 0;
    int posicion_actual = head;

    vector<int> secuencia;
    secuencia.push_back(posicion_actual);

    //calcular movimientos totales
    for(int i = 0; i < peticiones.size(); i++){
        movimientos_totales += abs(peticiones[i] - posicion_actual);
        posicion_actual = peticiones[i];
        if (i < 10) { //solo guardar las primeras 20 posiciones para el recorrido
            secuencia.push_back(posicion_actual);
        }
    }

    mostrar_recorrido(secuencia, "FCFS");
    grafico_detallado(secuencia);
    cout << "Total de movimientos del cabezal (FCFS): " << movimientos_totales << endl;
}

void SCAN(int head){
    //generar peticiones aleatorias y mostrarlas
    vector<int> peticiones = generar_peticiones();
    cout << "Primeras 20 peticiones generadas (SCAN): ";
    for(int i = 0; i < 20; i++){
        cout << peticiones[i] << " ";
    }
    cout << endl; 
    cout << "posicion actual del cabezal: " << head << endl;
    
    priority_queue<int, vector<int>, greater<int>> primer_ciclo; //cola de prioridad para el primer ciclo (ascendente)
    priority_queue<int> segundo_ciclo; //cola de prioridad para el segundo ciclo (descendente)

    for (int i = 0; i < peticiones.size(); i++){
        if(peticiones[i] >= head){
            primer_ciclo.push(peticiones[i]);
        } else {
            segundo_ciclo.push(peticiones[i]);
        }
    }

    int movimientos_totales = 0;
    int posicion_actual = head;
    vector<int> secuencia;
    secuencia.push_back(posicion_actual);
    

    //mover el cabezal hacia arriba primero
    while (!primer_ciclo.empty()){
        int siguiente_peticion = primer_ciclo.top();
        primer_ciclo.pop();
        movimientos_totales += abs(siguiente_peticion - posicion_actual);
        posicion_actual = siguiente_peticion;
        secuencia.push_back(posicion_actual);
    }
    if (!segundo_ciclo.empty()){
        //llegar al final del disco
        movimientos_totales += abs(4999 - posicion_actual);
        posicion_actual = 4999;
        secuencia.push_back(posicion_actual);
    }
    //luego mover hacia abajo
    while (!segundo_ciclo.empty()){
        int siguiente_peticion = segundo_ciclo.top();
        segundo_ciclo.pop();
        movimientos_totales += abs(siguiente_peticion - posicion_actual);
        posicion_actual = siguiente_peticion;
        secuencia.push_back(posicion_actual);
    }

    mostrar_recorrido(secuencia, "SCAN");
    grafico_detallado(secuencia);

    cout << "Total de movimientos del cabezal (SCAN): " << movimientos_totales << endl;
}

void C_SCAN(int head){
    //generar peticiones aleatorias y mostrarlas
    vector<int> peticiones = generar_peticiones();
    cout << "Peticiones generadas (C-SCAN): ";
    for(int i = 0; i < peticiones.size(); i++){
        cout << peticiones[i] << " ";
    }
    cout << endl; 
    cout << "posicion actual del cabezal: " << head << endl;
    
    priority_queue<int, vector<int>, greater<int>> primer_ciclo; //cola de prioridad para el primer ciclo (ascendente)
    priority_queue<int, vector<int>, greater<int>> segundo_ciclo; //cola de prioridad para el segundo ciclo (ascendente)

    for (int i = 0; i < peticiones.size(); i++){
        if(peticiones[i] >= head){
            primer_ciclo.push(peticiones[i]);
        } else {
            segundo_ciclo.push(peticiones[i]);
        }
    }

    int movimientos_totales = 0;
    int posicion_actual = head;
    //vector para el recorrido
    vector<int> secuencia;
    secuencia.push_back(head);

    //mover el cabezal hacia arriba primero
    while (!primer_ciclo.empty()){
        int siguiente_peticion = primer_ciclo.top();
        primer_ciclo.pop();
        movimientos_totales += abs(siguiente_peticion - posicion_actual);
        posicion_actual = siguiente_peticion;
        secuencia.push_back(posicion_actual);
    }
    if (!segundo_ciclo.empty()){
        //llegar al final del disco
        movimientos_totales += abs(4999 - posicion_actual);
        secuencia.push_back(4999);
        //saltar al inicio del disco
        movimientos_totales += abs(4999 - 0); //saltar al inicio del disco
        posicion_actual = 0;
        secuencia.push_back(posicion_actual);
    }
    //luego mover hacia arriba nuevamente
    while (!segundo_ciclo.empty()){
        int siguiente_peticion = segundo_ciclo.top();
        segundo_ciclo.pop();
        movimientos_totales += abs(siguiente_peticion - posicion_actual);
        posicion_actual = siguiente_peticion;
        secuencia.push_back(posicion_actual);
    }

    cout << "Total de movimientos del cabezal (C-SCAN): " << movimientos_totales << endl;
}


int main(){
    //Peticion de datos al usuario
    cout << "Bienvenido a una simulacion de algoritmos de planificacion de discos" << endl;
    cout << "Su disco tiene 5000 cilindos numerados del 0 al 4999" << endl;
    cout << "Ingrese la posicion inicial del cabezal del disco: ";
    int head;
    cin >> head;
    
    //validar una posicion valida del cabezal
    while(head < 0 || head > 4999){
        cout << "Posicion invalida. Debe estar entre 0 y 4999." << endl;
        cout << "Ingrese la posicion inicial del cabezal del disco: ";
        cin >> head;
    }

    //peticion del algoritmo a utilizar
    int algoritmo;
    cout << "Seleccione el algoritmo de planificacion de discos a utilizar:" << endl;
    cout << "1. FCFS (First-Come, First-Served)" << endl;
    cout << "2. SCAN" << endl;
    cout << "3. C-SCAN" << endl;
    cout << "Ingrese el numero del algoritmo: ";
    cin >> algoritmo;

    int movimientos_totales;
    switch (algoritmo)
    {
    case 1:
        FCFS(head);
        break;
    
    case 2:
        SCAN(head);
        break;
    case 3:
        C_SCAN(head);
        break;
    
    default:
        cout << "Algoritmo no reconocido." << endl;
        break;
    }

    return 0;
}
