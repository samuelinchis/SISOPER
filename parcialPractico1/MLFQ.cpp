#include <stdio.h>
#include <iostream>
#include <fstream>
#include <queue>
#include<vector>
#include<sstream>
#include <algorithm>


using namespace std;

class proceso{ //definicion de la clase proceso
    public:
    char etiqueta;
    int arrival_time;
    int burst_time;
    int OGburst_time;
    int cola;
    int priority;
    int ComTim;
    int TuArTi;
    int WaiTim;
    int ResTim;
    bool firstim = true;

    proceso(char et, int BT, int AT, int q, int p)
    : etiqueta(et), burst_time(BT), OGburst_time(BT), arrival_time(AT), cola(q), priority(p),
      ComTim(0), TuArTi(0), WaiTim(0), ResTim(0) {}

};

struct compararAT { //definicion del proceso que tiene el menor tiempo de llegada
    bool operator()(const proceso& a, const proceso& b) const {
        return a.arrival_time > b.arrival_time;
    }

};

void roundRobin(queue<proceso> &cola, int quantum, vector<proceso> &terminados, int &tiempo, int q) {
    int n = cola.size(); // cantidad de procesos en esta ronda

    for (int i = 0; i < n; i++) {
        proceso p = cola.front(); 
        cola.pop();

        p.cola = q; // actualizar la cola del proceso
        if (p.firstim) { // si es la primera vez que se ejecuta
            p.ResTim = tiempo;
            p.firstim = false;
        }

        if (p.burst_time <= quantum) { // si el proceso termina en este quantum
            tiempo += p.burst_time;
            p.burst_time = 0;
            p.ComTim = tiempo;
            p.TuArTi = p.ComTim - p.arrival_time;
            p.WaiTim = p.TuArTi - p.OGburst_time;
            terminados.push_back(p); // añadir a terminados
        } else { //el proceso aun no termina
            tiempo += quantum;
            p.burst_time -= quantum;
            cola.push(p); // sigue en cola
        }
    }

}

vector<proceso> MLFQ(priority_queue<proceso, vector<proceso>, compararAT> procesos){
    int tiempo;
    proceso p = procesos.top();
    tiempo = p.arrival_time;
    vector<proceso> terminados;
    queue<proceso> cola;

    while (!procesos.empty()){ // los proceosos pasan de la cola de prioridad a la cola normal
        proceso current = procesos.top();
        procesos.pop();
        cola.push(current);
    }
    roundRobin(cola, 3, terminados, tiempo,1);
    roundRobin(cola, 5, terminados, tiempo,2);
    roundRobin(cola, 6, terminados, tiempo,3);
    roundRobin(cola, 20, terminados, tiempo,4);
    while (terminados.size() <  procesos.size()){
        roundRobin(cola, 20, terminados, tiempo,4);
    }
    return terminados;
}

void promedios(vector<proceso> procesos){
    double totalWT = 0;
    double totalTAT = 0;
    double totalCT = 0;
    double totalRT = 0;
    for(int i = 0; i < procesos.size(); i++){
        totalWT += procesos[i].WaiTim;
        totalTAT += procesos[i].TuArTi;
        totalCT += procesos[i].ComTim;
        totalRT += procesos[i].ResTim;
    }
    cout << "WT=" << totalWT / procesos.size() << ";" 
        << "CT=" << totalCT / procesos.size() << ";" 
        << "RT=" << totalRT / procesos.size() << ";" 
        << "TAT=" << totalTAT / procesos.size() << ";" << endl;
}

void imprimirProceso(proceso p){
    cout << p.etiqueta << ";" << p.OGburst_time
        << "; " << p.arrival_time << "; " << p.cola
        << "; " << p.priority << "; " << p.WaiTim
        << "; " << p.ComTim << "; " << p.ResTim
        << "; " << p.TuArTi << endl;
}

int main(){
    ifstream archivo("mlq006.txt");
    string linea;
    priority_queue<proceso, vector<proceso>, compararAT> pq1;
    while(getline(archivo, linea)){  // lee toda la línea (ej "A; 0; 10; 5; 0")
        stringstream ss(linea);
        string campo;

        // extraer cada campo usando ';' como delimitador
        getline(ss, campo, ';'); 
        char et = campo[0];  // primera posición (carácter)

        getline(ss, campo, ';');  
        int BT = stoi(campo);
        
        getline(ss, campo, ';');  
        int AT = stoi(campo);

        getline(ss, campo, ';');  
        int q = stoi(campo);

        getline(ss, campo, ';');  
        int p = stoi(campo);

        // crear el objeto y meterlo en la cola
        pq1.push(proceso(et, BT, AT, q, p));
    }
    vector<proceso> fini = MLFQ(pq1);
    sort(fini.begin(), fini.end(), [](const proceso &a, const proceso &b) {
        return a.etiqueta < b.etiqueta; // ordenar por etiqueta
    });
    for(int i = 0; i < fini.size(); i++){
        imprimirProceso(fini[i]);
    }
    promedios(fini);
    return 0;
}