#pragma once


#include <string>
#include <vector>
#include <list>
#include <unordered_map>



class Graph {
 public:
   /**
   * @brief Constructor 
   */
   Graph();


  /**
   * @brief Add new node
   *
   * @param nodeId Layer id
   * @param info Information about the layer
   */
  void addNode(const std::string& nodeId, const std::vector<std::string>& info);

  /**
   * @brief Add a edge 
   *
   * @param node1 Inicial node
   * @param node2 destination node
   */
   void addEdge(const std::string& node1, const std::vector<std::string>& node2);
   void addEdgeVector(const std::vector<std::string>& relation);

  /**
   * @brief Print the graph, each node with the edge.
   *
   */
   void printGraph() const;
  
  /**
   * @brief get the size of the graph
   *
   * @return size of the graph
   */
   int getsize();

//private:
   // Nodo con su lista de información
   std::unordered_map<std::string, std::vector<std::string>> nodes;
   std::vector<std::vector<std::string>> nodesVector;
   // Lista de adyacencia para las aristas
   std::unordered_map<std::string, std::list<std::vector<std::string>>> adjList;
   std::vector<std::vector<std::string>> adjVector;
};
