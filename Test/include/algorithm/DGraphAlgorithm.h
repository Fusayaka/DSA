#ifndef DGRAPH_ALGORITHM_H
#define DGRAPH_ALGORITHM_H

#include "graph/graphheader.h"

template <class T>
class Path {
public:
    T start;
    T end;
    double totalCost;
    DLinkedList<T> vertices;

    Path(T start, T end) : start(start), end(end), totalCost(std::numeric_limits<double>::infinity()) {
        vertices.add(start);
    }

    string toString() {
        stringstream os;
        
        typename DLinkedList<T>::Iterator verIt = vertices.begin();
        while (verIt != vertices.end()) {
            os << *verIt;
            ++verIt;

            if (verIt != vertices.end()) {
                os << " -> ";
            }
        }
        os << " | Total Cost: " << this->totalCost;
        return os.str();
    }
};

template <class T>
class DGraphAlgorithm {
public:
    DLinkedList<Path<T>*> dijkstra(DGraphModel<T>* graph, T startVertex) {
        if (!graph) {
            throw std::invalid_argument("Graph pointer is null.");
        }

        DLinkedList<Path<T>*> paths;
        std::unordered_map<T, Path<T>*> pathMap;

        // Initialize paths for all vertices
        DLinkedList<T> vertices = graph->vertices();
        for (auto vertex : vertices) {
            Path<T>* path = new Path<T>(startVertex, vertex);
            paths.add(path);
            pathMap[vertex] = path;
        }

        // Set start vertex path cost to 0
        pathMap[startVertex]->totalCost = 0;

        // Priority queue to get minimum cost vertex
        auto comparator = [&pathMap](const T& a, const T& b) {
            return pathMap[a]->totalCost > pathMap[b]->totalCost;
        };
        std::priority_queue<T, std::vector<T>, decltype(comparator)> pq(comparator);
        pq.push(startVertex);

        while (!pq.empty()) {
            T current = pq.top();
            pq.pop();

            // Get outgoing edges from current vertex
            DLinkedList<T> edges = graph->getOutwardEdges(current);
            for (auto neighbor : edges) {
                double edgeCost = graph->weight(current, neighbor);
                double newCost = pathMap[current]->totalCost + edgeCost;

                // Update path if new cost is less
                if (newCost < pathMap[neighbor]->totalCost) {
                    pathMap[neighbor]->totalCost = newCost;
                    pathMap[neighbor]->vertices.clear();
                    
                    // Copy current path and add new vertex
                    for (auto it = pathMap[current]->vertices.begin(); 
                         it != pathMap[current]->vertices.end(); ++it) {
                        pathMap[neighbor]->vertices.add(*it);
                    }
                    pathMap[neighbor]->vertices.add(neighbor);
                    pathMap[neighbor]->end = neighbor;

                    pq.push(neighbor);
                }
            }
        }

        return paths;
    }
};

#endif // DGRAPH_ALGORITHM_H