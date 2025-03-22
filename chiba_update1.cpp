#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>

using namespace std;
using namespace std::chrono;

class Graph {
public:
    int V; // Number of vertices
    vector<set<int>> adj; // Adjacency list representation

    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int v, int w) {
        if (v >= 0 && v < V && w >= 0 && w < V) {
            adj[v].insert(w);
            adj[w].insert(v);
        } else {
            cerr << "Warning: Edge (" << v << "," << w << ") out of bounds. Skipping." << endl;
        }
    }

    void findMaximalCliques(ofstream& outputFile, int& maximalCliqueCount);

private:
    void expand(vector<int>& R, vector<int>& P, vector<int>& X, ofstream& outputFile, map<int, int>& clique_size_distribution, int& maximalCliqueCount);
};

void Graph::expand(vector<int>& R, vector<int>& P, vector<int>& X, ofstream& outputFile, map<int, int>& clique_size_distribution, int& maximalCliqueCount) {
    if (P.empty() && X.empty()) {
        int size = R.size();
        clique_size_distribution[size]++;  // Update histogram count
        maximalCliqueCount++;  // Increment clique count

        outputFile << "{ ";
        for (int v : R) outputFile << v << " ";
        outputFile << "}\n";
        return;
    }

    vector<int> P_copy = P; // Copy of P to iterate over
    for (int v : P_copy) {
        if (v < 0 || v >= V) continue;  // Skip invalid vertices

        vector<int> R_new = R, P_new, X_new;
        R_new.push_back(v);

        // Intersect P with neighbors of v
        for (int w : P)
            if (w >= 0 && w < V && adj[v].count(w))
                P_new.push_back(w);

        // Intersect X with neighbors of v
        for (int w : X)
            if (w >= 0 && w < V && adj[v].count(w))
                X_new.push_back(w);

        expand(R_new, P_new, X_new, outputFile, clique_size_distribution, maximalCliqueCount);

        // Remove v from P and add to X
        P.erase(remove(P.begin(), P.end(), v), P.end());
        X.push_back(v);
    }
}

void Graph::findMaximalCliques(ofstream& outputFile, int& maximalCliqueCount) {
    vector<int> R, P, X;
    map<int, int> clique_size_distribution;

    // Initialize P with all vertices
    for (int i = 0; i < V; i++)
        P.push_back(i);

    // Call expand to find all maximal cliques
    expand(R, P, X, outputFile, clique_size_distribution, maximalCliqueCount);

    // Print clique size distribution to output file
    outputFile << "\nClique Size Distribution (Histogram):\n";
    for (const auto& entry : clique_size_distribution) {
        outputFile << "Size " << entry.first << ": " << entry.second << " cliques\n";
    }

    // Print total number of maximal cliques
    outputFile << "\nTotal Number of Maximal Cliques: " << maximalCliqueCount << "\n";
}

int main() {
    try {
        // Input file name
        const string inputFileName = "Email-Enron.txt";
        ifstream inputFile(inputFileName);
        if (!inputFile) {
            cerr << "Error: Could not open file " << inputFileName << endl;
            return 1;
        }

        // Output file name
        const string outputFileName = "chiba-Email.txt";
        ofstream outputFile(outputFileName);
        if (!outputFile) {
            cerr << "Error: Could not open output file " << outputFileName << endl;
            return 1;
        }

        int n, m, u, v;
        inputFile >> n >> m; // Read number of vertices and edges

        if (n <= 0 || m <= 0) {
            cerr << "Error: Invalid graph size in file." << endl;
            return 1;
        }

        Graph g(n);
        map<int, int> vertex_map; // Maps original vertex IDs to zero-based indices
        int next_index = 0;
        vector<pair<int, int>> edges;

        while (inputFile >> u >> v) {
            if (vertex_map.find(u) == vertex_map.end()) {
                vertex_map[u] = next_index++;
            }
            if (vertex_map.find(v) == vertex_map.end()) {
                vertex_map[v] = next_index++;
            }
            edges.emplace_back(vertex_map[u], vertex_map[v]);
        }
        inputFile.close();

        // Add edges to graph
        for (const auto& edge : edges) {
            g.addEdge(edge.first, edge.second);
        }

        outputFile << "Successfully processed " << edges.size() << " edges.\n";

        auto start = high_resolution_clock::now();

        outputFile << "Maximal Cliques:\n";
        int maximalCliqueCount = 0; // Counter for the total number of maximal cliques

        g.findMaximalCliques(outputFile, maximalCliqueCount);

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);

        outputFile << "Time taken: " << duration.count() << " ms\n";
        cout << "Results saved to output.txt\n";
        cout << "Total Number of Maximal Cliques: " << maximalCliqueCount << "\n";
        cout << "Time taken: " << duration.count() << " ms\n";

        outputFile.close();
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    catch (...) {
        cerr << "Unknown error occurred." << endl;
        return 1;
    }

    return 0;
}