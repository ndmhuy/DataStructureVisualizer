#include "Utilities/PseudocodeManager.h"

const std::vector<std::string>& PseudocodeManager::getPseudocode(AlgorithmType type) {
    switch (type) {
        case AlgorithmType::None:
            return {"No algorithm selected."};
        case AlgorithmType::SinglyLinkedListInsert:
            return SinglyLinkedListInsertCode;
        case AlgorithmType::SinglyLinkedListDelete:
            return SinglyLinkedListDeleteCode;
        case AlgorithmType::SinglyLinkedListSearch:
            return SinglyLinkedListSearchCode;
        case AlgorithmType::GraphDAGShortestPath:
            return GraphDAGShortestPathCode;
        case AlgorithmType::GraphDijkstra:
            return GraphDijkstraCode;
        case AlgorithmType::GraphAStar:
            return GraphAStarCode;
        case AlgorithmType::GraphBellmanFord:
            return GraphBellmanFordCode;
        case AlgorithmType::GraphFloydWarshall:
            return GraphFloydWarshallCode;
        case AlgorithmType::GraphJohnson:
            return GraphJohnsonCode;
        default:
            return {"No pseudocode available for this algorithm."};
    }
}

const std::vector<std::string> PseudocodeManager::SinglyLinkedListInsertCode = {
    "function insert(value):",
    "    newNode = Node(value)",
    "    if head is null:",
    "        head = newNode",
    "        return",
    "    current = head",
    "    while current.next is not null:",
    "        current = current.next",
    "    current.next = newNode"
};

const std::vector<std::string> PseudocodeManager::SinglyLinkedListDeleteCode = {
    "function delete(value):",
    "    if head is null:",
    "        return",
    "    if head.value == value:",
    "        head = head.next",
    "        return",
    "    current = head",
    "    while current.next is not null:",
    "        if current.next.value == value:",
    "            deleteNode = current.next",
    "            current.next = current.next.next",
    "            delete deleteNode",
    "            return",
    "        current = current.next"
};

const std::vector<std::string> PseudocodeManager::SinglyLinkedListSearchCode = {
    "function search(value):",
    "    current = head",
    "    while current is not null:",
    "        if current.value == value:",
    "            return true",
    "        current = current.next",
    "    return false"
};

const std::vector<std::string> PseudocodeManager::GraphDAGShortestPathCode = {
};

const std::vector<std::string> PseudocodeManager::GraphDijkstraCode = {
};

const std::vector<std::string> PseudocodeManager::GraphAStarCode = {
};

const std::vector<std::string> PseudocodeManager::GraphBellmanFordCode = {
};

const std::vector<std::string> PseudocodeManager::GraphFloydWarshallCode = {
};

const std::vector<std::string> PseudocodeManager::GraphJohnsonCode = {
};