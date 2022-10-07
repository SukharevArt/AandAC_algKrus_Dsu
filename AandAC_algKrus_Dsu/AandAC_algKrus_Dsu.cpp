#include<iostream>
#include<algorithm>
#include<vector>
#include<list>


using namespace std;

// сортировка ребер при генерации графа = 0
// сортировка ребер внутри алгоритма = 1
int Flag = 1;

struct edge{
    int x, y;// две соединенные вершины
    int w;// вес ребра
    edge(int _x, int _y, int _w) :x(_x), y(_y), w(_w) {}
    bool operator<(const edge& a) {
        if (w != a.w)      
            return w < a.w;
        if (x != a.x)
            return x < a.x;
        return y < a.y;
    }
};

// Методы получающие представителей множеств
int get_repres_compr(int a,vector<int> &par) {
    return par[a] = (par[a] == a ? a : get_repres_compr(par[a],par));
}
int get_repres(int a, vector<int>& par) {
    return (par[a] == a ? a : get_repres(par[a],par));
}

// Объединение двух непересекающихся множеств 
// Одинаково для обоих случаев
int union_sets(int a, int b,vector<int>& par, vector<int>& rang) {
    if (a == b)
        return 0;
    if (rang[a] == rang[b])
        rang[a]++;
    if (rang[a] < rang[b])
        swap(a, b);
    par[b] = a;
    return 1;
}

// Генерация случайного графа
vector<edge> gengraph(int n = 100, int m = 700, int q = 1, int r = 1e6) {
    vector<edge> newgr;
    std::vector<int> ed(n);

    srand(time(0));
    for (int i = 1; i < n; i++) {
        int y = rand() % i;
        newgr.push_back(edge(i,y,q + rand() % (r - q + 1) ));
        ed[i] = y;
        m--;
    }

    for (int i = 1; i < n; i++) {
        int k = m / (n - i);
        for (int j = 0; j < i && k; j++) {
            if (ed[i] != j) {
                newgr.push_back(edge(i,j,q + rand() % (r - q + 1)));
                k--;
            }
        }
    }
    if(Flag==0)
        sort(newgr.begin(),newgr.end());
    return newgr;
}

// Алгоритм Краскала БЕЗ сжатия путей
vector<edge> alg_Krus_dsu(vector<edge> graph,int n,int m) {
    if(Flag==1)
        sort(graph.begin(), graph.end());
    vector<int>  par;
    vector<int> rang;
    vector<edge> span_tree;
    par.resize(n);
    for (int i = 0; i < n; i++)
        par[i] = i;
    rang.assign(n, 1);
    for(auto tmp:graph) {
        int o1 = tmp.x;
        int o2 = tmp.y;
        o1 = get_repres(o1,par);
        o2 = get_repres(o2,par);
        if (o1 != o2) {
            union_sets(o1, o2, par, rang);
            span_tree.push_back(tmp);
        }
    }
    return span_tree;
}

// Алгоритм Краскала С сжатием путей
vector<edge> alg_Krus_dsu_with_compr(vector<edge> graph,int n,int m) {
    if(Flag==1)
        sort(graph.begin(), graph.end());
    vector<int>  par;
    vector<int> rang;
    vector<edge> span_tree;
    par.resize(n);
    for (int i = 0; i < n; i++)
        par[i] = i;
    rang.assign(n, 1);
    for(auto tmp:graph) {
        int o1 = tmp.x;
        int o2 = tmp.y;
        o1 = get_repres_compr(o1,par);
        o2 = get_repres_compr(o2,par);
        if (o1 != o2) {
            union_sets(o1, o2, par, rang);
            span_tree.push_back(tmp);
        }
    }
    return span_tree;
}

// Тесты анализа скорости работы
// Результаты представлены в .txt файлах
// Графики по результатам представлены в .xlsx файлах
void test1() {
    vector<edge> graph;
    vector<int> tn, tc;
    cout <<" test1. n=[1..1e4+1]/100, m=n*n/10, q=1, r=1e6 " << endl;
    for (int n = 1; n <= 1e4 + 1;n+=100) {
        int m = n * n / 10;
        m = min(m, n * (n - 1) / 2);
        m = max(m, n - 1);
        graph = gengraph(n, m, 1, 1e6);
        cout << "N = " << n << ", M = " << m;
        int t1, t2;
        t1 = clock();
        alg_Krus_dsu(graph,n,m);
        t2 = clock();
        cout << ". Time norm = " << t2 - t1;
        tn.push_back(t2 - t1);
        t1 = clock();
        alg_Krus_dsu_with_compr(graph,n,m);
        t2 = clock();
        cout << ". Time compres = " << t2 - t1 << "." << endl;
        tc.push_back(t2 - t1);
    }
    for (auto i : tn)
        cout << i << "\n";
    cout << "\n";
    for (auto i : tc)
        cout << i << "\n";
}
void test2() {
    vector<edge> graph;
    vector<int> tn, tc;
    cout <<" test2. n=[1..1e4+1]/100, m=n*n, q=1, r=1e6 " << endl;
    for (int n = 1; n <= 1e4 + 1;n+=100) {
        int m = n * n;
        m = min(m, n * (n - 1) / 2);
        m = max(m, n - 1);
        graph = gengraph(n, m, 1, 1e6);
        cout << "N = " << n << ", M = " << m;
        int t1, t2;
        t1 = clock();
        alg_Krus_dsu(graph,n,m);
        t2 = clock();
        cout << ". Time norm = " << t2 - t1;
        tn.push_back(t2 - t1);
        t1 = clock();
        alg_Krus_dsu_with_compr(graph,n,m);
        t2 = clock();
        cout << ". Time compres = " << t2 - t1 << "." <<endl;
        tc.push_back(t2 - t1);
    }
    for (auto i : tn)
        cout << i << "\n";
    cout << "\n";
    for (auto i : tc)
        cout << i << "\n";
}
void test3() {
    vector<edge> graph;
    vector<int> tn, tc;
    cout <<" test3. n=[101..1e4+1]/100, m=n*100, q=1, r=1e6 " << endl;
    for (int n = 101; n <= 1e4 + 1;n+=100) {
        int m = n * 100;
        m = min(m, n * (n - 1) / 2);
        m = max(m, n - 1);
        graph = gengraph(n, m, 1, 1e6);
        cout << "N = " << n << ", M = " << m;
        int t1, t2;
        t1 = clock();
        alg_Krus_dsu(graph,n,m);
        t2 = clock();
        cout << ". Time norm = " << t2 - t1;
        tn.push_back(t2 - t1);
        t1 = clock();
        alg_Krus_dsu_with_compr(graph,n,m);
        t2 = clock();
        cout << ". Time compres = " << t2 - t1 << "." <<endl;
        tc.push_back(t2 - t1);
    }
    for (auto i : tn)
        cout << i << "\n";
    cout << "\n";
    for (auto i : tc)
        cout << i << "\n";
}
void test4() {
    vector<edge> graph;
    vector<int> tn, tc;
    cout <<" test4. n=1e4+1, m=[1e5..1e7]/1e5, q=1, r=1e6 " << endl;
    int n = 1e4+1;
    for (int m = 1e5; m <= 1e7 + 1;m+=1e5) {
        graph = gengraph(n, m, 1, 1e6);
        cout << "N = " << n << ", M = " << m;
        int t1, t2;
        t1 = clock();
        alg_Krus_dsu(graph,n,m);
        t2 = clock();
        cout << ". Time norm = " << t2 - t1;
        tn.push_back(t2 - t1);
        t1 = clock();
        alg_Krus_dsu_with_compr(graph,n,m);
        t2 = clock();
        cout << ". Time compres = " << t2 - t1 << "." <<endl;
        tc.push_back(t2 - t1);
    }
    for (auto i : tn)
        cout << i << "\n";
    cout << "\n";
    for (auto i : tc)
        cout << i << "\n";
}
void test5() {
    vector<edge> graph;
    vector<int> tn, tc;
    cout << " test5. n=1e4+1, m=n*1000, q=1, r=[1..200]/1" << endl;
    int n = 1e4+1;
    int m = n * 1000;
    for (int  r= 1; r <= 200; r ++) {
        graph = gengraph(n, m, 1, r);
        cout << "N = " << n << ", M = " << m;
        int t1, t2;
        t1 = clock();
        alg_Krus_dsu(graph, n, m);
        t2 = clock();
        cout << ". Time norm = " << t2 - t1;
        tn.push_back(t2 - t1);
        t1 = clock();
        alg_Krus_dsu_with_compr(graph, n, m);
        t2 = clock();
        cout << ". Time compres = " << t2 - t1 << "." << endl;
        tc.push_back(t2 - t1);
    }
    for (auto i : tn)
        cout << i << "\n";
    cout << "\n";
    for (auto i : tc)
        cout << i << "\n";
}

int main(signed argc, char* argv[]) {
    test1();
    test2();
    test3();
    test4();
    test5();
    return 0;
}