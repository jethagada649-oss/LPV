// to compile use : g++ -fopenmp DFS.cpp
// to run use : ./a.out
#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;
    vector<bool> visited;

public:

    Graph(int vertices) {
        V = vertices;
        adj.resize(V);
        visited.resize(V, false);
    }

    // Add edge to undirected graph
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Parallel DFS
    void parallelDFS(int node) {

        bool alreadyVisited = false;

        // Critical section for safe access
        #pragma omp critical
        {
            if (visited[node]) {
                alreadyVisited = true;
            } else {
                visited[node] = true;
                cout << node << " ";
            }
        }

        // Return OUTSIDE critical section
        if (alreadyVisited)
            return;

        // Create parallel tasks for neighbors
        for (int neighbor : adj[node]) {

            #pragma omp task
            parallelDFS(neighbor);
        }
    }

    void startDFS(int startNode) {

        cout << "Parallel DFS Traversal: ";

        #pragma omp parallel
        {
            #pragma omp single
            {
                parallelDFS(startNode);
            }
        }

        cout << endl;
    }
};

int main() {

    Graph g(7);

    // Undirected graph
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);

    double start = omp_get_wtime();

    g.startDFS(0);

    double end = omp_get_wtime();

    cout << "Execution Time: "
         << end - start
         << " seconds\n";

    return 0;
}
