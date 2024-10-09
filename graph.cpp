#pragma once


#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include "graph.hpp"

Graph::Graph(){}

// Añadir un nodo con su lista de información
void Graph::addNode(const std::string& nodeId, const std::vector<std::string>& info) {
    nodes[nodeId] = info; // tener cuidado porque si hay un nodo de igual nombre lo va a reemplazar
    nodesVector.push_back(info);
}

// Añadir una arista dirigida desde node1 hacia node2
void Graph::addEdge(const std::string& node1, const std::vector<std::string>& node2) {
    adjList[node1].push_back(node2);
}

void Graph::addEdgeVector(const std::vector<std::string>& relation) {
    adjVector.push_back(relation);
}

// Imprimir el grafo
void Graph::printGraph() const {
    for (const auto& pair : nodes) {
        std::cout << pair.second[1];// << ": ";
        /*for (const auto& info : pair.second) {
            std::cout << info << " ";
        }*/
        //std::cout << "\nEdges: ";
        
        if (adjList.find(pair.first) != adjList.end()) {
            for (const auto& neighbor : adjList.at(pair.second[1])) {
                std::cout << " --------> "<< neighbor[1];
            }
        }
        std::cout << std::endl;
    }
}

int Graph::getsize(){
    return nodes.size();
}