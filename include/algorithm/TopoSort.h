#ifndef TOPOSORTER_H
#define TOPOSORTER_H

#include "graph/graphheader.h"

template <class T>
class TopoSorter {
public:
    enum Mode {
        BFS,
        DFS
    };

private:
    DGraphModel<T> *graph;

public:
    TopoSorter(DGraphModel<T> *graph) : graph(graph) {}

    // Topological sort function
    DLinkedList<T> sort(Mode mode = BFS)
    {
        if (mode == BFS) {
            return bfsTopologicalSort();
        }
        else {
            return dfsTopologicalSort();
        }
    }

private:
    // BFS-based topological sort (Kahn's Algorithm)
    DLinkedList<T> bfsTopologicalSort() {
        unordered_map<T, int> inDegrees;
        queue<T> zeroInDegreeQueue;
        DLinkedList<T> topoOrder;

        // Initialize in-degree counts
        for (T vertex : graph->vertices()) {
            inDegrees[vertex] = graph->inDegree(vertex);
            if (inDegrees[vertex] == 0) {
                zeroInDegreeQueue.push(vertex);
            }
        }

        // Process vertices with zero in-degree
        while (!zeroInDegreeQueue.empty()) {
            T current = zeroInDegreeQueue.front();
            zeroInDegreeQueue.pop();
            topoOrder.add(current);

            // Decrease in-degree for outward edges
            for (T neighbor : graph->getOutwardEdges(current)) {
                inDegrees[neighbor]--;
                if (inDegrees[neighbor] == 0) {
                    zeroInDegreeQueue.push(neighbor);
                }
            }
        }

        // Check for cycles
        if (topoOrder.size() != graph->size()) {
            throw std::runtime_error("Graph has cycles, topological sort is not possible.");
        }

        return topoOrder;
    }

    // DFS-based topological sort
    DLinkedList<T> dfsTopologicalSort() {
        std::unordered_map<T, bool> visited;
        std::stack<T> stack;
        DLinkedList<T> topoOrder;

        // Visit all vertices
        for (T vertex : graph->vertices()) {
            if (!visited[vertex]) {
                dfsVisit(vertex, visited, stack);
            }
        }

        // Collect the result from the stack
        while (!stack.empty()) {
            topoOrder.add(stack.top());
            stack.pop();
        }

        return topoOrder;
    }

    // Recursive DFS visit function
    void dfsVisit(T vertex, std::unordered_map<T, bool> &visited, std::stack<T> &stack) {
        visited[vertex] = true;

        // Visit neighbors
        for (T neighbor : graph->getOutwardEdges(vertex)) {
            if (!visited[neighbor]) {
                dfsVisit(neighbor, visited, stack);
            }
        }

        // Add to stack after all neighbors are processed
        stack.push(vertex);
    }
};

#endif /* TOPOSORTER_H */
