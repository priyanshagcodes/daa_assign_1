#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <sstream>

using namespace std;
using namespace std::chrono;

class Graph {
public:
    int n;
    vector<unordered_set<int>> adj; // Adjacency list

    Graph(int n) : n(n), adj(n) {}

    void addEdge(int u, int v) {
        if (u >= 0 && u < n && v >= 0 && v < n) {
            adj[u].insert(v);
            adj[v].insert(u);
        }
    }

    void tomitaIterative(int& maximalCliqueCount, unordered_map<int, int>& cliqueSizeDistribution) {
        vector<int> R;
        unordered_set<int> P, X;
        for (int i = 0; i < n; i++) P.insert(i);

        vector<tuple<vector<int>, unordered_set<int>, unordered_set<int>>> stack;
        stack.emplace_back(R, P, X);

        while (!stack.empty()) {
            auto [R, P, X] = stack.back();
            stack.pop_back();

            if (P.empty() && X.empty()) {
                int size = R.size();
                cliqueSizeDistribution[size]++;
                maximalCliqueCount++;
                continue;
            }

            int pivot = -1, max_deg = -1;
            for (int v : P) {
                if ((int)adj[v].size() > max_deg) {
                    max_deg = adj[v].size();
                    pivot = v;
                }
            }
            for (int v : X) {
                if ((int)adj[v].size() > max_deg) {
                    max_deg = adj[v].size();
                    pivot = v;
                }
            }

            unordered_set<int> P_copy = P;
            for (int v : P_copy) {
                if (pivot != -1 && adj[pivot].count(v)) continue;

                vector<int> R_new = R;
                R_new.push_back(v);

                unordered_set<int> P_new, X_new;
                for (int u : P) {
                    if (adj[v].count(u)) P_new.insert(u);
                }
                for (int u : X) {
                    if (adj[v].count(u)) X_new.insert(u);
                }

                stack.emplace_back(R_new, P_new, X_new);
                P.erase(v);
                X.insert(v);
            }
        }
    }

    void findMaximalCliques(int& maximalCliqueCount, unordered_map<int, int>& cliqueSizeDistribution) {
        tomitaIterative(maximalCliqueCount, cliqueSizeDistribution);
    }
};

void fastInputProcessing(const string& filename, vector<pair<int, int>>& edges, int& n, int& m) {
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Could not open file " << filename << endl;
        exit(1);
    }

    string line;
    getline(inputFile, line);
    stringstream ss(line);
    ss >> n >> m;

    unordered_map<int, int> vertex_map;
    int next_index = 0;

    while (getline(inputFile, line)) {
        stringstream ss(line);
        int u, v;
        ss >> u >> v;

        if (vertex_map.find(u) == vertex_map.end()) vertex_map[u] = next_index++;
        if (vertex_map.find(v) == vertex_map.end()) vertex_map[v] = next_index++;
        edges.emplace_back(vertex_map[u], vertex_map[v]);
    }
    inputFile.close();
}

int main() {
    string inputFile = "as-skitter.txt";
    vector<pair<int, int>> edges;
    int n, m;

    auto start_io = high_resolution_clock::now();
    fastInputProcessing(inputFile, edges, n, m);
    auto stop_io = high_resolution_clock::now();

    auto io_duration = duration_cast<milliseconds>(stop_io - start_io);

    Graph g(n);
    for (const auto& edge : edges) {
        g.addEdge(edge.first, edge.second);
    }

    cout << "Processed " << edges.size() << " edges. (I/O Time: " << io_duration.count() << " ms)" << endl;

    int maximalCliqueCount = 0;
    unordered_map<int, int> cliqueSizeDistribution;

    auto start = high_resolution_clock::now();
    g.findMaximalCliques(maximalCliqueCount, cliqueSizeDistribution);
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start);

    ofstream outputFile("worst-case-faster-skitter.txt");
    if (!outputFile) {
        cerr << "Error: Could not open output file" << endl;
        return 1;
    }

    
    outputFile << "Number of maximal cliques: " << maximalCliqueCount << "\n";
    outputFile << "Execution Time: " << duration.count() << " ms\n";
    outputFile << "Clique Size Distribution:\n";
    for (const auto& entry : cliqueSizeDistribution) {
        outputFile << "Size " << entry.first << ": " << entry.second << " cliques\n";
    }
    outputFile.close();

  
    cout << "Execution Time: " << duration.count() << " ms" << endl;
    cout << "Number of maximal cliques: " << maximalCliqueCount << endl;
    cout << "Clique Size Distribution:" << endl;
    for (const auto& entry : cliqueSizeDistribution) {
        cout << "Size " << entry.first << ": " << entry.second << " cliques" << endl;
    }

    return 0;
}
