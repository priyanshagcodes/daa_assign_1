#include <iostream>
#include <vector>
#include <unordered_set>
#include <map>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>
#include <numeric>
#include <omp.h>
#include <stack>

using namespace std;
using namespace std::chrono;

class Graph {
public:
    int V;                       /
    vector<vector<bool>> adj;    
    vector<int> degree;          
    vector<int> order;           

    Graph(int V) : V(V) {
        adj.resize(V, vector<bool>(V, false));
        degree.resize(V, 0);
    }

    // Add an undirected edge.
    void addEdge(int u, int v) {
        if (u >= 0 && u < V && v >= 0 && v < V) {
            adj[u][v] = true;
            adj[v][u] = true;
            degree[u]++;
            degree[v]++;
        }
    }

    
    void orderVertices() {
        order.resize(V);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b) {
            return degree[a] < degree[b];
        });
    }

    
    void CLIQUE(const string &outputFileName, int &maximalCliqueCount);

private:
    
    bool isMaximal(const unordered_set<int> &C);
};

bool Graph::isMaximal(const unordered_set<int> &C) {
    
    for (int v = 0; v < V; v++) {
        if (C.find(v) == C.end()) {
            bool adjacentToAll = true;
            for (int u : C) {
                if (!adj[u][v]) {
                    adjacentToAll = false;
                    break;
                }
            }
            if (adjacentToAll)
                return false;
        }
    }
    return true;
}

void Graph::CLIQUE(const string &outputFileName, int &maximalCliqueCount) {
    orderVertices();
    
   
    vector<int> pos(V, 0);
    for (int j = 0; j < V; j++) {
        pos[order[j]] = j;
    }
    
    
    ofstream outFile(outputFileName);
    if (!outFile) {
        cerr << "Error: Could not open output file " << outputFileName << endl;
        return;
    }
    outFile << "Maximal Cliques:\n";
    
    
    struct StackFrame {
        int i;                     
        unordered_set<int> C;      
        unordered_set<int> P;      
    };
    stack<StackFrame> stack;
    
    
    unordered_set<int> P;
    for (int i = 0; i < V; i++) {
        P.insert(i);
    }
    
    
    stack.push({1, {}, P});
    
    map<int, int> cliqueHistogram;
    maximalCliqueCount = 0;
    
   
    while (!stack.empty()) {
        StackFrame frame = stack.top();
        stack.pop();
        
        int i = frame.i;
        unordered_set<int> C = frame.C;
        unordered_set<int> P = frame.P;
        
        
        if (i == V + 1) {
            if (!C.empty() && isMaximal(C)) {
                int size = C.size();
                cliqueHistogram[size]++;
                #pragma omp atomic
                maximalCliqueCount++;
                #pragma omp critical
                {
                    outFile << "{ ";
                    for (int v : C) outFile << v << " ";
                    outFile << "}\n";
                }
                if (maximalCliqueCount % 1000 == 0) {
                    #pragma omp critical
                    cout << "[DEBUG] " << maximalCliqueCount << " maximal cliques found." << endl;
                }
            }
            continue;
        }
        
        
        int current = order[i - 1];
        
       
        unordered_set<int> P1 = P;
        P1.erase(current);
        stack.push({i + 1, C, P1});
        

        bool canInclude = true;
        for (int u : C) {
            if (!adj[u][current]) {
                canInclude = false;
                break;
            }
        }
        if (canInclude) {
            unordered_set<int> C_new = C;
            C_new.insert(current);
          
            unordered_set<int> P_new;
            for (int v : P) {
                if (adj[current][v])
                    P_new.insert(v);
            }
            stack.push({i + 1, C_new, P_new});
        }
    }
    
    
    outFile << "\nClique Size Distribution (Histogram):\n";
    for (const auto &entry : cliqueHistogram)
        outFile << "Size " << entry.first << ": " << entry.second << " cliques\n";
    
    outFile << "\nTotal Number of Maximal Cliques: " << maximalCliqueCount << "\n";
    outFile.close();
}


int main() {
    try {
        const string inputFileName = "/content/Email-Enron.txt";
        ifstream inFile(inputFileName);
        if (!inFile) {
            cerr << "Error: Could not open file " << inputFileName << endl;
            return 1;
        }
        const string outputFileName = "/content/chiba-Email.txt";
        
        int n, m, u, v;
        inFile >> n >> m;
        if (n <= 0 || m <= 0) {
            cerr << "Error: Invalid graph size in file." << endl;
            return 1;
        }
        
        Graph g(n);
        map<int, int> vertexMap;
        int nextIndex = 0;
        vector<pair<int, int>> edges;
        while (inFile >> u >> v) {
            if (vertexMap.find(u) == vertexMap.end())
                vertexMap[u] = nextIndex++;
            if (vertexMap.find(v) == vertexMap.end())
                vertexMap[v] = nextIndex++;
            edges.push_back({vertexMap[u], vertexMap[v]});
        }
        inFile.close();
        
        for (const auto &edge : edges) {
            g.addEdge(edge.first, edge.second);
        }
        
        cout << "Successfully processed " << edges.size() << " edges.\n";
        
        auto start = high_resolution_clock::now();
        int maximalCliqueCount = 0;
        g.CLIQUE(outputFileName, maximalCliqueCount);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        
        cout << "Results saved to " << outputFileName << "\n";
        cout << "Total Number of Maximal Cliques: " << maximalCliqueCount << "\n";
        cout << "Time taken: " << duration.count() << " ms\n";
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    } catch (...) {
        cerr << "Unknown error occurred.\n";
        return 1;
    }
    
    return 0;
}
