#ifndef UGRAPH_ALGORITHM_H
#define UGRAPH_ALGORITHM_H

#include "graph/graphheader.h"

template <class T>
class UGraphAlgorithm {
public:
    /**
     * Computes the Minimum Spanning Tree (MST) of the given undirected graph
     * using Prim's Algorithm.
     *
     * @param graph: Pointer to the input UGraphModel.
     * @return A new UGraphModel representing the MST.
     */
    UGraphModel<T> minSpanningTree(UGraphModel<T>* graph) {
        if (!graph) {
            throw std::invalid_argument("Graph pointer is null.");
        }

        // Step 1: Initialize MST and visited set
        UGraphModel<T> mst(graph->getVertexEQ(), graph->getVertex2Str());
        DLinkedList<T> vertices = graph->vertices();

        for (T vertex : vertices) {
            mst.add(vertex);
        }

        // Step 2: Priority queue to select the minimum edge
        using EdgeInfo = std::tuple<float, T, T>;  // (weight, from, to)
        std::priority_queue<EdgeInfo, std::vector<EdgeInfo>, std::greater<>> pq;

        std::unordered_map<T, bool> visited;  // Track visited vertices
        T startVertex = vertices.get(0);          // Start from the first vertex
        visited[startVertex] = true;

        // Push all edges of the starting vertex into the priority queue
        DLinkedList<T> neighbors = graph->getOutwardEdges(startVertex);
        for (T neighbor : neighbors) {
            float weight = graph->weight(startVertex, neighbor);
            pq.emplace(weight, startVertex, neighbor);
        }

        // Step 3: Expand the MST
        while (!pq.empty()) {
            auto [weight, from, to] = pq.top();
            pq.pop();

            if (visited[to]) {
                continue;  // Skip already visited vertices
            }

            // Add the edge to the MST
            mst.connect(from, to, weight);
            visited[to] = true;

            // Push all edges of the newly added vertex into the priority queue
            neighbors = graph->getOutwardEdges(to);
            for (T neighbor : neighbors) {
                if (!visited[neighbor]) {
                    float edgeWeight = graph->weight(to, neighbor);
                    pq.emplace(edgeWeight, to, neighbor);
                }
            }
        }

        return mst;
    }
};

#endif /* UGRAPH_ALGORITHM_H */
