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
    TWoThreeFourTree,

    // Heaps
    MinHeap,
    MaxHeap,

    // Graphs
    AdjacencyList,
    AdjacencyMatrix,

    None // Default
};

#endif // STRUCTURETYPE_H