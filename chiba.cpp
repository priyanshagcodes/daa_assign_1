%%writefile graph_cliques.cpp

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

using namespace std;
using namespace std::chrono;

class Graph {
public:
    int V;                       
    vector<vector<bool>> adj;    
    vector<int> degree;          
    vector<int> order;          

    Graph(int V) : V(V) {
        adj.resize(V, vector<bool>(V, false));
        degree.resize(V, 0);
    }

  
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

    void UPDATE(int i, unordered_set<int> &C, unordered_set<int> &P, vector<string> &outputBuffer, 
                map<int,int> &cliqueHistogram, int &maximalCliqueCount, const vector<int> &pos);
  
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

void Graph::UPDATE(int i, unordered_set<int> &C, unordered_set<int> &P, vector<string> &outputBuffer, 
                   map<int,int> &cliqueHistogram, int &maximalCliqueCount, const vector<int> &pos) {
  
    if(i == V+1) {
        if(!C.empty() && isMaximal(C)) {
            int size = C.size();
            cliqueHistogram[size]++;
            #pragma omp atomic
            maximalCliqueCount++;
            #pragma omp critical
            {
                string cliqueStr = "{ ";
                for (int v : C) cliqueStr += to_string(v) + " ";
                cliqueStr += "}";
                outputBuffer.push_back(cliqueStr);
            }
            if(maximalCliqueCount % 1000 == 0) {
                #pragma omp critical
                cout << "[DEBUG] " << maximalCliqueCount << " maximal cliques found." << endl;
            }
        }
        return;
    }
    
    int current = order[i-1];
    
    unordered_set<int> P1 = P;
    P1.erase(current);
    UPDATE(i+1, C, P1, outputBuffer, cliqueHistogram, maximalCliqueCount, pos);
    
   
    bool canInclude = true;
    for (int u : C) {
        if (!adj[u][current]) {
            canInclude = false;
            break;
        }
    }
    if(canInclude) {
        unordered_set<int> C_new = C;
        C_new.insert(current);
   
        unordered_set<int> P_new;
        for (int v : P) {
            if (adj[current][v])
                P_new.insert(v);
        }
        
        if(C.size() < 3) {
            #pragma omp task shared(maximalCliqueCount, cliqueHistogram, outputBuffer)
            {
                UPDATE(i+1, C_new, P_new, outputBuffer, cliqueHistogram, maximalCliqueCount, pos);
            }
            #pragma omp taskwait
        } else {
            UPDATE(i+1, C_new, P_new, outputBuffer, cliqueHistogram, maximalCliqueCount, pos);
        }
    }
}

void Graph::CLIQUE(const string &outputFileName, int &maximalCliqueCount) {
    orderVertices();
    
    vector<int> pos(V, 0);
    for (int j = 0; j < V; j++) {
        pos[order[j]] = j;
    }
    
   
    unordered_set<int> C;
    unordered_set<int> P;
    for (int i = 0; i < V; i++) {
        P.insert(i);
    }
    
    map<int,int> cliqueHistogram;
    vector<string> outputBuffer;
    
    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            UPDATE(1, C, P, outputBuffer, cliqueHistogram, maximalCliqueCount, pos);
        }
    }
    
    // Write results to file.
    ofstream outFile(outputFileName);
    if(!outFile) {
        cerr << "Error: Could not open output file " << outputFileName << endl;
        return;
    }
    outFile << "Maximal Cliques:\n";
    for (const auto &line : outputBuffer)
        outFile << line << "\n";
    
    outFile << "\nClique Size Distribution (Histogram):\n";
    for (const auto &entry : cliqueHistogram)
        outFile << "Size " << entry.first << ": " << entry.second << " cliques\n";
    
    outFile << "\nTotal Number of Maximal Cliques: " << maximalCliqueCount << "\n";
    outFile.close();
}


int main() {
    try {
        const string inputFileName = "/content/wiki-Vote.txt";
        ifstream inFile(inputFileName);
        if (!inFile) {
            cerr << "Error: Could not open file " << inputFileName << endl;
            return 1;
        }
        const string outputFileName = "/content/chiba-Email.txt";
        
        int n, m, u, v;
        inFile >> n >> m;
        if(n <= 0 || m <= 0) {
            cerr << "Error: Invalid graph size in file." << endl;
            return 1;
        }
        
        Graph g(n);
        map<int, int> vertexMap;
        int nextIndex = 0;
        vector<pair<int,int>> edges;
        while(inFile >> u >> v) {
            if(vertexMap.find(u) == vertexMap.end())
                vertexMap[u] = nextIndex++;
            if(vertexMap.find(v) == vertexMap.end())
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
