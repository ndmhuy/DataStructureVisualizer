#ifndef SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_H

#include "Model/IDataStructure.h"
#include "Model/Timeline.h"

class SinglyLinkedList : public IDataStructure {
    private:

    struct Node {
        int value;
        Node* next;
        Node(int val) : value(val), next(nullptr) {}
    };
    
    Node* head;
    std::vector<int> toVector() const;

    public:
    SinglyLinkedList();
    ~SinglyLinkedList() override;

    void initialize(const std::vector<int>& data, Timeline& timeline) override;
    void insert(int value, Timeline& timeline) override;
    void remove(int value, Timeline& timeline) override;
    void search(int value, Timeline& timeline) override;
    void clear(Timeline& timeline) override;
};

#endif // SINGLYLINKEDLIST_H