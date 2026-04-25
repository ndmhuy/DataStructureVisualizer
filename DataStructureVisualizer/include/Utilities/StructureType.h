#ifndef STRUCTURETYPE_H
#define STRUCTURETYPE_H

enum class StructureType {
    // Linear
    SinglyLinkedList,
    DoublyLinkedList,
    CircularLinkedList,
    
    // Trees
    BinarySearchTree,
    AVLTree,
    RedBlackTree,
    TwoThreeTree,
    TwoThreeFourTree,

    // Heaps
    MinHeap,
    MaxHeap,

    // Graphs
    AdjacencyList,
    AdjacencyMatrix,
    GridGraph,

    None // Default
};

#endif // STRUCTURETYPE_H