### Johnson’s Algorithm 概述

**Johnson’s Algorithm** 是一种用于解决**所有节点对最短路径问题**的算法，尤其适用于带有**负权边**的有向图。它结合了 **Bellman-Ford** 和 **Dijkstra** 算法的优点来有效处理负权边，并能在没有负权环的情况下快速计算出最短路径。

Johnson算法的核心思想是通过 **重新标号（reweighting）** 图中的边，使得所有的边都具有非负权重，然后使用Dijkstra算法来求解所有节点对之间的最短路径。

### Johnson算法的步骤

1. **增加一个虚拟节点**：
   - 向图中增加一个新的虚拟节点 \(q\)，并将其与图中所有节点连边，边的权重为0。
   
2. **使用Bellman-Ford算法**：
   - 对扩展后的图（包含虚拟节点）运行Bellman-Ford算法，计算虚拟节点 \(q\) 到图中其他所有节点的最短路径，并用这些最短路径作为每个节点的新的“势能函数”值 \(h(v)\)。
   - 如果在运行Bellman-Ford时发现负权环，则算法终止并报告图中有负权环。

3. **重新标号边的权重**：
   - 根据 **势能函数** \(h(v)\) 对图中的边进行重新标号，使得每条边的权重变为：
$$
     w'(u, v) = w(u, v) + h(u) - h(v)
$$
   - 经过这种重新标号后，所有边的权重都将变为非负数，同时最短路径的相对顺序保持不变。

4. **使用Dijkstra算法**：
   - 对重新标号后的图中的每个节点使用Dijkstra算法，计算出该节点到所有其他节点的最短路径。

5. **恢复原始最短路径权重**：
   - 最后，将通过Dijkstra算法计算的最短路径结果还原为原图的最短路径权重：
$$
     \delta(u, v) = d'(u, v) + h(v) - h(u)
$$
   - 其中 \(d'(u, v)\) 是在重新标号后的图上使用Dijkstra算法得到的最短路径长度。

### Johnson算法的时间复杂度

- **Bellman-Ford算法** 的时间复杂度为 \(O(V ⋅ E)\)，其中 \(V\) 是顶点数，\(E\) 是边的数量。
- **Dijkstra算法** 使用堆的优先队列，其时间复杂度为 \(O((V + E) log V)\)，而Johnson算法需要对每个节点都运行一次Dijkstra算法，因此其复杂度为 \(O(V⋅ (V + E) log V)\)。

Johnson算法的总时间复杂度为：
\[
O(V \cdot E + V \cdot (V + E) \log V) \approx O(V^2 \log V + V \cdot E)
\]

这使得它在稀疏图中比 **Floyd-Warshall算法** 更有效，因为后者的复杂度为 \(O(V^3)\)。

### Johnson算法的优缺点

**优点**：
- **适用于负权边**：可以处理带有负权边的图，但不能处理负权环。
- **在稀疏图中效率高**：对于边远多于顶点的稀疏图，Johnson算法的复杂度优于Floyd-Warshall算法。
- **结合了Bellman-Ford和Dijkstra的优势**：在处理负权边时比单独使用Bellman-Ford更高效。

**缺点**：
- **需要重新标号**：重新标号边的权重是一个相对复杂的过程，尽管不会影响最短路径的计算，但增加了实现的复杂性。
- **无法处理负权环**：如果图中存在负权环，Bellman-Ford会检测到并终止，但不能解决这种问题。

### Johnson算法的应用场景

- **带负权边的图**：Johnson算法特别适用于带有负权边但没有负权环的图。
- **稀疏图中的最短路径问题**：在稀疏图中，Johnson算法可以比Floyd-Warshall算法更高效地解决所有节点对最短路径问题。

### C语言实现

以下是Johnson算法的C语言实现，它结合了Bellman-Ford和Dijkstra算法来解决所有节点对最短路径问题。

```c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF INT_MAX

typedef struct Edge {
    int u, v, weight;
} Edge;

typedef struct Graph {
    int V, E;
    Edge* edges;
} Graph;

// Bellman-Ford算法
int bellmanFord(Graph* graph, int* h, int src) {
    int V = graph->V;
    int E = graph->E;
    for (int i = 0; i < V; i++) {
        h[i] = INF;
    }
    h[src] = 0;

    for (int i = 1; i <= V - 1; i++) {
        for (int j = 0; j < E; j++) {
            int u = graph->edges[j].u;
            int v = graph->edges[j].v;
            int weight = graph->edges[j].weight;
            if (h[u] != INF && h[u] + weight < h[v]) {
                h[v] = h[u] + weight;
            }
        }
    }

    // 检测负权环
    for (int i = 0; i < E; i++) {
        int u = graph->edges[i].u;
        int v = graph->edges[i].v;
        int weight = graph->edges[i].weight;
        if (h[u] != INF && h[u] + weight < h[v]) {
            return 1; // 存在负权环
        }
    }

    return 0; // 无负权环
}

// Dijkstra算法
void dijkstra(int** adjMatrix, int V, int src, int* dist) {
    int* visited = (int*)malloc(V * sizeof(int));
    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[src] = 0;

    for (int i = 0; i < V; i++) {
        int u = -1;
        for (int j = 0; j < V; j++) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }
        if (dist[u] == INF) {
            break;
        }

        visited[u] = 1;

        for (int v = 0; v < V; v++) {
            if (adjMatrix[u][v] && dist[u] + adjMatrix[u][v] < dist[v]) {
                dist[v] = dist[u] + adjMatrix[u][v];
            }
        }
    }

    free(visited);
}

// Johnson算法
void johnson(Graph* graph) {
    int V = graph->V;
    int E = graph->E;
    int* h = (int*)malloc(V * sizeof(int));

    // 新增一个虚拟节点
    Graph newGraph;
    newGraph.V = V + 1;
    newGraph.E = E + V;
    newGraph.edges = (Edge*)malloc((E + V) * sizeof(Edge));
    for (int i = 0; i < E; i++) {
        newGraph.edges[i] = graph->edges[i];
    }
    for (int i = 0; i < V; i++) {
        newGraph.edges[E + i].u = V;
        newGraph.edges[E + i].v = i;
        newGraph.edges[E + i].weight = 0;
    }

    // Step 1: 使用Bellman-Ford算法
    if (bellmanFord(&newGraph, h, V)) {
        printf("图中存在负权环\n");
        return;
    }

    // Step 2: 重新标号边权重
    for (int i = 0; i < E; i++) {
        graph->edges[i].weight += h[graph->edges[i].u] - h[graph->edges[i].v];
    }

    // Step 3: 使用Dijkstra算法
    int** adjMatrix = (int**)malloc(V * sizeof(int*));
    for (int i = 0; i < V; i++) {
        adjMatrix[i] = (int*)malloc(V * sizeof(int));
        for (int j = 0; j < V; j++) {
            adjMatrix[i][j] = INF;
        }
    }
    for (int i = 0; i < E; i++) {
        adjMatrix[graph->edges[i].u][graph->edges[i].v] = graph->edges[i].weight;
    }

    for (int u = 0; u < V; u++) {
        int* dist = (int*)malloc

(V * sizeof(int));
        dijkstra(adjMatrix, V, u, dist);

        for (int v = 0; v < V; v++) {
            if (dist[v] != INF) {
                dist[v] += h[v] - h[u]; // 还原原图的最短路径
            }
            printf("从节点 %d 到节点 %d 的最短路径长度为 %d\n", u, v, dist[v]);
        }

        free(dist);
    }

    free(h);
    for (int i = 0; i < V; i++) {
        free(adjMatrix[i]);
    }
    free(adjMatrix);
    free(newGraph.edges);
}

int main() {
    int V = 5;
    int E = 10;
    Graph graph;
    graph.V = V;
    graph.E = E;
    graph.edges = (Edge*)malloc(E * sizeof(Edge));

    // 初始化图的边
    graph.edges[0] = (Edge){0, 1, -1};
    graph.edges[1] = (Edge){0, 2, 4};
    graph.edges[2] = (Edge){1, 2, 3};
    graph.edges[3] = (Edge){1, 3, 2};
    graph.edges[4] = (Edge){1, 4, 2};
    graph.edges[5] = (Edge){3, 2, 5};
    graph.edges[6] = (Edge){3, 1, 1};
    graph.edges[7] = (Edge){4, 3, -3};

    // 运行Johnson算法
    johnson(&graph);

    free(graph.edges);
    return 0;
}
```
