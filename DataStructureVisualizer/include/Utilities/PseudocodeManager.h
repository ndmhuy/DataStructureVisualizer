#ifndef PSEUDOCODEMANAGER_H
#define PSEUDOCODEMANAGER_H

#include <string>
#include <vector>

enum class AlgorithmType {
  None,
  // Singly linked list algorithms
  SinglyLinkedListInsert,
  SinglyLinkedListDelete,
  SinglyLinkedListSearch,
  SinglyLinkedListUpdate,

  // Heap

  // AVL Tree

  // Graph algorithms
  // Grid graph algorithms
  GraphGridBFSShortestPath,
  GraphGridAStar,
  // General graph algorithms
  GraphDAGShortestPath,
  GraphDijkstra,
  GraphAStar,
  GraphBellmanFord,
  GraphFloydWarshall,
  GraphJohnson,
};

class PseudocodeManager {
private:
  static const std::vector<std::string> SinglyLinkedListInsertCode;
  static const std::vector<std::string> SinglyLinkedListDeleteCode;
  static const std::vector<std::string> SinglyLinkedListSearchCode;
  static const std::vector<std::string> SinglyLinkedListUpdateCode;

  // Graph algorithms
  static const std::vector<std::string> GraphGridBFSShortestPathCode;
  static const std::vector<std::string> GraphGridAStarCode;
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