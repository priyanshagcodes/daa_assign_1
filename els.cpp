#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

class Graph {
public:
    int n;                       // Number of vertices
    vector<vector<int>> adj;     // Adjacency list
    vector<int> order;           // Vertices ordered by non-increasing degree
    int totalMaximalCliques = 0;
    map<int, int> cliqueSizeDistribution;
    
    Graph(int n) : n(n) {
        adj.resize(n);
    }
    
    // Add an undirected edge (if u==v, do nothing)
    void addEdge(int u, int v) {
        if(u == v) return;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    // Order vertices by non-increasing degree.
    void orderVertices() {
        vector<int> deg(n);
        for(int i = 0; i < n; i++){
            deg[i] = adj[i].size();
        }
        order.resize(n);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b) {
            return deg[a] > deg[b];
        });
    }
    
    // Recursive UPDATE procedure.
    // i ranges from 1 to n+1 (using 1-indexed positions into 'order').
    // C is the current clique (vector of vertex indices).
    // P is the set of candidate vertices (from the ordered set).
    void UPDATE(int i, vector<int>& C, set<int>& P, vector<string>& outputBuffer, 
                map<int,int>& cliqueHistogram, int &maximalCliqueCount);
    
    // Returns true if C is maximal (i.e. no vertex outside C is adjacent to every member of C).
    bool isMaximal(const vector<int>& C);
    
    // Top-level function: calls UPDATE and writes results.
    void CLIQUE(const string &outputFileName, int &maximalCliqueCount);
};

bool Graph::isMaximal(const vector<int>& C) {
    // For every vertex not in C, if that vertex is adjacent to every vertex in C,
    // then C can be extended (so C is not maximal).
    for (int v = 0; v < n; v++) {
        if (find(C.begin(), C.end(), v) == C.end()) {
            bool extendable = true;
            for (int u : C) {
                if (find(adj[u].begin(), adj[u].end(), v) == adj[u].end()){
                    extendable = false;
                    break;
                }
            }
            if (extendable)
                return false;
        }
    }
    return true;
}

void Graph::UPDATE(int i, vector<int>& C, set<int>& P, vector<string>& outputBuffer, 
                   map<int,int>& cliqueHistogram, int &maximalCliqueCount) {
    // Base case: if we have processed all vertices in the ordered list.
    if(i == n + 1) {
        if(!C.empty() && isMaximal(C)) {
            cliqueHistogram[C.size()]++;
            maximalCliqueCount++;
            if(maximalCliqueCount % 1000 == 0)
                cout << "[DEBUG] " << maximalCliqueCount << " maximal cliques found." << endl;
            // Convert clique C to a string.
            string cliqueStr = "{ ";
            for (int v : C)
                cliqueStr += to_string(v) + " ";
            cliqueStr += "}";
            outputBuffer.push_back(cliqueStr);
        }
        return;
    }
    
    // Let current vertex be the one in 'order' at position (i-1).
    int current = order[i-1];
    
    // Branch 1: Do not include current vertex.
    set<int> P1 = P;
    P1.erase(current);
    UPDATE(i + 1, C, P1, outputBuffer, cliqueHistogram, maximalCliqueCount);
    
    // Branch 2: If current is adjacent to every vertex in C, try including it.
    bool canInclude = true;
    for (int u : C) {
        if (find(adj[u].begin(), adj[u].end(), current) == adj[u].end()) {
            canInclude = false;
            break;
        }
    }
    if(canInclude) {
        vector<int> C_new = C;
        C_new.push_back(current);
        // Build a position mapping for the ordering.
        vector<int> pos(n, 0);
        for (int j = 0; j < n; j++) {
            pos[order[j]] = j;
        }
        // New candidate set: intersection of P and neighbors of current,
        // but only those vertices that appear later than current in the order.
        set<int> P_new;
        for (int x : P) {
            if (pos[x] > pos[current]) { // Only those later than current.
                if (find(adj[current].begin(), adj[current].end(), x) != adj[current].end())
                    P_new.insert(x);
            }
        }
        UPDATE(i + 1, C_new, P_new, outputBuffer, cliqueHistogram, maximalCliqueCount);
    }
}

void Graph::CLIQUE(const string &outputFileName, int &maximalCliqueCount) {
    orderVertices(); // Order vertices by non-increasing degree.
    
    // Initial candidate set P: all vertices.
    set<int> P;
    for (int i = 0; i < n; i++) {
        P.insert(i);
    }
    
    vector<int> C;  // Initially empty clique.
    map<int,int> cliqueHistogram;
    vector<string> outputBuffer;
    
    UPDATE(1, C, P, outputBuffer, cliqueHistogram, maximalCliqueCount);
    
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

//////////////////////
// Main function
//////////////////////
int main() {
    try {
        const string inputFileName = "/content/wiki-Vote.txt";
        const string outputFileName = "/content/clique_output.txt";
        ifstream in(inputFileName);
        if (!in) {
            cerr << "Error: Could not open input file " << inputFileName << endl;
            return 1;
        }
        
        int numVertices, numEdges, u, v;
        in >> numVertices >> numEdges;
        if(numVertices <= 0 || numEdges <= 0) {
            cerr << "Error: Invalid graph size in file." << endl;
            return 1;
        }
        
        // Build graph using given number of vertices.
        Graph g(numVertices);

        unordered_map<int, int> vertexMap;
        int nextIndex = 0;
        vector<pair<int, int>> edges;
        while(in >> u >> v) {
            if(vertexMap.find(u) == vertexMap.end())
                vertexMap[u] = nextIndex++;
            if(vertexMap.find(v) == vertexMap.end())
                vertexMap[v] = nextIndex++;
            edges.push_back({vertexMap[u], vertexMap[v]});
        }
        in.close();
        
        for (const auto &edge : edges) {
            g.addEdge(edge.first, edge.second);
        }
        
        cout << "Successfully processed " << edges.size() << " edges." << endl;
        
        auto start = high_resolution_clock::now();
        int maximalCliqueCount = 0;
        g.CLIQUE(outputFileName, maximalCliqueCount);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        
        cout << "Results saved to " << outputFileName << "\n";
        cout << "Total Number of Maximal Cliques: " << maximalCliqueCount << "\n";
        cout << "Time taken: " << duration.count() << " ms" << "\n";
    } catch(const exception &e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    } catch(...) {
        cerr << "Unknown error occurred.\n";
        return 1;
    }
    
    return 0;
}
