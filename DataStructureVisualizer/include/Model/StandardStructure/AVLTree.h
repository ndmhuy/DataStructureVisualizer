#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include "Model/StandardStructure/IStandardStructure.h"
#include "Model/Timeline.h"
#include "Model/Payloads/Payloads.h"
#include <vector>
#include <unordered_map>
#include <cstddef>

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
    // std::unordered_map<int, size_t> getNodeIndex; // value -> node id for highlighting

    // Builds a tree payload for rendering and highlighting.
    TreePayload makeTreePayload(const std::vector<size_t>& highlightedNodes = {}, const std::map<std::string, size_t>& pointers = {}, const std::vector<size_t>& successNodes = {});
    // Deletes a subtree recursively.
    void deleteNodes(Node*& root);

    // Performs a right rotation around the given node.
    Node* rotateRight(Node* root, Timeline* timeline, size_t id);
    // Performs a left rotation around the given node.
    Node* rotateLeft(Node* root, Timeline* timeline, size_t id);
    // Rebalances a subtree after mutation.
    void balance(Node*& root, Timeline* timeline, size_t id);

    // Beta
    // Performs the initial AVL insertion pass.
    bool initialInsert(int value, Node*& root, Timeline* timeline, size_t id);
    // Performs the initial AVL removal pass.
    bool initialRemove(int value, Node*& root, Timeline* timeline, size_t id);

    public:
    // Creates an empty AVL tree.
    AVLTree();
    // Destroys the tree and releases all nodes.
    ~AVLTree() override;

    // Creates an AVL tree with the selected layout configuration.
    AVLTree(const LayoutConfig& config = LayoutConfig());
    AVLTree& operator=(const AVLTree&) = delete;    

    // Initializes the tree from raw data.
    void initialize(const std::vector<int>& data, Timeline& timeline) override;
    // Inserts a value into the tree.
    void insert(int value, Timeline& timeline) override;
    // Removes a value from the tree.
    void remove(int value, Timeline& timeline) override;
    // Searches for a value in the tree.
    void search(int value, Timeline& timeline) override;
    // Updates one tree value to another.
    void update(int oldValue, int newValue, Timeline& timeline) override;
    // Clears the tree.
    void clear(Timeline& timeline) override;

    // Returns the structure type for UI routing.
    StructureType getStructureType() const override;
};

#endif