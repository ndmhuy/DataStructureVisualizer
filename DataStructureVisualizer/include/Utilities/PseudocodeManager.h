#ifndef PSEUDOCODEMANAGER_H
#define PSEUDOCODEMANAGER_H

#include <vector>
#include <string>

enum class AlgorithmType {
    None,
    // Singly linked list algorithms
    SinglyLinkedListInsert,
    SinglyLinkedListRemove,
    SinglyLinkedListSearch,
    SinglyLinkedListUpdate,

    // Heap
    HeapInsert,
    HeapRemove,
    HeapSearch,
    HeapUpdate,

    // AVL Tree
    AVLTreeInsert,
    AVLTreeRemove,
    AVLTreeSearch,
    AVLTreeUpdate,

    // Graph algorithms
    GraphGridBFSShortestPath,
    GraphDAGShortestPath,
    GraphDijkstra,
    GraphAStar,
    GraphBellmanFord,
    GraphFloydWarshall,
    GraphJohnson,
};

class PseudocodeManager {
    private:
    // Singly Linked List
    static const std::vector<std::string> SinglyLinkedListInsertCode;
    static const std::vector<std::string> SinglyLinkedListRemoveCode;
    static const std::vector<std::string> SinglyLinkedListSearchCode;
    static const std::vector<std::string> SinglyLinkedListUpdateCode;

    // Heap
    static const std::vector<std::string> HeapInsertCode;
    static const std::vector<std::string> HeapRemoveCode;
    static const std::vector<std::string> HeapSearchCode;
    static const std::vector<std::string> HeapUpdateCode;

    // AVL Tree
    static const std::vector<std::string> AVLTreeInsertCode;
    static const std::vector<std::string> AVLTreeRemoveCode;
    static const std::vector<std::string> AVLTreeSearchCode;
    static const std::vector<std::string> AVLTreeUpdateCode;

    // Graph
    static const std::vector<std::string> GraphGridBFSShortestPathCode;
    static const std::vector<std::string> GraphDAGShortestPathCode;
    static const std::vector<std::string> GraphDijkstraCode;
    static const std::vector<std::string> GraphAStarCode;
    static const std::vector<std::string> GraphBellmanFordCode;
    static const std::vector<std::string> GraphFloydWarshallCode;
    static const std::vector<std::string> GraphJohnsonCode;
    
    public:
    static const std::vector<std::string>& getPseudocode(AlgorithmType type);
};

#endif // PSEUDOCODEMANAGER_H