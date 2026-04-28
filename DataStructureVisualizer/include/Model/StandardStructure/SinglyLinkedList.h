#ifndef SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_H

#include "Model/StandardStructure/IStandardStructure.h"
#include "Model/Timeline.h"
#include <vector>

class SinglyLinkedList : public IStandardStructure {
    private:

    struct Node {
        int value;
        Node* next;
        Node(int val) : value(val), next(nullptr) {}
    };
    
    Node* head;
    // Converts the list to a vector snapshot.
    std::vector<int> toVector() const;
    // Deletes all nodes in the list.
    void deleteNodes(Node*& head);

    public:
    // Creates an empty singly linked list.
    SinglyLinkedList(const LayoutConfig& config = LayoutConfig());
    // Destroys the list and releases all nodes.
    ~SinglyLinkedList() override;

    SinglyLinkedList(const SinglyLinkedList&) = delete;
    SinglyLinkedList& operator=(const SinglyLinkedList&) = delete;

    // Initializes the list from raw data.
    void initialize(const std::vector<int>& data, Timeline& timeline) override;
    // Inserts a value into the list.
    void insert(int value, Timeline& timeline) override;
    // Removes a value from the list.
    void remove(int value, Timeline& timeline) override;
    // Searches for a value in the list.
    void search(int value, Timeline& timeline) override;
    // Updates one list value to another.
    void update(int value1, int value2, Timeline& timeline) override;
    // Clears the list.
    void clear(Timeline& timeline) override;

    // Returns the structure type for UI routing.
    StructureType getStructureType() const override;
};

#endif // SINGLYLINKEDLIST_H