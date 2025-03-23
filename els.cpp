#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Graph {
public:
    int n;
    vector<set<int>> adj;

    Graph(int n) : n(n), adj(n) {}

    void addEdge(int u, int v) {
        if (u >= 0 && u < n && v >= 0 && v < n) {
            adj[u].insert(v);
            adj[v].insert(u);
        } else {
            cerr << "Warning: Edge (" << u << "," << v << ") out of bounds. Skipping.\n";
        }
    }

    void tomita(vector<int>& R, set<int>& P, set<int>& X, ofstream& outputFile, map<int, int>& clique_size_distribution, int& clique_count) {
        if (P.empty() && X.empty()) {
            clique_count++;
            clique_size_distribution[R.size()]++;

            // Write clique to file directly
            outputFile << "{ ";
            for (int v : R) outputFile << v << " ";
            outputFile << "}\n";

            return;
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

        set<int> P_copy = P;
        for (int v : P_copy) {
            if (pivot != -1 && adj[pivot].count(v)) continue;

            vector<int> R_new = R;
            R_new.push_back(v);

            set<int> P_new, X_new;
            for (int u : P) {
                if (adj[v].count(u)) P_new.insert(u);
            }
            for (int u : X) {
                if (adj[v].count(u)) X_new.insert(u);
            }

            tomita(R_new, P_new, X_new, outputFile, clique_size_distribution, clique_count);
            P.erase(v);
            X.insert(v);
        }
    }

    void findMaximalCliques(ofstream& outputFile, map<int, int>& clique_size_distribution, int& clique_count) {
        vector<int> R;
        set<int> P, X;
        for (int i = 0; i < n; i++) P.insert(i);
        tomita(R, P, X, outputFile, clique_size_distribution, clique_count);
    }
};

int main() {
    ifstream inputFile("Email-Enron.txt");
    if (!inputFile) {
        cerr << "Error: Could not open file Email-Enron.txt\n";
        return 1;
    }

    int n, m, u, v;
    inputFile >> n >> m;

    if (n <= 0 || m <= 0) {
        cerr << "Error: Invalid graph size in file.\n";
        return 1;
    }

    Graph g(n);
    map<int, int> vertex_map;
    int next_index = 0;
    vector<pair<int, int>> edges;

    while (inputFile >> u >> v) {
        if (vertex_map.find(u) == vertex_map.end()) vertex_map[u] = next_index++;
        if (vertex_map.find(v) == vertex_map.end()) vertex_map[v] = next_index++;
        edges.emplace_back(vertex_map[u], vertex_map[v]);
    }
    inputFile.close();

    for (const auto& edge : edges) {
        g.addEdge(edge.first, edge.second);
    }

    cout << "Successfully processed " << edges.size() << " edges.\n";

    ofstream outputFile("els-EmailEnron.txt");
    if (!outputFile) {
        cerr << "Error: Could not open output file worst-case-Email.txt\n";
        return 1;
    }

    auto start = high_resolution_clock::now();

    int clique_count = 0;
    map<int, int> clique_size_distribution;

    g.findMaximalCliques(outputFile, clique_size_distribution, clique_count);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    outputFile << "\nNumber of maximal cliques: " << clique_count << "\n";
    outputFile << "Time taken: " << duration.count() << " ms\n";
    outputFile << "\nClique Size Distribution:\n";
    for (const auto& entry : clique_size_distribution) {
        outputFile << "Size " << entry.first << ": " << entry.second << " cliques\n";
    }

    cout << "Number of maximal cliques: " << clique_count << endl;
    cout << "Clique Size Distribution:\n";
    for (const auto& entry : clique_size_distribution) {
        cout << "Size " << entry.first << ": " << entry.second << " cliques\n";
    }

    cout << "Results saved to worst-case-Email.txt\n";
    outputFile.close();
    return 0;
}
