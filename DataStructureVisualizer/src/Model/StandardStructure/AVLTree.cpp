#include "Model/StandardStructure/AVLTree.h"
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

// the toVector will store all the values and nullptr by level (like a heap)
std::vector<int> AVLTree::toVector() {
    std::vector<int> result;
    getNodeIndex.clear();
    // BFS
    std::queue<Node*> que;
    que.push(root);
    
    int currentIdx = 0;
    while (!que.empty()) {
        Node* current = que.front();
        que.pop();

        if (current) {
            result.push_back(currentIdx);
            getNodeIndex[current->value] = currentIdx;

            que.push(current->left);
            que.push(current->right);
        }
        else {
            result.push_back(-1);
        }

        currentIdx++;
    }

    // Remove redundant -1 (nullptr)
    while (!result.empty() && result.back() == -1) {
        result.pop_back();
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

AVLTree::Node* AVLTree::rotateRight(Node* root, Timeline& timeline) {
    std::vector<int> currentState = toVector();

    Node* curr = root->left;
    int currIdx = getNodeIndex[curr->value];
    int rootIdx = getNodeIndex[root->value];
    
    timeline.addFrame(Frame(currentState, {currIdx, rootIdx}, 1, "Changing the connection between" + std::to_string(root->value) + " and " + std::to_string(curr->value)));
    root->left = curr->right;
    curr->right = root;

    // Update again
    currentState = toVector();
    currIdx = getNodeIndex[curr->value];
    rootIdx = getNodeIndex[root->value];
    timeline.addFrame(Frame(currentState, {currIdx, rootIdx}, 2, "Rotating done"));

    root->height = 1 + std::max(Node::heightOf(root->left), Node::heightOf(root->right));
    curr->height = 1 + std::max(Node::heightOf(curr->left), Node::heightOf(curr->right));
    timeline.addFrame(Frame(currentState, {currIdx, rootIdx}, 3, "Updating the height"));

    return curr;
}

AVLTree::Node* AVLTree::rotateLeft(Node* root, Timeline& timeline) {
    std::vector<int> currentState = toVector();

    Node* curr = root->right;
    int currIdx = getNodeIndex[curr->value];
    int rootIdx = getNodeIndex[root->value];
    
    timeline.addFrame(Frame(currentState, {currIdx, rootIdx}, 1, "Changing the connection between" + std::to_string(root->value) + " and " + std::to_string(curr->value)));
    root->right = curr->left;
    curr->left = root;

    // Update again
    currentState = toVector();
    currIdx = getNodeIndex[curr->value];
    rootIdx = getNodeIndex[root->value];
    timeline.addFrame(Frame(currentState, {currIdx, rootIdx}, 2, "Rotating done"));

    root->height = 1 + std::max(Node::heightOf(root->left), Node::heightOf(root->right));
    curr->height = 1 + std::max(Node::heightOf(curr->left), Node::heightOf(curr->right));
    timeline.addFrame(Frame(currentState, {currIdx, rootIdx}, 3, "Updating the height"));

    return curr;
}

void AVLTree::balance(Node*& root, Timeline& timeline) {
    if (!root) {
        return;
    }

    std::vector<int> currentState = toVector();

    int rootIdx = getNodeIndex[root->value];
    timeline.addFrame(Frame(currentState, {rootIdx}, 1, "Updating the height"));
    root->height = 1 + std::max(Node::heightOf(root->left), Node::heightOf(root->right));
    int balance_factor = root->balanceFactor();
    bool hasRotation = false; // Skipping updating if there is not any rotation

    if (balance_factor > 1) { 
        hasRotation = true;

        if (root->left->balanceFactor() < 0) {
            timeline.addFrame(Frame(currentState, {rootIdx}, 2, "Found left right imbalance"));
            root->left = rotateLeft(root->left, timeline);
        }
        else
            timeline.addFrame(Frame(currentState, {rootIdx}, 3, "Found left left imbalance"));

        root = rotateRight(root, timeline);               
    } 
    else if (balance_factor < -1) {
        hasRotation = true;

        if (root->right->balanceFactor() > 0) {
            timeline.addFrame(Frame(currentState, {rootIdx}, 4, "Found right left imbalance"));
            root->right = rotateRight(root->right, timeline); 
        }
        else 
            timeline.addFrame(Frame(currentState, {rootIdx}, 5, "Found right right imbalance"));
            
        root = rotateLeft(root, timeline);                    
    }

    // Update again if needs
    if (hasRotation) {
        rootIdx = getNodeIndex[root->value];
        currentState = toVector();
        timeline.addFrame(Frame(currentState, {rootIdx}, 6, "Balance completed"));
    }
}

bool AVLTree::initialInsert(int value, Node*& root, Timeline& timeline) {
    if (!root) {
        root = new Node(value);

        std::vector<int> currentState = toVector();
        int newIdx = getNodeIndex[value];
        timeline.addFrame(Frame(currentState, {newIdx}, 1, "Inserted " + std::to_string(value)));

        return true; 
    }

    std::vector<int> currentState = toVector();
    int currIdx = getNodeIndex[root->value];

    bool isPut = false;
    if (value < root->value) {
        timeline.addFrame(Frame(currentState, {currIdx}, 2, std::to_string(value) + " < " + std::to_string(root->value) + ", go to the left branch"));
        isPut = initialInsert(value, root->left, timeline);
    }
    else if (value > root->value) {
        timeline.addFrame(Frame(currentState, {currIdx}, 3, std::to_string(value) + " > " + std::to_string(root->value) + ", go to the right branch"));
        isPut = initialInsert(value, root->right, timeline);
    }
    else {
        timeline.addFrame(Frame(currentState, {currIdx}, 4, std::to_string(value) + " has existed in tree. Canceled"));
        return false; 
    }

    if (isPut) {
        balance(root, timeline);
    }
    return isPut;
}

bool AVLTree::initialRemove(int value, Node*& root, Timeline& timeline) {
    if (!root) 
        return false;

    std::vector<int> currentState = toVector();
    int currIdx = getNodeIndex[root->value];

    bool isRemove = false;
    if (value < root->value) {
        timeline.addFrame(Frame(currentState, {currIdx}, 1, std::to_string(value) + " < " + std::to_string(root->value) + ", go to the left branch"));
        isRemove = initialRemove(value, root->left, timeline);
    }
    else if (value > root->value) {
        timeline.addFrame(Frame(currentState, {currIdx}, 2, std::to_string(value) + " < " + std::to_string(root->value) + ", go to the right branch"));
        isRemove = initialRemove(value, root->right, timeline);
    }
    else {
        timeline.addFrame(Frame(currentState, {currIdx}, 3, "Found " + std::to_string(value)));
        isRemove = true;
        
        if (!root->left || !root->right) {
            Node* temp = root;
            root = (root->left) ? root->left : root->right;
            delete temp;

            std::vector<int> afterState = toVector();
            timeline.addFrame(Frame(afterState, {}, 4, "Removed " + std::to_string(value)));
        } 
        else {
            timeline.addFrame(Frame(currentState, {currIdx}, 5, "There are 2 children, finding the inorder successor node"));

            Node* minRight = root->right;
            while (minRight->left)
                minRight = minRight->left;

            int minRightIdx = getNodeIndex[minRight->value];
            timeline.addFrame(Frame(currentState, {currIdx, minRightIdx}, 6, "Overwrite " + std::to_string(minRight->value) + " on " + std::to_string(root->value)));

            root->value = minRight->value;

            std::vector<int> stateAfterSwap = toVector();
            int newRootIdx = getNodeIndex[root->value];

            timeline.addFrame(Frame(stateAfterSwap, {newRootIdx}, 7, "Calling a recursion to remove the alternative"));
            
            initialRemove(root->value, root->right, timeline);
        }
    }

    if (isRemove && root)
        balance(root, timeline);

    return isRemove;
}

AVLTree::AVLTree() : root(nullptr) {}

AVLTree::~AVLTree() {
    deleteNodes(root);
}

void AVLTree::initialize(const std::vector<int>& data, Timeline& timeline) {
    clear(timeline);
    std::vector<int> currentState = toVector();
    timeline.addFrame(Frame(currentState, {}, 0, "Initializing AVL Tree from data..."));

    Node* current;
    for (int value : data) {
        initialInsert(value, current, timeline);
    }

    for (int idx = 0; idx < currentState.size(); ++idx) {
        getNodeIndex[currentState[idx]] = idx;
    }

    timeline.addFrame(Frame(toVector(), {}, 0, "Initialization complete."));
}

void AVLTree::insert(int value, Timeline& timeline) {
    timeline.addFrame(Frame(toVector(), {}, 1, "Starting insertion of " + std::to_string(value)));

    if (!initialInsert(value, root, timeline)) 
        return;

    timeline.addFrame(Frame(toVector(), {}, 2, "Successfully inserted " + std::to_string(value)));
}

void AVLTree::remove(int value, Timeline& timeline) {
    timeline.addFrame(Frame(toVector(), {}, 1, "Attempting to remove " + std::to_string(value)));

    if (initialRemove(value, root, timeline))
        return;

    timeline.addFrame(Frame(toVector(), {}, 2, "Value " + std::to_string(value) + " was not found in the tree."));
}

void AVLTree::search(int value, Timeline& timeline) {
    std::vector<int> currentState = toVector();
    timeline.addFrame(Frame(currentState, {}, 1, "Searching for " + std::to_string(value)));

    Node* current = root;
    while (current) {
        int index = getNodeIndex[current->value];
        timeline.addFrame(Frame(currentState, {index}, 2, "Comparing with node value " + std::to_string(current->value)));
        if (current->value == value) {
            timeline.addFrame(Frame(currentState, {index}, 3, "Value found at index " + std::to_string(index)));
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

StructureType AVLTree::getStructureType() const {
    return StructureType::AVLTree;
}