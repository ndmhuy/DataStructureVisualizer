#include "Model/StandardStructure/AVLTree.h"

#include <algorithm>
#include <queue>
#include <string>

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

TreePayload AVLTree::makeTreePayload(const std::vector<size_t>& highlightedNodes) {
    std::vector<TreeNodeData> nodes;
    getNodeIndex.clear();

    if (!root) {
        return TreePayload({}, 0, highlightedNodes);
    }

    std::queue<std::pair<Node*, size_t>> que;
    que.push({root, 0});

    while (!que.empty()) {
        Node* current = que.front().first;
        size_t currentId = que.front().second;
        que.pop();

        if (!current) {
            continue;
        }

        TreeNodeData nodeData;
        nodeData.id = currentId;
        nodeData.value = current->value;
        nodeData.height = static_cast<size_t>(current->height);

        if (current->left) {
            nodeData.leftId = 2 * currentId + 1;
            que.push({current->left, nodeData.leftId});
        }

        if (current->right) {
            nodeData.rightId = 2 * currentId + 2;
            que.push({current->right, nodeData.rightId});
        }

        getNodeIndex[current->value] = currentId;
        nodes.push_back(nodeData);
    }

    return TreePayload(std::move(nodes), 0, highlightedNodes);
}

void AVLTree::deleteNodes(Node*& root) {
    if (!root) {
        return;
    }

    std::queue<Node*> que;
    que.push(root);

    while (!que.empty()) {
        Node* current = que.front();
        que.pop();

        if (current->left) {
            que.push(current->left);
        }
        if (current->right) {
            que.push(current->right);
        }

        delete current;
    }

    root = nullptr;
}

AVLTree::Node* AVLTree::rotateRight(Node* root, Timeline& timeline) {
    makeTreePayload();
    Node* curr = root->left;
    size_t currIdx = getNodeIndex[curr->value];
    size_t rootIdx = getNodeIndex[root->value];

    timeline.addFrame(Frame(makeTreePayload({currIdx, rootIdx}), 1, "Changing the connection between" + std::to_string(root->value) + " and " + std::to_string(curr->value)));
    root->left = curr->right;
    curr->right = root;

    // Update again
    currIdx = getNodeIndex[curr->value];
    rootIdx = getNodeIndex[root->value];
    timeline.addFrame(Frame(makeTreePayload({currIdx, rootIdx}), 2, "Rotating done"));

    root->height = 1 + std::max(Node::heightOf(root->left), Node::heightOf(root->right));
    curr->height = 1 + std::max(Node::heightOf(curr->left), Node::heightOf(curr->right));
    timeline.addFrame(Frame(makeTreePayload({currIdx, rootIdx}), 3, "Updating the height"));

    return curr;
}

AVLTree::Node* AVLTree::rotateLeft(Node* root, Timeline& timeline) {
    makeTreePayload();
    Node* curr = root->right;
    size_t currIdx = getNodeIndex[curr->value];
    size_t rootIdx = getNodeIndex[root->value];

    timeline.addFrame(Frame(makeTreePayload({currIdx, rootIdx}), 1, "Changing the connection between" + std::to_string(root->value) + " and " + std::to_string(curr->value)));
    root->right = curr->left;
    curr->left = root;

    // Update again
    currIdx = getNodeIndex[curr->value];
    rootIdx = getNodeIndex[root->value];
    timeline.addFrame(Frame(makeTreePayload({currIdx, rootIdx}), 2, "Rotating done"));

    root->height = 1 + std::max(Node::heightOf(root->left), Node::heightOf(root->right));
    curr->height = 1 + std::max(Node::heightOf(curr->left), Node::heightOf(curr->right));
    timeline.addFrame(Frame(makeTreePayload({currIdx, rootIdx}), 3, "Updating the height"));

    return curr;
}

void AVLTree::balance(Node*& root, Timeline& timeline) {
    if (!root) {
        return;
    }

    makeTreePayload();
    size_t rootIdx = getNodeIndex[root->value];
    timeline.addFrame(Frame(makeTreePayload({rootIdx}), 1, "Updating the height"));
    root->height = 1 + std::max(Node::heightOf(root->left), Node::heightOf(root->right));
    int balance_factor = root->balanceFactor();
    bool hasRotation = false; // Skipping updating if there is not any rotation

    if (balance_factor > 1) {
        hasRotation = true;

        if (root->left->balanceFactor() < 0) {
            timeline.addFrame(Frame(makeTreePayload({rootIdx}), 2, "Found left right imbalance"));
            root->left = rotateLeft(root->left, timeline);
        } else {
            timeline.addFrame(Frame(makeTreePayload({rootIdx}), 3, "Found left left imbalance"));
        }

        root = rotateRight(root, timeline);
    } else if (balance_factor < -1) {
        hasRotation = true;

        if (root->right->balanceFactor() > 0) {
            timeline.addFrame(Frame(makeTreePayload({rootIdx}), 4, "Found right left imbalance"));
            root->right = rotateRight(root->right, timeline);
        } else {
            timeline.addFrame(Frame(makeTreePayload({rootIdx}), 5, "Found right right imbalance"));
        }

        root = rotateLeft(root, timeline);
    }

    // Update again if needed
    if (hasRotation) {
        rootIdx = getNodeIndex[root->value];
        timeline.addFrame(Frame(makeTreePayload({rootIdx}), 6, "Balance completed"));
    }
}

bool AVLTree::initialInsert(int value, Node*& root, Timeline& timeline) {
    if (!root) {
        root = new Node(value);
        makeTreePayload();
        size_t newIdx = getNodeIndex[value];
        timeline.addFrame(Frame(makeTreePayload({newIdx}), 1, "Inserted " + std::to_string(value)));
        return true;
    }

    size_t currIdx = getNodeIndex[root->value];

    bool isPut = false;
    if (value < root->value) {
        timeline.addFrame(Frame(makeTreePayload({currIdx}), 2, std::to_string(value) + " < " + std::to_string(root->value) + ", go to the left branch"));
        isPut = initialInsert(value, root->left, timeline);
    } else if (value > root->value) {
        timeline.addFrame(Frame(makeTreePayload({currIdx}), 3, std::to_string(value) + " > " + std::to_string(root->value) + ", go to the right branch"));
        isPut = initialInsert(value, root->right, timeline);
    } else {
        timeline.addFrame(Frame(makeTreePayload({currIdx}), 4, std::to_string(value) + " has existed in tree. Canceled"));
        return false;
    }

    if (isPut) {
        balance(root, timeline);
    }
    return isPut;
}

bool AVLTree::initialRemove(int value, Node*& root, Timeline& timeline) {
    if (!root) {
        return false;
    }

    size_t currIdx = getNodeIndex[root->value];

    bool isRemove = false;
    if (value < root->value) {
        timeline.addFrame(Frame(makeTreePayload({currIdx}), 1, std::to_string(value) + " < " + std::to_string(root->value) + ", go to the left branch"));
        isRemove = initialRemove(value, root->left, timeline);
    } else if (value > root->value) {
        timeline.addFrame(Frame(makeTreePayload({currIdx}), 2, std::to_string(value) + " > " + std::to_string(root->value) + ", go to the right branch"));
        isRemove = initialRemove(value, root->right, timeline);
    } else {
        timeline.addFrame(Frame(makeTreePayload({currIdx}), 3, "Found " + std::to_string(value)));
        isRemove = true;

        if (!root->left || !root->right) {
            Node* temp = root;
            root = (root->left) ? root->left : root->right;
            delete temp;

            timeline.addFrame(Frame(makeTreePayload({}), 4, "Removed " + std::to_string(value)));
        } else {
            timeline.addFrame(Frame(makeTreePayload({currIdx}), 5, "There are 2 children, finding the inorder successor node"));

            Node* minRight = root->right;
            while (minRight->left) {
                minRight = minRight->left;
            }

            size_t minRightIdx = getNodeIndex[minRight->value];
            timeline.addFrame(Frame(makeTreePayload({currIdx, minRightIdx}), 6, "Overwrite " + std::to_string(minRight->value) + " on " + std::to_string(root->value)));

            root->value = minRight->value;
            initialRemove(root->value, root->right, timeline);

            size_t newRootIdx = getNodeIndex[root->value];
            timeline.addFrame(Frame(makeTreePayload({newRootIdx}), 7, "Removed the inorder successor"));
        }
    }

    if (isRemove && root) {
        balance(root, timeline);
    }

    return isRemove;
}

AVLTree::AVLTree() : root(nullptr) {}

AVLTree::~AVLTree() {
    deleteNodes(root);
}

void AVLTree::initialize(const std::vector<int>& data, Timeline& timeline) {
    clear(timeline);
    timeline.addFrame(Frame(makeTreePayload({}), 0, "Initializing AVL Tree from data..."));

    for (int value : data) {
        initialInsert(value, root, timeline);
    }

    timeline.addFrame(Frame(makeTreePayload({}), 0, "Initialization complete."));
}

void AVLTree::insert(int value, Timeline& timeline) {
    timeline.addFrame(Frame(makeTreePayload({}), 1, "Starting insertion of " + std::to_string(value)));

    if (!initialInsert(value, root, timeline)) {
        return;
    }

    timeline.addFrame(Frame(makeTreePayload({}), 2, "Successfully inserted " + std::to_string(value)));
}

void AVLTree::remove(int value, Timeline& timeline) {
    timeline.addFrame(Frame(makeTreePayload({}), 1, "Attempting to remove " + std::to_string(value)));

    if (initialRemove(value, root, timeline)) {
        return;
    }

    timeline.addFrame(Frame(makeTreePayload({}), 2, "Value " + std::to_string(value) + " was not found in the tree."));
}

void AVLTree::search(int value, Timeline& timeline) {
    timeline.addFrame(Frame(makeTreePayload({}), 1, "Searching for " + std::to_string(value)));

    Node* current = root;
    while (current) {
        size_t index = getNodeIndex[current->value];
        timeline.addFrame(Frame(makeTreePayload({index}), 2, "Comparing with node value " + std::to_string(current->value)));
        if (current->value == value) {
            timeline.addFrame(Frame(makeTreePayload({index}), 3, "Value found at index " + std::to_string(index)));
            return;
        }

        current = (value < current->value) ? current->left : current->right;
    }
    timeline.addFrame(Frame(makeTreePayload({}), 4, "Value " + std::to_string(value) + " was not found in the tree."));
}

void AVLTree::clear(Timeline& timeline) {
    timeline.addFrame(Frame(makeTreePayload({}), 1, "Clearing the entire tree..."));

    deleteNodes(root);

    timeline.addFrame(Frame(makeTreePayload({}), 2, "Tree cleared successfully."));
}

StructureType AVLTree::getStructureType() const {
    return StructureType::AVLTree;
}
