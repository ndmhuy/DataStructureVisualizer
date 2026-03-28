#include "Utilities/PseudocodeManager.h"

const std::vector<std::string>& PseudocodeManager::getPseudocode(AlgorithmType type) {
    switch (type) {
        case AlgorithmType::None:
            return {"No algorithm selected."};
        case AlgorithmType::SinglyLinkedList_Insert:
            return SinglyLinkedListInsertCode;
        case AlgorithmType::SinglyLinkedList_Delete:
            return SinglyLinkedListDeleteCode;
        case AlgorithmType::SinglyLinkedList_Search:
            return SinglyLinkedListSearchCode;
        case AlgorithmType::Graph_DAGShortestPath:
            return DAGShortestPathCode;
        case AlgorithmType::Graph_Dijkstra:
            return DijkstraCode;
        case AlgorithmType::Graph_AStar:
            return AStarCode;
        case AlgorithmType::Graph_BellmanFord:
            return BellmanFordCode;
        case AlgorithmType::Graph_FloydWarshall:
            return FloydWarshallCode;
        case AlgorithmType::Graph_Johnson:
            return JohnsonCode;
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

const std::vector<std::string> PseudocodeManager::DAGShortestPathCode = {
};

const std::vector<std::string> PseudocodeManager::DijkstraCode = {
};

const std::vector<std::string> PseudocodeManager::AStarCode = {
};

const std::vector<std::string> PseudocodeManager::BellmanFordCode = {
};

const std::vector<std::string> PseudocodeManager::FloydWarshallCode = {
};

const std::vector<std::string> PseudocodeManager::JohnsonCode = {
};