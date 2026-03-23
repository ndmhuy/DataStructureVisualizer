#include "Model/DataStructure/AVLTree.h"
#include <string>
#include <queue>
#include <algorithm>

int AVLTree::Node::heightOf(Node* root) {
    return root ? root->height : 0;
}

int AVLTree::Node::heightLChild() {
    return heightOf(left);
}

int AVLTree::Node::heightRChild() {
    return heightOf(right);
}

int AVLTree::Node::balanceFactor() {
    return heightLChild() - heightRChild();
}

std::vector<int> AVLTree::toVector() const {
    std::vector<int> result;
    // BFS
    std::queue<Node*> que;
    que.push(root);
    
    while (!que.empty()) {
        Node* current = que.front();
        que.pop();

        if (current->left) 
            que.push(current->left);
        if (current->right) 
            que.push(current->right);

        result.push_back(current->value);
    }
    return result;
}

void AVLTree::deleteNodes(Node*& root) {
    std::queue<Node*> que;
    que.push(root);
    
    while (!que.empty()) {
        Node* current = que.front();
        que.pop();

        if (current->left) 
            que.push(current->left);
        if (current->right) 
            que.push(current->right);

        delete current;
    }
}

AVLTree::Node* AVLTree::rotateRight(Node* root) {
    Node* curr = root->left;

    root->left = curr->right;
    curr->right = root;

    root->height = 1 + std::max(Node::heightOf(root->left), Node::heightOf(root->right));
    curr->height = 1 + std::max(Node::heightOf(curr->left), Node::heightOf(curr->right));

    return curr;
}

AVLTree::Node* AVLTree::rotateLeft(Node* root) {
    Node* curr = root->right;

    root->right = curr->left;
    curr->left = root;

    root->height = 1 + std::max(Node::heightOf(root->left), Node::heightOf(root->right));
    curr->height = 1 + std::max(Node::heightOf(curr->left), Node::heightOf(curr->right));

    return curr;
}

void AVLTree::balance(Node*& root) {
    if (!root) return;

    root->height = 1 + std::max(Node::heightOf(root->left), Node::heightOf(root->right));
    int balance_factor = root->balanceFactor();

    if (balance_factor > 1) { 
        if (root->left->balanceFactor() < 0) 
            root->left = rotateLeft(root->left);
        root = rotateRight(root);               
    } 
    else if (balance_factor < -1) {
        if (root->right->balanceFactor() > 0) 
            root->right = rotateRight(root->right); 
        root = rotateLeft(root);                    
    }
}

bool AVLTree::initialInsert(int value, Node*& root) {
    if (!root) {
        root = new Node(value);
        return true; 
    }

    bool isPut = false;
    if (value < root->value)
        isPut = initialInsert(value, root->left);
    else if (value > root->value)
        isPut = initialInsert(value, root->right);
    else 
        return false; 

    if (isPut) {
        balance(root);
    }
    return isPut;
}

bool AVLTree::initialRemove(int value, Node*& root) {
    if (!root) 
        return false;

    bool isRemove = false;
    if (value < root->value)
        isRemove = initialRemove(value, root->left);
    else if (value > root->value)
        isRemove = initialRemove(value, root->right);
    else {
        isRemove = true;
        
        if (!root->left || !root->right) {
            Node* temp = root;
            root = (root->left) ? root->left : root->right;
            delete temp;
        } 
        else {
            Node* minRight = root->right;
            while (minRight->left)
                minRight = minRight->left;

            root->value = minRight->value;
            
            initialRemove(root->value, root->right);
        }
    }

    if (isRemove && root)
        balance(root);

    return isRemove;
}

AVLTree::AVLTree() : root(nullptr) {}

AVLTree::~AVLTree() {
    deleteNodes(root);
}

// initialize, insert and remove are missing Frame!
void AVLTree::initialize(const std::vector<int>& data, Timeline& timeline) {
    clear(timeline);
    
    timeline.addFrame(Frame(toVector(), {}, 0, "Initializing AVL Tree from data..."));

    Node* current;
    for (int value : data) {
        initialInsert(value, current);
    }

    timeline.addFrame(Frame(toVector(), {}, 0, "Initialization complete."));
}

void AVLTree::insert(int value, Timeline& timeline) {
    timeline.addFrame(Frame(toVector(), {}, 1, "Starting insertion of " + std::to_string(value)));

    if (initialInsert(value, root)) 
        return;

    timeline.addFrame(Frame(toVector(), {}, 2, "Successfully inserted " + std::to_string(value)));
}

void AVLTree::remove(int value, Timeline& timeline) {
    timeline.addFrame(Frame(toVector(), {}, 1, "Attempting to remove " + std::to_string(value)));

    if (initialRemove(value, root))
        return;

    timeline.addFrame(Frame(toVector(), {}, 2, "Value " + std::to_string(value) + " was not found in the tree."));
}

void AVLTree::search(int value, Timeline& timeline) {
    std::vector<int> currentState = toVector();
    timeline.addFrame(Frame(currentState, {}, 1, "Searching for " + std::to_string(value)));

    Node* current = root;
    while (current) {
        // Need to include index for highlighting
        // timeline.addFrame(Frame(currentState, {index}, 2, "Comparing with node value " + std::to_string(current->value)));
        if (current->value == value) {
            // timeline.addFrame(Frame(currentState, {index}, 3, "Value found at index " + std::to_string(index)));
            return;
        }
            
        current = (value < current->value) ? current->left : current->right;
    }
    timeline.addFrame(Frame(currentState, {}, 4, "Value " + std::to_string(value) + " was not found in the tree."));
}

void AVLTree::clear(Timeline& timeline) {
    timeline.addFrame(Frame(toVector(), {}, 1, "Clearing the entire tree..."));

    deleteNodes(root);

    timeline.addFrame(Frame(toVector(), {}, 2, "Tree cleared successfully."));
}