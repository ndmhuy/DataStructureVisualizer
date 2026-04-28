#ifndef PSEUDOCODEMANAGER_H
#define PSEUDOCODEMANAGER_H

#include <string>
#include <vector>

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
  // Pseudocode for singly linked list insertion.
  AVLTreeSearch,
  // Pseudocode for singly linked list removal.
  AVLTreeUpdate,
  // Pseudocode for singly linked list search.

  // Pseudocode for singly linked list update.
  // Graph algorithms
  // Grid graph algorithms
  GraphGridBFSShortestPath,
  // Pseudocode for heap insertion.
  GraphGridAStar,
  // Pseudocode for heap removal.
  // General graph algorithms
  // Pseudocode for heap search.
  GraphDAGShortestPath,
  // Pseudocode for heap update.
  GraphDijkstra,
  GraphAStar,
  GraphBellmanFord,
  // Pseudocode for AVL tree insertion.
  GraphFloydWarshall,
  // Pseudocode for AVL tree removal.
  GraphJohnson,
  // Pseudocode for AVL tree search.
};
  // Pseudocode for AVL tree update.

class PseudocodeManager {
private:
  // Singly Linked List
  // Pseudocode for grid BFS shortest path.
  static const std::vector<std::string> SinglyLinkedListInsertCode;
  // Pseudocode for grid A*.
  static const std::vector<std::string> SinglyLinkedListRemoveCode;
  static const std::vector<std::string> SinglyLinkedListSearchCode;
  // Pseudocode for DAG shortest path.
  static const std::vector<std::string> SinglyLinkedListUpdateCode;
  // Pseudocode for Dijkstra's algorithm.

  // Pseudocode for A* on graphs.
  // Heap
  // Pseudocode for Bellman-Ford.
  static const std::vector<std::string> HeapInsertCode;
  // Pseudocode for Floyd-Warshall.
  static const std::vector<std::string> HeapRemoveCode;
  // Pseudocode for Johnson's algorithm.
  static const std::vector<std::string> HeapSearchCode;
  static const std::vector<std::string> HeapUpdateCode;

  // Returns the pseudocode lines for the selected algorithm.
  // AVL Tree
  static const std::vector<std::string> AVLTreeInsertCode;
  static const std::vector<std::string> AVLTreeRemoveCode;
  static const std::vector<std::string> AVLTreeSearchCode;
  static const std::vector<std::string> AVLTreeUpdateCode;

  // Graph
  // Grid graph algorithms
  static const std::vector<std::string> GraphGridBFSShortestPathCode;
  static const std::vector<std::string> GraphGridAStarCode;
  // General graph algorithms
  static const std::vector<std::string> GraphDAGShortestPathCode;
  static const std::vector<std::string> GraphDijkstraCode;
  static const std::vector<std::string> GraphAStarCode;
  static const std::vector<std::string> GraphBellmanFordCode;
  static const std::vector<std::string> GraphFloydWarshallCode;
  static const std::vector<std::string> GraphJohnsonCode;

public:
  static const std::vector<std::string> &getPseudocode(AlgorithmType type);
};

#endif // PSEUDOCODEMANAGER_H