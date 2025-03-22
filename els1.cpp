#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <chrono>
#include <algorithm>
#include <fstream>

using namespace std;
using namespace std::chrono;

typedef vector<set<int>> Graph;
const int MAX_RECURSION_DEPTH = 1000;

// Function to choose the pivot that maximizes |P ∩ Γ(u)|
int choose_pivot(const set<int>& P, const set<int>& X, Graph& graph) {
    int best_pivot = -1, max_neighbors = -1;
    for (int u : P) {
        int neighbors = 0;
        for (int v : graph[u]) {
            if (P.count(v)) neighbors++;
        }
        if (neighbors > max_neighbors) {
            max_neighbors = neighbors;
            best_pivot = u;
        }
    }
    for (int u : X) {  // Also check X
        int neighbors = 0;
        for (int v : graph[u]) {
            if (P.count(v)) neighbors++;
        }
        if (neighbors > max_neighbors) {
            max_neighbors = neighbors;
            best_pivot = u;
        }
    }
    return best_pivot;
}

// Optimized Bron-Kerbosch algorithm with pivoting
void BronKerbosch(set<int> R, set<int> P, set<int> X, Graph &graph, vector<set<int>> &maximal_cliques, int depth = 0) {
    if (depth > MAX_RECURSION_DEPTH) return;

    if (P.empty() && X.empty()) {
        maximal_cliques.push_back(R);  // Store maximal clique
        return;
    }

    // Choose a pivot
    int pivot = choose_pivot(P, X, graph);

    // Process vertices not in pivot's neighbors
    set<int> non_neighbors;
    for (int v : P) {
        if (!graph[pivot].count(v)) non_neighbors.insert(v);
    }

    for (int v : non_neighbors) {
        set<int> newR = R, newP, newX;
        newR.insert(v);

        for (int u : graph[v]) {
            if (P.count(u)) newP.insert(u);
            if (X.count(u)) newX.insert(u);
        }

        BronKerbosch(newR, newP, newX, graph, maximal_cliques, depth + 1);

        P.erase(v);
        X.insert(v);
    }
}

// Function to find degeneracy ordering
vector<int> degeneracy_ordering(Graph &graph) {
    int n = graph.size();
    vector<int> order, degree(n);
    set<pair<int, int>> degree_set;

    for (int i = 0; i < n; i++) {
        degree[i] = graph[i].size();
        degree_set.insert({degree[i], i});
    }

    while (!degree_set.empty()) {
        int v = degree_set.begin()->second;
        degree_set.erase(degree_set.begin());
        order.push_back(v);

        for (int u : graph[v]) {
            if (degree_set.count({degree[u], u})) {
                degree_set.erase({degree[u], u});
                degree[u]--;
                degree_set.insert({degree[u], u});
            }
        }
    }

    reverse(order.begin(), order.end());
    return order;
}

int main() {
    
    ifstream infile("wiki-Vote.txt");  // Open the input file
    if (!infile) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    int n, m, u, v;
    infile >> n >> m;  // Read number of vertices and edges
    Graph graph(n);
    unordered_map<int, int> vertex_map;  // Maps original vertex IDs to zero-based IDs
    int next_index = 0;

    vector<pair<int, int>> edges;

    while (infile >> u >> v) {
        if (vertex_map.find(u) == vertex_map.end()) vertex_map[u] = next_index++;
        if (vertex_map.find(v) == vertex_map.end()) vertex_map[v] = next_index++;
        edges.emplace_back(vertex_map[u], vertex_map[v]);
    }
    infile.close();

    int ed = 0;
    for (const auto &edge : edges) {
        int a = edge.first, b = edge.second;
        if (a < n && b < n) {
            graph[a].insert(b);
            graph[b].insert(a);
            ed++;
        } else {
            cerr << "Invalid edge after mapping: " << a << " " << b << endl;
            return 1;
        }
    }

    // Create a map for fast lookup of vertex order
    vector<int> order = degeneracy_ordering(graph);
    unordered_map<int, int> order_index;
    for (int i = 0; i < order.size(); i++) {
        order_index[order[i]] = i;
    }

    vector<set<int>> maximal_cliques;
    auto start = high_resolution_clock::now();

    for (int v : order) {
        set<int> R = {v}, P, X;
        for (int u : graph[v]) {
            if (order_index[u] > order_index[v]) {
                P.insert(u);
            } else {
                X.insert(u);
            }
        }
        BronKerbosch(R, P, X, graph, maximal_cliques);
    }

    auto end = high_resolution_clock::now();

    cout << "Maximal cliques found: " << maximal_cliques.size() << "\n";
    for (const auto &clique : maximal_cliques) {
        for (int node : clique) cout << node << " ";
        cout << "\n";
    }

    cout << "Time taken: " << duration_cast<milliseconds>(end - start).count() << " ms\n";
    return 0;
}