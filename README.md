# DAA Assignment - 1

## Group Members
- Priyansh Agarwal
- Harsh Jawajiwar (2021B5A72358H)
- Ansh Kalucha
- Sarthak Somani
- Ujjwal Mishra

---

## 1. Chiba Implementation

### Overview
The Chiba implementation is based on the Bron-Kerbosch algorithm, which is a well-known algorithm for finding all maximal cliques in an undirected graph. The algorithm uses a recursive backtracking approach to explore all possible cliques in the graph.

### Key Features
- **Graph Representation**: The graph is represented using an adjacency list, where each vertex maintains a set of its neighbors. This allows for efficient neighbor lookups.
- **Recursive Backtracking**: The algorithm uses a recursive function `expand` to explore all possible cliques. It maintains three sets:
  - **R**: The current clique being constructed.
  - **P**: The set of candidate vertices that can be added to the clique.
  - **X**: The set of vertices that have already been processed and should not be considered again.
- **Maximal Clique Detection**: A clique is maximal when both P and X are empty, meaning no more vertices can be added to the current clique.
- **Output**: The algorithm writes the maximal cliques to an output file and also maintains a histogram of clique sizes.

### Algorithm Steps
1. **Initialization**: Start with an empty clique R, all vertices in P, and an empty X.
2. **Recursive Expansion**:
   - For each vertex in P, add it to R and recursively explore its neighbors.
   - Update P and X by intersecting them with the neighbors of the current vertex.
   - If P and X are empty, the current R is a maximal clique.
3. **Backtracking**: After exploring a vertex, remove it from P and add it to X to avoid reprocessing.

### Performance Considerations
- **Time Complexity**: The worst-case time complexity of the Bron-Kerbosch algorithm is O(3^(n/3)), where n is the number of vertices. This is because the algorithm explores all possible cliques in the graph.
- **Space Complexity**: The space complexity is O(n + m), where n is the number of vertices and m is the number of edges, due to the adjacency list representation.
- **Optimization**: The algorithm does not use any pivot selection, which could improve performance by reducing the number of recursive calls.

### Output
The algorithm outputs the maximal cliques to a file, along with a histogram of clique sizes and the total number of maximal cliques. It also measures and outputs the time taken to find all cliques.

---

## 2. ELS Implementation

### Overview
The ELS implementation is also based on the Bron-Kerbosch algorithm, but it uses a pivot-based optimization known as the Tomita algorithm. This optimization reduces the number of recursive calls by selecting a pivot vertex with the highest degree, which helps prune the search space.

### Key Features
- **Graph Representation**: Similar to the Chiba implementation, the graph is represented using an adjacency list.
- **Pivot Selection**: The algorithm selects a pivot vertex with the maximum degree from the union of P and X. This pivot is used to reduce the number of recursive calls by skipping vertices that are neighbors of the pivot.
- **Recursive Backtracking**: The algorithm uses a recursive function `tomita` to explore cliques, maintaining the same sets R, P, and X as in the Chiba implementation.
- **Output**: The algorithm writes the maximal cliques to an output file and maintains a histogram of clique sizes.

### Algorithm Steps
1. **Initialization**: Start with an empty clique R, all vertices in P, and an empty X.
2. **Pivot Selection**: Select a pivot vertex with the highest degree from P and X.
3. **Recursive Expansion**:
   - For each vertex in P that is not a neighbor of the pivot, add it to R and recursively explore its neighbors.
   - Update P and X by intersecting them with the neighbors of the current vertex.
   - If P and X are empty, the current R is a maximal clique.
4. **Backtracking**: After exploring a vertex, remove it from P and add it to X.

### Performance Considerations
- **Time Complexity**: The pivot-based optimization reduces the number of recursive calls, but the worst-case time complexity remains O(3^(n/3)). However, in practice, the pivot selection can significantly improve performance.
- **Space Complexity**: The space complexity is O(n + m), similar to the Chiba implementation.
- **Optimization**: The pivot selection is the key optimization in this implementation, which helps prune the search space and reduce redundant computations.

### Output
The algorithm outputs the maximal cliques to a file, along with a histogram of clique sizes and the total number of maximal cliques. It also measures and outputs the time taken to find all cliques.

---

## 3. Worst-Case Implementation

### Overview
The Worst-Case implementation is another variant of the Bron-Kerbosch algorithm, but it is designed to handle the worst-case scenario where the graph is dense, and the number of maximal cliques is exponential. This implementation also uses the Tomita algorithm with pivot selection.

### Key Features
- **Graph Representation**: The graph is represented using an adjacency list, similar to the other implementations.
- **Pivot Selection**: The algorithm selects a pivot vertex with the highest degree from P and X, similar to the ELS implementation.
- **Recursive Backtracking**: The algorithm uses a recursive function `tomita` to explore cliques, maintaining the same sets R, P, and X.
- **Output**: The algorithm outputs the maximal cliques to a file, along with a histogram of clique sizes and the total number of maximal cliques.

### Algorithm Steps
1. **Initialization**: Start with an empty clique R, all vertices in P, and an empty X.
2. **Pivot Selection**: Select a pivot vertex with the highest degree from P and X.
3. **Recursive Expansion**:
   - For each vertex in P that is not a neighbor of the pivot, add it to R and recursively explore its neighbors.
   - Update P and X by intersecting them with the neighbors of the current vertex.
   - If P and X are empty, the current R is a maximal clique.
4. **Backtracking**: After exploring a vertex, remove it from P and add it to X.

### Performance Considerations
- **Time Complexity**: The worst-case time complexity is O(3^(n/3)), similar to the other implementations. However, the pivot selection helps reduce the number of recursive calls in practice.
- **Space Complexity**: The space complexity is O(n + m), due to the adjacency list representation.
- **Optimization**: The pivot selection is the key optimization, which helps prune the search space and reduce redundant computations.

### Output
The algorithm outputs the maximal cliques to a file, along with a histogram of clique sizes and the total number of maximal cliques. It also measures and outputs the time taken to find all cliques.

---

## Comparison of the Three Implementations

| Feature                  | Chiba Implementation       | ELS Implementation         | Worst-Case Implementation |
|--------------------------|---------------------------|---------------------------|---------------------------|
| Algorithm                | Bron-Kerbosch             | Tomita (Bron-Kerbosch + Pivot) | Tomita (Bron-Kerbosch + Pivot) |
| Pivot Selection          | No                        | Yes                       | Yes                       |
| Graph Representation     | Adjacency List            | Adjacency List            | Adjacency List            |
| Time Complexity          | O(3^(n/3))                | O(3^(n/3))                | O(3^(n/3))                |
| Space Complexity         | O(n + m)                  | O(n + m)                  | O(n + m)                  |
| Optimization             | None                      | Pivot Selection           | Pivot Selection           |
| Output                   | Cliques, Histogram, Time  | Cliques, Histogram, Time  | Cliques, Histogram, Time  |

---

## Theoretical Analysis

### Bron-Kerbosch Algorithm
- The Bron-Kerbosch algorithm is a classic algorithm for finding all maximal cliques in a graph. It uses a recursive backtracking approach to explore all possible cliques.
- The algorithm is efficient for sparse graphs but can be slow for dense graphs due to its exponential time complexity.

### Tomita Algorithm (Pivot-Based Optimization)
- The Tomita algorithm is an optimized version of the Bron-Kerbosch algorithm that uses pivot selection to reduce the number of recursive calls.
- The algorithm prunes the search space and avoids redundant computations by selecting a pivot vertex with the highest degree.
- This optimization is particularly useful for dense graphs, where the number of maximal cliques can be very large.

### Performance in Practice
- The Chiba implementation (without pivot selection) is simpler but may perform poorly on dense graphs.
- The ELS and Worst-Case implementations (with pivot selection) are more efficient in practice, especially for dense graphs, due to the pivot-based optimization.

### Output and Usability
- All three implementations provide detailed output, including the maximal cliques, a histogram of clique sizes, and the time taken to find the cliques.
- This makes them suitable for both theoretical analysis and practical applications.

---

## Conclusion
- The Chiba implementation is a straightforward implementation of the Bron-Kerbosch algorithm without any optimizations. It is suitable for small graphs or sparse graphs but may struggle with dense graphs.
- The ELS and Worst-Case implementations use the Tomita algorithm with pivot selection, making them more efficient for dense graphs. They are better suited for real-world applications where the graph may be large and dense.
- All three implementations provide detailed output, making them useful for both theoretical analysis and practical applications. However, the ELS and Worst-Case implementations are generally preferred due to their pivot-based optimizations.

---

## Results and Discussion

### Els-wiki
- **Clique Size Distribution**:
  - Size 2: 8655 cliques
  - Size 3: 13718 cliques
  - Size 4: 27292 cliques
  - Size 5: 48416 cliques
  - Size 6: 68872 cliques
  - Size 7: 83266 cliques
  - Size 8: 76732 cliques
  - Size 9: 54456 cliques
  - Size 10: 35470 cliques
  - Size 11: 21736 cliques
  - Size 12: 11640 cliques
  - Size 13: 5449 cliques
  - Size 14: 2329 cliques
  - Size 15: 740 cliques
  - Size 16: 208 cliques
  - Size 17: 23 cliques

![Histogram](/daa_assign_1/public/clique_histogram.png)

---

### Els-email
- **Number of maximal cliques**: 226859
- **Time taken**: 195089 ms
- **Clique Size Distribution**:
  - Size 2: 14070 cliques
  - Size 3: 7077 cliques
  - Size 4: 13319 cliques
  - Size 5: 18143 cliques
  - Size 6: 22715 cliques
  - Size 7: 25896 cliques
  - Size 8: 24766 cliques
  - Size 9: 22884 cliques
  - Size 10: 21393 cliques
  - Size 11: 17833 cliques
  - Size 12: 15181 cliques
  - Size 13: 11487 cliques
  - Size 14: 7417 cliques
  - Size 15: 3157 cliques
  - Size 16: 1178 cliques
  - Size 17: 286 cliques
  - Size 18: 41 cliques
  - Size 19: 10 cliques
  - Size 20: 6 cliques

![Histogram](/daa_assign_1/public/els_email_histogram.png)

---

### Chiba email
- **Clique Size Distribution (Histogram)**:
  - Size 2: 14070 cliques
  - Size 3: 7077 cliques
  - Size 4: 13319 cliques
  - Size 5: 18143 cliques
  - Size 6: 22715 cliques
  - Size 7: 25896 cliques
  - Size 8: 24766 cliques
  - Size 9: 22884 cliques
  - Size 10: 21393 cliques
  - Size 11: 17833 cliques
  - Size 12: 15181 cliques
  - Size 13: 11487 cliques
  - Size 14: 7417 cliques
  - Size 15: 3157 cliques
  - Size 16: 1178 cliques
  - Size 17: 286 cliques
  - Size 18: 41 cliques
  - Size 19: 10 cliques
  - Size 20: 6 cliques

- **Total Number of Maximal Cliques**: 226859
- **Time taken**: 512711 ms

![Histogram](/daa_assign_1/public/Chiba_email_histogram.png)

---

### Chiba-wiki
- **Clique Size Distribution (Histogram)**:
  - Size 2: 8655 cliques
  - Size 3: 13718 cliques
  - Size 4: 27292 cliques
  - Size 5: 48416 cliques
  - Size 6: 68872 cliques
  - Size 7: 83266 cliques
  - Size 8: 76732 cliques
  - Size 9: 54456 cliques
  - Size 10: 35470 cliques
  - Size 11: 21736 cliques
  - Size 12: 11640 cliques
  - Size 13: 5449 cliques
  - Size 14: 2329 cliques
  - Size 15: 740 cliques
  - Size 16: 208 cliques
  - Size 17: 23 cliques

- **Total Number of Maximal Cliques**: 459002
- **Time taken**: 156874 ms

![Histogram](/daa_assign_1/public/Chiba_wiki_histogram.png)

---

### Worst case - email
- **Number of maximal cliques**: 226859
- **Time taken**: 113507 ms
- **Clique Size Distribution**:
  - Size 2: 14070 cliques
  - Size 3: 7077 cliques
  - Size 4: 13319 cliques
  - Size 5: 18143 cliques
  - Size 6: 22715 cliques
  - Size 7: 25896 cliques
  - Size 8: 24766 cliques
  - Size 9: 22884 cliques
  - Size 10: 21393 cliques
  - Size 11: 17833 cliques
  - Size 12: 15181 cliques
  - Size 13: 11487 cliques
  - Size 14: 7417 cliques
  - Size 15: 3157 cliques
  - Size 16: 1178 cliques
  - Size 17: 286 cliques
  - Size 18: 41 cliques
  - Size 19: 10 cliques
  - Size 20: 6 cliques

![Histogram](/daa_assign_1/public/Worst_case_email_histogram.png)

---

### Worst case - wiki
- **Number of maximal cliques**: 459002
- **Time taken**: 12691 ms
- **Clique Size Distribution**:
  - Size 2: 8655 cliques
  - Size 3: 13718 cliques
  - Size 4: 27292 cliques
  - Size 5: 48416 cliques
  - Size 6: 68872 cliques
  - Size 7: 83266 cliques
  - Size 8: 76732 cliques
  - Size 9: 54456 cliques
  - Size 10: 35470 cliques
  - Size 11: 21736 cliques
  - Size 12: 11640 cliques
  - Size 13: 5449 cliques
  - Size 14: 2329 cliques
  - Size 15: 740 cliques
  - Size 16: 208 cliques
  - Size 17: 23 cliques

![Histogram](/daa_assign_1/public/Worst_case_wiki_histogram.png)
