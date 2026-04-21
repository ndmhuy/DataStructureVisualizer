#include "Model/StandardStructure/SinglyLinkedList.h"
#include <string>

std::vector<int> SinglyLinkedList::toVector() const {
    std::vector<int> result;
    const Node* current = head;
    while (current) {
        result.push_back(current->value);
        current = current->next;
    }
    return result;
}

void SinglyLinkedList::deleteNodes(Node*& head) {
    while (head) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

SinglyLinkedList::SinglyLinkedList() : head(nullptr) {}

SinglyLinkedList::~SinglyLinkedList() {
    deleteNodes(head);
}

void SinglyLinkedList::initialize(const std::vector<int>& data, Timeline& timeline) {
    clear(timeline);
    
    timeline.addFrame(Frame(LinkedListPayload(toVector(), {}), 0, "Initializing Singly Linked List from data..."));

    Node* current;
    for (int value : data) {
        Node* newNode = new Node(value);
        if (!head) {
            head = newNode;
            current = head;
        } else {
            current->next = newNode;
            current = current->next;
        }
    }
    timeline.addFrame(Frame(LinkedListPayload(toVector(), {}), 0, "Initialization complete."));
}

void SinglyLinkedList::insert(int value, Timeline& timeline) {
    timeline.addFrame(Frame(LinkedListPayload(toVector(), {}), 1, "Starting insertion of " + std::to_string(value)));

    Node* newNode = new Node(value);

    if (!head) {
        head = newNode;
        timeline.addFrame(Frame(LinkedListPayload(toVector(), {}), 2, "List was empty. Inserted at head."));
        return;
    }

    Node* current = head;
    size_t index = 0;

    std::vector<int> currentState = toVector();
    while (current->next) {
        current = current->next;
        index++;
        timeline.addFrame(Frame(LinkedListPayload(currentState, {index}), 3, "Traversing... current node is " + std::to_string(current->value)));
    }
    current->next = newNode;
    timeline.addFrame(Frame(LinkedListPayload(toVector(), {index + 1}), 4, "Successfully inserted " + std::to_string(value) + " at the tail."));
}

void SinglyLinkedList::remove(int value, Timeline& timeline) {
    timeline.addFrame(Frame(LinkedListPayload(toVector(), {}), 1, "Attempting to remove " + std::to_string(value)));

    if (!head) {
        timeline.addFrame(Frame(LinkedListPayload(toVector(), {}), 2, "List is empty. Nothing to remove."));
        return;
    }

    if (head->value == value) {
        timeline.addFrame(Frame(LinkedListPayload(toVector(), {0}), 3, "Value found at head. Removing..."));
        Node* temp = head;
        head = head->next;
        delete temp;
        timeline.addFrame(Frame(LinkedListPayload(toVector(), {}), 4, "Head removed successfully."));
        return;
    }

    Node* current = head;
    size_t index = 0;

    std::vector<int> currentState = toVector();
    while (current->next) {
        timeline.addFrame(Frame(LinkedListPayload(currentState, {index, index + 1}), 5, "Checking if next node is " + std::to_string(current->next->value)));
        if (current->next->value == value) {
            timeline.addFrame(Frame(LinkedListPayload(currentState, {index + 1}), 6, "Target found! Unlinking node..."));

            Node* nodeToRemove = current->next;
            current->next = current->next->next;
            delete nodeToRemove;

            timeline.addFrame(Frame(LinkedListPayload(toVector(), {}), 7, "Node removed successfully."));
            return;
        }
        current = current->next;
        index++;
    }
    timeline.addFrame(Frame(LinkedListPayload(toVector(), {}), 8, "Value " + std::to_string(value) + " was not found in the list."));
}

void SinglyLinkedList::search(int value, Timeline& timeline) {
    std::vector<int> currentState = toVector();
    timeline.addFrame(Frame(LinkedListPayload(currentState, {}), 1, "Searching for " + std::to_string(value)));

    Node* current = head;
    size_t index = 0;
    
    while (current) {
        timeline.addFrame(Frame(LinkedListPayload(currentState, {index}), 2, "Comparing with node value " + std::to_string(current->value)));
        if (current->value == value) {
            timeline.addFrame(Frame(LinkedListPayload(currentState, {index}), 3, "Value found at index " + std::to_string(index)));
            return;
        }
        current = current->next;
        index++;
    }
    timeline.addFrame(Frame(LinkedListPayload(currentState, {}), 4, "Value " + std::to_string(value) + " was not found in the list."));
}

void SinglyLinkedList::clear(Timeline& timeline) {
    timeline.addFrame(Frame(LinkedListPayload(toVector(), {}), 1, "Clearing the entire linked list..."));

    deleteNodes(head);

    timeline.addFrame(Frame(LinkedListPayload(toVector(), {}), 2, "Linked list cleared successfully."));
}

StructureType SinglyLinkedList::getStructureType() const {
    return StructureType::SinglyLinkedList;
}