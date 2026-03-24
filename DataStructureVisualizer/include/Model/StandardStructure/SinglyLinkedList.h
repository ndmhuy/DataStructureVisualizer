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
    std::vector<int> toVector() const;
    void deleteNodes(Node*& head);

    public:
    SinglyLinkedList();
    ~SinglyLinkedList() override;

    SinglyLinkedList(const SinglyLinkedList&) = delete;
    SinglyLinkedList& operator=(const SinglyLinkedList&) = delete;

    void initialize(const std::vector<int>& data, Timeline& timeline) override;
    void insert(int value, Timeline& timeline) override;
    void remove(int value, Timeline& timeline) override;
    void search(int value, Timeline& timeline) override;
    void clear(Timeline& timeline) override;

    StructureType getStructureType() const override;
};

#endif // SINGLYLINKEDLIST_H