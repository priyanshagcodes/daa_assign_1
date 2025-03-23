#include <bits/stdc++.h>
#include <thread>
#include <atomic>
using namespace std;
using namespace std::chrono;

class Graph {
public:
    int n;                       // Number of vertices
    vector<vector<bool>> adj;    // Adjacency matrix (compressed)
    vector<int> order;           // Vertices ordered by non-increasing degree
    atomic<int> totalMaximalCliques{0};
    map<int, int> cliqueSizeDistribution;
    
    Graph(int n) : n(n) {
        adj.resize(n, vector<bool>(n, false));
    }
    
    void addEdge(int u, int v) {
        if(u == v) return;
        adj[u][v] = true;
        adj[v][u] = true;
    }
    
    // Order vertices by non-increasing degree
    void orderVertices() {
        vector<int> deg(n);
        for(int i = 0; i < n; i++) {
            deg[i] = count(adj[i].begin(), adj[i].end(), true);
        }
        order.resize(n);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int a, int b) {
            return deg[a] > deg[b]; // Non-increasing order
        });
    }
    
    // Iterative clique enumeration
    void CLIQUE(const string &outputFileName);
};

void Graph::CLIQUE(const string &outputFileName) {
    orderVertices(); // Order vertices by non-decreasing degree.
    
    // Precompute positions of vertices in the order array.
    vector<int> pos(n, 0);
    for (int j = 0; j < n; j++) {
        pos[order[j]] = j;
    }
    
    // Open output file
    ofstream outFile(outputFileName);
    if(!outFile) {
        cerr << "Error: Could not open output file " << outputFileName << endl;
        return;
    }
    
    // Stack for iterative DFS
    struct State {
        int i;                  // Current vertex index in order
        vector<int> C;          // Current clique
        vector<int> P;          // Candidate set
    };
    stack<State> stack;
    
    // Initial state
    State initialState;
    initialState.i = 0;
    initialState.P.resize(n);
    iota(initialState.P.begin(), initialState.P.end(), 0);
    stack.push(initialState);
    
    // Iterative DFS
    while (!stack.empty()) {
        State state = stack.top();
        stack.pop();
        
        if (state.i == n) {
            // Check if C is maximal
            bool isMaximal = true;
            for (int v : state.P) {
                bool extendable = true;
                for (int u : state.C) {
                    if (!adj[u][v]) {
                        extendable = false;
                        break;
                    }
                }
                if (extendable) {
                    isMaximal = false;
                    break;
                }
            }
            if (isMaximal && !state.C.empty()) {
                totalMaximalCliques++;
                cliqueSizeDistribution[state.C.size()]++;
                if (totalMaximalCliques % 1000 == 0) {
                    cout << "[DEBUG] " << totalMaximalCliques << " maximal cliques found." << endl;
                }
                // Write clique to file
                outFile << "{ ";
                for (int v : state.C) outFile << v << " ";
                outFile << "}\n";
            }
            continue;
        }
        
        int current = order[state.i];
        
        // Branch 1: Do not include current
        State newState1 = state;
        newState1.i++;
        stack.push(newState1);
        
        // Branch 2: Include current if it forms a clique with C
        bool canInclude = true;
        for (int u : state.C) {
            if (!adj[u][current]) {
                canInclude = false;
                break;
            }
        }
        if (canInclude) {
            State newState2 = state;
            newState2.C.push_back(current);
            newState2.P.clear();
            for (int x : state.P) {
                if (pos[x] > pos[current] && adj[current][x]) {
                    newState2.P.push_back(x);
                }
            }
            newState2.i++;
            stack.push(newState2);
        }
    }
    
    // Write statistics to file
    outFile << "\nClique Size Distribution (Histogram):\n";
    for (const auto &entry : cliqueSizeDistribution) {
        outFile << "Size " << entry.first << ": " << entry.second << " cliques\n";
    }
    outFile << "\nTotal Number of Maximal Cliques: " << totalMaximalCliques << "\n";
    outFile.close();
}

//////////////////////
// Main function
//////////////////////
int main() {
    try {
        const string inputFileName = "/content/Email-Enron.txt";
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
        // Map original vertex IDs to 0-based indices.
        unordered_map<int,int> vertexMap;
        int nextIndex = 0;
        vector<pair<int,int>> edges;
        while(in >> u >> v) {
            if(vertexMap.find(u) == vertexMap.end()) {
                vertexMap[u] = nextIndex++;
            }
            if(vertexMap.find(v) == vertexMap.end()) {
                vertexMap[v] = nextIndex++;
            }
            edges.push_back({vertexMap[u], vertexMap[v]});
        }
        in.close();
        
        // Add edges to the graph.
        for(const auto &edge : edges) {
            g.addEdge(edge.first, edge.second);
        }
        
        cout << "Successfully processed " << edges.size() << " edges." << endl;
        
        auto start = high_resolution_clock::now();
        g.CLIQUE(outputFileName);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<chrono::milliseconds>(stop - start);
        
        cout << "Results saved to " << outputFileName << endl;
        cout << "Total Number of Maximal Cliques: " << g.totalMaximalCliques << endl;
        cout << "Time taken: " << duration.count() << " ms" << endl;
    } catch(const exception &e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    } catch(...) {
        cerr << "Unknown error occurred.\n";
        return 1;
    }
    
    return 0;
}
