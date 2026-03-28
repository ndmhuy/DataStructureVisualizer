#ifndef PSEUDOCODEMANAGER_H
#define PSEUDOCODEMANAGER_H

#include <vector>
#include <string>

enum class AlgorithmType {
    None,
    // Singly linked list algorithms
    SinglyLinkedList_Insert,
    SinglyLinkedList_Delete,
    SinglyLinkedList_Search,

    // Heap

    // AVL Tree

    // Graph algorithms
    Graph_DAGShortestPath,
    Graph_Dijkstra,
    Graph_AStar,
    Graph_BellmanFord,
    Graph_FloydWarshall,
    Graph_Johnson,
};

class PseudocodeManager {
    private:
    static const std::vector<std::string> SinglyLinkedListInsertCode;
    static const std::vector<std::string> SinglyLinkedListDeleteCode;
    static const std::vector<std::string> SinglyLinkedListSearchCode;
    static const std::vector<std::string> DAGShortestPathCode;
    static const std::vector<std::string> DijkstraCode;
    static const std::vector<std::string> AStarCode;
    static const std::vector<std::string> BellmanFordCode;
    static const std::vector<std::string> FloydWarshallCode;
    static const std::vector<std::string> JohnsonCode;
    
    public:
    static std::vector<std::string> getPseudocode(AlgorithmType type);
};

#endif // PSEUDOCODEMANAGER_H