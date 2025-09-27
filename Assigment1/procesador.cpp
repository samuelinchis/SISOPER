/*Grupo :Daniela Tellez, Samuel Rueda, Juan David Pomar, David Bayona */


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

class Procesador {
public:
    int memoria[100];    
    int ACC;              
    int PC;               
    int MAR;              
    string MDR;           
    string ICR;           
    string UC;            

    Procesador() {
        for(int i=0; i<100; i++) memoria[i] = 0;
        ACC = PC = MAR = 0;
        MDR = ICR = UC = "";
    }

    int ALU(string op, int op1, int op2 = 0) {
        if(op == "ADD") return op1 + op2;
        else if(op == "INC") return op1 + 1;
        else if(op == "DEC") return op1 - 1;
        else {
            cout << "esa operación q mano? " << op << endl;
            exit(1);
        }
    }
};

class Ejecutor {
public:
    Procesador CPU;
    ofstream salida;

    Ejecutor(string archivoSalida) {
        salida.open(archivoSalida);
    }

    ~Ejecutor() {
        salida.close();
    }


    void funcionSET(string d1, string x) {
        CPU.UC = "EXECUTE";
        int dir = stoi(d1.substr(1));   
        int val = stoi(x);
        CPU.memoria[dir] = val;
    }

    void funcionLDR(string d1) {
        CPU.UC = "EXECUTE";
        int dir = stoi(d1.substr(1));
        CPU.MAR = dir;
        CPU.MDR = to_string(CPU.memoria[dir]);
        CPU.ACC = CPU.memoria[dir];
    }

    void funcionADD(const vector<string>& ops) {
        CPU.UC = "EXECUTE";
        if (ops.size() == 1) { // ADD D1
            int dir = stoi(ops[0].substr(1));
            int valor = CPU.memoria[dir];
            CPU.ACC = CPU.ALU("ADD", CPU.ACC, valor);
            CPU.UC = "ADD D1";
        } else if (ops.size() == 2) { // ADD D1 D3
            int dir1 = stoi(ops[0].substr(1));
            int dir2 = stoi(ops[1].substr(1));
            CPU.ACC = CPU.memoria[dir1];
            CPU.ACC = CPU.ALU("ADD", CPU.ACC, CPU.memoria[dir2]);
            CPU.UC = "ADD D1 D3";
        } else if (ops.size() == 3) { // ADD D1 D3 D4
            int dir1 = stoi(ops[0].substr(1));
            int dir2 = stoi(ops[1].substr(1));
            int dir_dest = stoi(ops[2].substr(1));
            int resultado = CPU.ALU("ADD", CPU.memoria[dir1], CPU.memoria[dir2]);
            CPU.memoria[dir_dest] = resultado;
            CPU.UC = "ADD D1 D3 D4";
        } else {
            cout << "Error: Cantidad de operandos invalida para ADD" << endl;
            exit(1);
        }
    }

    void funcionINC(string d1) {
        CPU.UC = "EXECUTE";
        int dir = stoi(d1.substr(1));
        int valor = CPU.memoria[dir];
        CPU.memoria[dir] = CPU.ALU("INC", valor);  
    }

    void funcionDEC(string d1) {
        CPU.UC = "EXECUTE";
        int dir = stoi(d1.substr(1));
        int valor = CPU.memoria[dir];
        CPU.memoria[dir] = CPU.ALU("DEC", valor);  
    }

    void funcionSTR(string d1) {
        CPU.UC = "EXECUTE";
        int dir = stoi(d1.substr(1));
        CPU.memoria[dir] = CPU.ACC;
    }

    void funcionSHW(string arg) {
        CPU.UC = "EXECUTE";
        if(arg == "ACC") salida << "ACC: " << CPU.ACC << endl;
        else if(arg == "ICR") salida << "ICR: " << CPU.ICR << endl;
        else if(arg == "MAR") salida << "MAR: " << CPU.MAR << endl;
        else if(arg == "MDR") salida << "MDR: " << CPU.MDR << endl;
        else if(arg == "UC") salida << "UC: " << CPU.UC << endl;
        else {
            int dir = stoi(arg.substr(1));
            salida <<  CPU.memoria[dir] << endl;
        }
    }

    void ejecutar(vector<vector<string>> instrucciones) {
        CPU.PC = 0;

        while (CPU.PC < instrucciones.size()) {
            CPU.UC = "FETCH";
            CPU.MAR = CPU.PC;

            string instruccionTexto;
            for (int i = 0; i < instrucciones[CPU.PC].size(); i++) {
                instruccionTexto += instrucciones[CPU.PC][i];
                if (i < instrucciones[CPU.PC].size() - 1) instruccionTexto += " ";
            }
            CPU.MDR = instruccionTexto;
            CPU.ICR = CPU.MDR;

            CPU.UC = "DECODE";
            vector<string> inst = instrucciones[CPU.PC];
            string op = inst[0];

            if(op == "SET") funcionSET(inst[1], inst[2]);
            else if(op == "LDR") funcionLDR(inst[1]);
            else if(op == "ADD") {
                vector<string> ops;
                for (size_t i = 1; i < inst.size() && inst[i] != "NULL"; ++i) {
                    ops.push_back(inst[i]);
                }
                funcionADD(ops);
            }
            else if(op == "INC") funcionINC(inst[1]);
            else if(op == "DEC") funcionDEC(inst[1]);
            else if(op == "STR") funcionSTR(inst[1]);
            else if(op == "SHW") funcionSHW(inst[1]);
            else if(op == "END") break;
            else if (op == "PAUSE") {
                cout << "Ejecución pausada. Presiona Enter para continuar...";
                cin.ignore();
                cin.get();
            }

            CPU.PC++;
        }
    }
};


int main() {
    ifstream archivo("programa3.txt");
    string linea;
    vector<vector<string>> instrucciones;

    while(getline(archivo, linea)) {
        stringstream ss(linea);
        string palabra;
        vector<string> partes;
        while(ss >> palabra) partes.push_back(palabra);
        instrucciones.push_back(partes);
    }

    Ejecutor simulador("salida.txt");
    simulador.ejecutar(instrucciones);

    return 0;
}
