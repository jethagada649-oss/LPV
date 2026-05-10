// to compile use : g++ -fopenmp BFS.cpp
// to run use : ./a.out
#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int vertices) {
        V = vertices;
        adj.resize(V);
    }

    // Add edge to undirected graph
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Parallel BFS using OpenMP
    void parallelBFS(int start) {

        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "Parallel BFS Traversal: ";

        while (!q.empty()) {

            int size = q.size();

            // Store current level nodes
            vector<int> currentLevel;

            for (int i = 0; i < size; i++) {
                int node = q.front();
                q.pop();

                cout << node << " ";

                currentLevel.push_back(node);
            }

            // Parallel processing of neighbors
            #pragma omp parallel for
            for (int i = 0; i < currentLevel.size(); i++) {

                int node = currentLevel[i];

                for (int neighbor : adj[node]) {

                    if (!visited[neighbor]) {

                        #pragma omp critical
                        {
                            if (!visited[neighbor]) {
                                visited[neighbor] = true;
                                q.push(neighbor);
                            }
                        }
                    }
                }
            }
        }

        cout << endl;
    }
};

int main() {

    int vertices = 7;

    Graph g(vertices);

    // Creating undirected graph
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);

    cout << "Starting BFS from node 0\n";

    double start_time = omp_get_wtime();

    g.parallelBFS(0);

    double end_time = omp_get_wtime();

    cout << "Execution Time: "
         << end_time - start_time
         << " seconds\n";

    return 0;
}
