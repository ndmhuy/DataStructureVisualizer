#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include "Model/IDataStructure.h"
#include "Model/Timeline.h"
#include <vector>

class AVLTree : public IDataStructure {
    private:
    struct Node {
        int value;
        Node* left = nullptr;
        Node* right = nullptr;
        int height;

        Node(int val) : value(val), left(nullptr), right(nullptr), height(1) {}

        int heightOf(Node* root);
        int heightLChild();
        int heightRChild();
        int balanceFactor();
    };
    
    Node* root;

    std::vector<int> toVector() const;
    Node* rotateRight(Node* root);
    Node* rotateLeft(Node* root);
    void balance(Node*& root);
    void deleteNodes(Node*& root);

    public:
    AVLTree();
    ~AVLTree() override;

    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;    

    void initialize(const std::vector<int>& data, Timeline& timeline) override;
    void insert(int value, Timeline& timeline) override;
    void remove(int value, Timeline& timeline) override;
    void search(int value, Timeline& timeline) override;
    void clear(Timeline& timeline) override;
};

#endif