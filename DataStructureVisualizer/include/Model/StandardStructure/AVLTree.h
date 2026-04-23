#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include "Model/StandardStructure/IStandardStructure.h"
#include "Model/Timeline.h"
#include "Model/Payloads/Payloads.h"
#include <vector>
#include <unordered_map>

class AVLTree : public IStandardStructure {
    private:

    struct Node {
        int value;
        Node* left;
        Node* right;
        int height;
        Node(int val) : value(val), left(nullptr), right(nullptr), height(1) {}

        static int heightOf(Node* root);
        int heightLChild();
        int heightRChild();
        int balanceFactor();
    };
    
    Node* root;
    std::unordered_map<int, size_t> getNodeIndex; // value -> node id for highlighting

    TreePayload makeTreePayload(const std::vector<size_t>& highlightedNodes = {});
    void deleteNodes(Node*& root);

    Node* rotateRight(Node* root, Timeline& timeline);
    Node* rotateLeft(Node* root, Timeline& timeline);
    void balance(Node*& root, Timeline& timeline);

    // Beta
    bool initialInsert(int value, Node*& root, Timeline& timeline);
    bool initialRemove(int value, Node*& root, Timeline& timeline);

    public:
    AVLTree();
    ~AVLTree() override;

    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;    

    void initialize(const std::vector<int>& data, Timeline& timeline) override;
    void insert(int value, Timeline& timeline) override;
    void remove(int value, Timeline& timeline) override;
    void search(int value, Timeline& timeline) override;
    void update(int oldValue, int newValue, Timeline& timeline) override;
    void clear(Timeline& timeline) override;

    StructureType getStructureType() const override;
};

#endif