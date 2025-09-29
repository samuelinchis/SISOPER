#include <bits/stdc++.h>
using namespace std;

struct proceso
{
    string etiqueta;
    int arrival_time;
    int burst_time;
    int cola;
    int priority;
    int ComTim;
    int TuArTi;
    int WaiTim;
    int ResTim;
    int tiempoRestante;

    proceso() {}
    proceso(string et, int BT, int AT, int q, int p)
        : etiqueta(et), burst_time(BT), arrival_time(AT), cola(q), priority(p),
          ComTim(0), TuArTi(0), WaiTim(0), ResTim(-1), tiempoRestante(BT) {}
};

void printResultados(const vector<proceso> &resp, const string &titulo)
{
    cout << "\n=== " << titulo << " ===\n";
    cout << left << setw(8) << "Proc" << setw(8) << "BT" << setw(8) << "AT"
         << setw(10) << "CT" << setw(8) << "TAT" << setw(8) << "WT" << setw(8) << "RT" << "\n";

    double sumTAT = 0, sumWT = 0, sumRT = 0;
    for (auto &p : resp)
    {
        cout << left << setw(8) << p.etiqueta
             << setw(8) << p.burst_time
             << setw(8) << p.arrival_time
             << setw(10) << p.ComTim
             << setw(8) << p.TuArTi
             << setw(8) << p.WaiTim
             << setw(8) << p.ResTim << "\n";
        sumTAT += p.TuArTi;
        sumWT += p.WaiTim;
        sumRT += p.ResTim;
    }
    int n = resp.size();
    cout << "Promedios -> TAT=" << sumTAT / n
         << " | WT=" << sumWT / n
         << " | RT=" << sumRT / n << "\n";
}

// Reiniciar métricas
void reset(vector<proceso> &p, const vector<proceso> &orig)
{
    p = orig;
    for (auto &x : p)
    {
        x.ComTim = x.TuArTi = x.WaiTim = 0;
        x.ResTim = -1;
        x.tiempoRestante = x.burst_time;
    }
}

// -------- FCFS --------
void FCFS(vector<proceso> orig)
{
    vector<proceso> p;
    reset(p, orig);
    sort(p.begin(), p.end(), [](auto &a, auto &b)
         { return a.arrival_time < b.arrival_time; });
    int tiempo = 0;
    vector<proceso> resp;
    for (auto &x : p)
    {
        if (tiempo < x.arrival_time)
            tiempo = x.arrival_time;
        x.ResTim = tiempo - x.arrival_time;
        tiempo += x.burst_time;
        x.ComTim = tiempo;
        x.TuArTi = x.ComTim - x.arrival_time;
        x.WaiTim = x.TuArTi - x.burst_time;
        resp.push_back(x);
    }
    printResultados(resp, "FCFS");
}

// -------- SJF (no-preemptivo) --------
void SJF(vector<proceso> orig)
{
    vector<proceso> p;
    reset(p, orig);
    sort(p.begin(), p.end(), [](auto &a, auto &b)
         { return a.arrival_time < b.arrival_time; });
    auto cmp = [](proceso *a, proceso *b)
    {
        if (a->burst_time==b->burst_time) return a->arrival_time>b->arrival_time;
        return a->burst_time>b->burst_time; };
    priority_queue<proceso *, vector<proceso *>, decltype(cmp)> pq(cmp);

    int tiempo = 0, idx = 0, n = p.size(), comp = 0;
    vector<proceso> resp;
    while (comp < n)
    {
        while (idx < n && p[idx].arrival_time <= tiempo)
            pq.push(&p[idx]), idx++;
        if (pq.empty())
        {
            tiempo++;
            continue;
        }
        proceso *cur = pq.top();
        pq.pop();
        if (cur->ResTim == -1)
            cur->ResTim = tiempo - cur->arrival_time;
        tiempo += cur->burst_time;
        cur->ComTim = tiempo;
        cur->TuArTi = cur->ComTim - cur->arrival_time;
        cur->WaiTim = cur->TuArTi - cur->burst_time;
        resp.push_back(*cur);
        comp++;
    }
    printResultados(resp, "SJF");
}

// -------- STCF (preemptivo) --------
void STCF(vector<proceso> orig)
{
    vector<proceso> p;
    reset(p, orig);
    sort(p.begin(), p.end(), [](auto &a, auto &b)
         { return a.arrival_time < b.arrival_time; });
    auto cmp = [](proceso *a, proceso *b)
    {
        if(a->tiempoRestante==b->tiempoRestante) return a->arrival_time>b->arrival_time;
        return a->tiempoRestante>b->tiempoRestante; };
    priority_queue<proceso *, vector<proceso *>, decltype(cmp)> pq(cmp);
    int tiempo = 0, idx = 0, n = p.size(), comp = 0;
    vector<proceso> resp;
    while (comp < n)
    {
        while (idx < n && p[idx].arrival_time <= tiempo)
            pq.push(&p[idx]), idx++;
        if (pq.empty())
        {
            tiempo++;
            continue;
        }
        proceso *cur = pq.top();
        pq.pop();
        if (cur->ResTim == -1)
            cur->ResTim = tiempo - cur->arrival_time;
        cur->tiempoRestante--;
        tiempo++;
        if (cur->tiempoRestante == 0)
        {
            cur->ComTim = tiempo;
            cur->TuArTi = cur->ComTim - cur->arrival_time;
            cur->WaiTim = cur->TuArTi - cur->burst_time;
            resp.push_back(*cur);
            comp++;
        }
        else
            pq.push(cur);
    }
    printResultados(resp, "STCF");
}

// -------- RR --------
void RR(vector<proceso> orig, int q)
{
    vector<proceso> p;
    reset(p, orig);
    sort(p.begin(), p.end(), [](auto &a, auto &b)
         { return a.arrival_time < b.arrival_time; });
    queue<int> rq;
    vector<bool> first(p.size(), true);
    int tiempo = 0, idx = 0, n = p.size(), comp = 0;
    vector<proceso> resp;
    while (comp < n)
    {
        while (idx < n && p[idx].arrival_time <= tiempo)
            rq.push(idx), idx++;
        if (rq.empty())
        {
            tiempo++;
            continue;
        }
        int id = rq.front();
        rq.pop();
        auto &cur = p[id];
        if (first[id])
        {
            cur.ResTim = tiempo - cur.arrival_time;
            first[id] = false;
        }
        int run = min(q, cur.tiempoRestante);
        tiempo += run;
        cur.tiempoRestante -= run;
        while (idx < n && p[idx].arrival_time <= tiempo)
            rq.push(idx), idx++;
        if (cur.tiempoRestante == 0)
        {
            cur.ComTim = tiempo;
            cur.TuArTi = cur.ComTim - cur.arrival_time;
            cur.WaiTim = cur.TuArTi - cur.burst_time;
            resp.push_back(cur);
            comp++;
        }
        else
            rq.push(id);
    }
    printResultados(resp, "Round Robin (Q=" + to_string(q) + ")");
}

int main()
{
    int n;
    cout << "Número de procesos: ";
    cin >> n;
    vector<proceso> procesos;
    for (int i = 0; i < n; i++)
    {
        string et;
        int bt, at, q, p;
        cout << "Proceso " << i + 1 << " (etiqueta BT AT Q P): ";
        cin >> et >> bt >> at >> q >> p;
        procesos.emplace_back(et, bt, at, q, p);
    }
    int quantum;
    cout << "Quantum para RR: ";
    cin >> quantum;

    FCFS(procesos);
    SJF(procesos);
    STCF(procesos);
    RR(procesos, quantum);
}
