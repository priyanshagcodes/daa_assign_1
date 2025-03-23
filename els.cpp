#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <chrono>
#include <map>
#include <unordered_map>

using namespace std;
using namespace std::chrono;

class Graph {
private:
    int n, m;
    vector<vector<int>> adj;
    int degeneracy;
    vector<int> degeneracy_ordering;
    int total_cliques;
    map<int, int> clique_sizes;

public:
    Graph(int n, int m) : n(n), m(m), adj(n), total_cliques(0), degeneracy(0) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void computeDegeneracyOrdering() {
        vector<int> degree(n, 0);
        vector<vector<int>> bin(n);
        vector<bool> removed(n, false);
        degeneracy_ordering.clear();

        for (int i = 0; i < n; i++) {
            degree[i] = adj[i].size();
            bin[degree[i]].push_back(i);
        }

        for (int i = 0, v = 0; i < n; i++) {
            int j = 0;
            while (j < n && bin[j].empty()) j++;
            if (j == n) break;
            v = bin[j].back();
            bin[j].pop_back();
            removed[v] = true;
            degeneracy_ordering.push_back(v);

            for (int neighbor : adj[v]) {
                if (!removed[neighbor]) {
                    bin[degree[neighbor]].erase(
                        find(bin[degree[neighbor]].begin(), bin[degree[neighbor]].end(), neighbor)
                    );
                    degree[neighbor]--;
                    bin[degree[neighbor]].push_back(neighbor);
                }
            }
            degeneracy = max(degeneracy, j);
        }
        reverse(degeneracy_ordering.begin(), degeneracy_ordering.end());
    }

    void bronKerboschPivot(unordered_set<int> R, unordered_set<int> P, unordered_set<int> X) {
        if (P.empty() && X.empty()) {
            total_cliques++;
            clique_sizes[R.size()]++;
            if (total_cliques % 10000 == 0) {
                cout << "[Milestone] Found " << total_cliques << " cliques so far. Latest size: " << R.size() << endl;
            }
            return;
        }

        int pivot = -1, max_connections = -1;
        for (int v : P) {
            int connections = count_if(P.begin(), P.end(), [&](int u) { return find(adj[v].begin(), adj[v].end(), u) != adj[v].end(); });
            if (connections > max_connections) {
                max_connections = connections;
                pivot = v;
            }
        }

        unordered_set<int> P_minus_neighbors;
        for (int v : P) {
            if (pivot == -1 || find(adj[pivot].begin(), adj[pivot].end(), v) == adj[pivot].end()) {
                P_minus_neighbors.insert(v);
            }
        }

        for (int v : P_minus_neighbors) {
            unordered_set<int> new_R = R;
            new_R.insert(v);
            unordered_set<int> new_P, new_X;
            for (int w : P) {
                if (find(adj[v].begin(), adj[v].end(), w) != adj[v].end()) {
                    new_P.insert(w);
                }
            }
            for (int w : X) {
                if (find(adj[v].begin(), adj[v].end(), w) != adj[v].end()) {
                    new_X.insert(w);
                }
            }
            bronKerboschPivot(new_R, new_P, new_X);
            P.erase(v);
            X.insert(v);
        }
    }

    void findAllMaximalCliques() {
        if (degeneracy_ordering.empty()) {
            computeDegeneracyOrdering();
        }
        for (int i = 0; i < n; i++) {
            int v = degeneracy_ordering[i];
            unordered_set<int> P, X;
            for (int neighbor : adj[v]) {
                if (find(degeneracy_ordering.begin() + i + 1, degeneracy_ordering.end(), neighbor) != degeneracy_ordering.end()) {
                    P.insert(neighbor);
                }
                if (find(degeneracy_ordering.begin(), degeneracy_ordering.begin() + i, neighbor) != degeneracy_ordering.begin() + i) {
                    X.insert(neighbor);
                }
            }
            unordered_set<int> R = {v};
            bronKerboschPivot(R, P, X);
        }
    }

    void processAndFindCliques(const string& inputFile, const string& outputFile) {
        auto start_time = high_resolution_clock::now();
        ifstream fin(inputFile);
        fin >> n >> m;
        adj.resize(n);

        unordered_map<int, int> vertexMap;
        int nextIndex = 0;
        for (int i = 0; i < m; i++) {
            int u, v;
            fin >> u >> v;
            if (vertexMap.find(u) == vertexMap.end()) vertexMap[u] = nextIndex++;
            if (vertexMap.find(v) == vertexMap.end()) vertexMap[v] = nextIndex++;
            addEdge(vertexMap[u], vertexMap[v]);
        }
        fin.close();
        computeDegeneracyOrdering();
        auto clique_start = high_resolution_clock::now();
        findAllMaximalCliques();
        auto clique_end = high_resolution_clock::now();
        auto end_time = high_resolution_clock::now();
        auto clique_duration = duration_cast<milliseconds>(clique_end - clique_start);
        auto total_duration = duration_cast<milliseconds>(end_time - start_time);
        ofstream fout(outputFile);
        fout << "Number of maximal cliques: " << total_cliques << endl;
        fout << "Time taken for clique finding: " << clique_duration.count() << " ms" << endl;
        fout << "Total execution time: " << total_duration.count() << " ms" << endl;
        fout << "Clique Size Distribution:" << endl;
        for (const auto& pair : clique_sizes) {
            fout << "Size " << pair.first << ": " << pair.second << " cliques" << endl;
        }
        fout.close();
    }
};

int main() {
    Graph g(0, 0);
    g.processAndFindCliques("/content/wiki-Vote.txt", "/content/Els-as-skitter.txt");
    return 0;
}
