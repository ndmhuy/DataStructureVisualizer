#include "Utilities/PseudocodeManager.h"

const std::vector<std::string>& PseudocodeManager::getPseudocode(AlgorithmType type) {
    switch (type) {
        case AlgorithmType::None:
            return {"No algorithm selected."};
        
        // Singly Linked List
        case AlgorithmType::SinglyLinkedListInsert:
            return SinglyLinkedListInsertCode;
        case AlgorithmType::SinglyLinkedListRemove:
            return SinglyLinkedListRemoveCode;
        case AlgorithmType::SinglyLinkedListSearch:
            return SinglyLinkedListSearchCode;
        case AlgorithmType::SinglyLinkedListUpdate:
            return SinglyLinkedListUpdateCode;

        // Heap
        case AlgorithmType::HeapInsert:
            return HeapInsertCode;
        case AlgorithmType::HeapRemove:
            return HeapRemoveCode;
        case AlgorithmType::HeapSearch:
            return HeapSearchCode;
        case AlgorithmType::HeapUpdate:
            return HeapUpdateCode;

        // AVL Tree
        case AlgorithmType::AVLTreeInsert:
            return AVLTreeInsertCode;
        case AlgorithmType::AVLTreeRemove:
            return AVLTreeRemoveCode;
        case AlgorithmType::AVLTreeSearch:
            return AVLTreeSearchCode;
        case AlgorithmType::AVLTreeUpdate:
            return AVLTreeUpdateCode;

        // Graph
        case AlgorithmType::GraphGridBFSShortestPath:
            return GraphGridBFSShortestPathCode;
        case AlgorithmType::GraphDAGShortestPath:
            return GraphDAGShortestPathCode;
        case AlgorithmType::GraphDijkstra:
            return GraphDijkstraCode;
        case AlgorithmType::GraphAStar:
            return GraphAStarCode;
        case AlgorithmType::GraphBellmanFord:
            return GraphBellmanFordCode;
        case AlgorithmType::GraphFloydWarshall:
            return GraphFloydWarshallCode;
        case AlgorithmType::GraphJohnson:
            return GraphJohnsonCode;
        default:
            return {"No pseudocode available for this algorithm."};
    }
}

// Singly Linked List
const std::vector<std::string> PseudocodeManager::SinglyLinkedListInsertCode = {
    "function insert(value):", // Line 0
    "   newNode = Node(value)", // Line 1
    "   if head is null:", // Line 2
    "       head = newNode", // Line 3
    "       return", // Line 4
    "   current = head", // Line 5
    "   while current.next is not null:", // Line 6
    "       current = current.next", // Line 7
    "       current.next = newNode" // Line 8
};

const std::vector<std::string> PseudocodeManager::SinglyLinkedListRemoveCode = {
    "function remove(value):", // Line 0
    "   if head is null:", // Line 1
    "       return", // Line 2
    "   if head.value == value:", // Line 3
    "       head = head.next", // Line 4
    "       return", // Line 5
    "   current = head", // Line 6
    "   while current.next is not null:", // Line 7
    "       if current.next.value == value:", // Line 8
    "           removeNode = current.next", // Line 9
    "           current.next = current.next.next", // Line 10
    "           delete removeNode", // Line 11
    "           return", // Line 12
    "       current = current.next" // Line 13
};

const std::vector<std::string> PseudocodeManager::SinglyLinkedListSearchCode = {
    "function search(value):", // Line 0
    "   current = head", // Line 1
    "   while current is not null:", // Line 2
    "       if current.value == value:", // Line 3
    "           return true", // Line 4
    "       current = current.next", // Line 5
    "   return false" // Line 6
};

const std::vector<std::string> PseudocodeManager::SinglyLinkedListUpdateCode = {
    "function update(oldValue, newValue):", // Line 0
    "   current = head", // Line 1
    "   while current is not null:", // Line 2
    "       if current.value == oldValue:", // Line 3
    "           current.value = newValue", // Line 4
    "           return true", // Line 5
    "       current = current.next", // Line 6
    "   return false" // Line 7
};

// Heap
const std::vector<std::string> PseudocodeManager::HeapInsertCode = {
    "function insert(value):", // Line 0
    "   heap.append(value)", // Line 1
    "   i = heap.length - 1", // Line 2
    "   while i > 0 and compare(i, parent(i)):", // Line 3
    "       swap(heap[i], heap[parent(i)])", // Line 4
    "       i = parent(i)" // Line 5
};

const std::vector<std::string> PseudocodeManager::HeapRemoveCode = {
    "function remove(value):", // Line 0
    "   index = search(value)", // Line 1
    "   if index == -1: return", // Line 2
    "   swap(heap[index], heap.last())", // Line 3
    "   heap.removeLast()", // Line 4
    "   if index < heap.length:", // Line 5
    "       fixNode(index) // shiftUp or shiftDown" // Line 6
};

const std::vector<std::string> PseudocodeManager::HeapSearchCode = {
    "function search(value):", // Line 0
    "   for i = 0 to heap.length - 1:", // Line 1
    "       if heap[i] == value:", // Line 2
    "           return i", // Line 3
    "   return -1" // Line 4
};

const std::vector<std::string> PseudocodeManager::HeapUpdateCode = {
    "function update(oldValue, newValue):", // Line 0
    "   index = search(oldValue)", // Line 1
    "   if index == -1: return false", // Line 2
    "   heap[index] = newValue", // Line 3
    "   fixNode(index) // shiftUp or shiftDown", // Line 4
    "   return true" // Line 5
};

// AVL Tree
const std::vector<std::string> PseudocodeManager::AVLTreeInsertCode = {
    "function insert(node, value):", // Line 0
    "   if node is null: return new Node(value)", // Line 1
    "   if value < node.value:", // Line 2
    "       node.left = insert(node.left, value)", // Line 3
    "   else if value > node.value:", // Line 4
    "       node.right = insert(node.right, value)", // Line 5
    "   node.height = 1 + max(height(node.left), height(node.right))", // Line 6
    "   balanceFactor = height(node.left) - height(node.right)", // Line 7
    "   return balance(node, balanceFactor)" // Line 8
};

const std::vector<std::string> PseudocodeManager::AVLTreeRemoveCode = {
    "function remove(node, value):", // Line 0
    "   if node is null: return node", // Line 1
    "   if value < node.value: node.left = remove(node.left, value)", // Line 2
    "   else if value > node.value: node.right = remove(node.right, value)", // Line 3
    "   else:", // Line 4
    "       if node has 1 or 0 children: return non_null_child_or_null", // Line 5
    "       temp = minValueNode(node.right)", // Line 6
    "       node.value = temp.value", // Line 7
    "       node.right = remove(node.right, temp.value)", // Line 8
    "   if node is null: return node", // Line 9
    "   node.height = 1 + max(height(node.left), height(node.right))", // Line 10
    "   balanceFactor = height(node.left) - height(node.right)", // Line 11
    "   return balance(node, balanceFactor)" // Line 12
};

const std::vector<std::string> PseudocodeManager::AVLTreeSearchCode = {
    "function search(node, value):", // Line 0
    "   while node is not null:", // Line 1
    "       if value == node.value: return node", // Line 2
    "       if value < node.value:", // Line 3
    "           node = node.left", // Line 4
    "       else:", // Line 5
    "           node = node.right", // Line 6
    "   return null" // Line 7
};

const std::vector<std::string> PseudocodeManager::AVLTreeUpdateCode = {
    "function update(root, oldValue, newValue):", // Line 0
    "   if search(root, oldValue) is null:", // Line 1
    "       return root // Not found", // Line 2
    "   root = remove(root, oldValue)", // Line 3
    "   root = insert(root, newValue)", // Line 4
    "   return root" // Line 5
};

// Graph
const std::vector<std::string> PseudocodeManager::GraphGridBFSShortestPathCode = {
    "function bfsShortestPath(graph, start, target):", // Line 0
    "   visited = 2D array of size rows x cols initialized to false", // Line 1
    "   distances = 2D array of size rows x cols initialized to inf", // Line 2
    "   directions = [(0,1), (0,-1), (1,0), (-1,0)] # right, left, down, up", // Line 3
    "   queue = empty queue", // Line 4
    "   queue.enqueue(start)", // Line 5
    "   visited[start.row][start.col] = true", // Line 6
    "   distances[start.row][start.col] = 0", // Line 7
    "   while queue is not empty:", // Line 8
    "       current = queue.dequeue()", // Line 9
    "       if current == target:", // Line 10
    "           return distances[target.row][target.col]", // Line 11
    "       for direction in directions:", // Line 12
    "           newRow = current.row + direction[0]", // Line 13
    "           newCol = current.col + direction[1]", // Line 14
    "           if isValid(newRow, newCol) and not visited[newRow][newCol]:", // Line 15
    "               visited[newRow][newCol] = true", // Line 16
    "               distances[newRow][newCol] = distances[current.row][current.col] + 1", // Line 17
    "               queue.enqueue((newRow, newCol))", // Line 18
    "   return inf" // Line 19
};

const std::vector<std::string> PseudocodeManager::GraphDAGShortestPathCode = {
    "function dagShortestPath(graph, start):", // Line 0
    "   visited = [false, false, ... , false] # size V", // Line 1
    "   distances = [inf, inf, ... , inf] # size V", // Line 2
    "   distances[start] = 0", // Line 3
    "   topoOrder = topologicalSort(graph)", // Line 4
    "   while topoOrder is not empty:", // Line 5
    "       index = topoOrder.pop()", // Line 6
    "       if distances[index] == inf: continue", // Line 7
    "       for edge in graph[index]:", // Line 8
    "           newDist = distances[index] + edge.cost", // Line 9
    "           if newDist < distances[edge.to]:", // Line 10
    "               distances[edge.to] = newDist", // Line 11
    "   return distances", // Line 12
    "",
    "function topologicalSort(graph):", // Line 13
    "   visited = [false, false, ... , false] # size V", // Line 14
    "   order = empty stack", // Line 15
    "   for i in range(0, V):", // Line 16
    "       if !visited[i]:", // Line 17
    "           dfs(graph, i, visited, order)", // Line 18
    "   return order", // Line 19
    ""
    "function dfs(graph, index, visited, order):", // Line 20
    "   visited[index] = true", // Line 21
    "   for edge in graph[index]:", // Line 22
    "       if not visited[edge.to]:", // Line 23
    "           dfs(graph, edge.to, visited, order)", // Line 24
    "   order.push(index)" // Line 25
};

const std::vector<std::string> PseudocodeManager::GraphDijkstraCode = {
    "function dijkstra(graph, start):", // Line 0
    "   visited = [false, false, ... , false] # size V", // Line 1
    "   distances = [inf, inf, ... , inf] # size V", // Line 2
    "   distances[start] = 0", // Line 3
    "   pq = empty priority queue", // Line 4
    "   pq.insert((start, 0))", // Line 5
    "   while pq is not empty:", // Line 6
    "       index, minValue = pq.poll()", // Line 7
    "       visited[index] = true", // Line 8
    "       if distances[index] < minValue: continue", // Line 9
    "       for (edge in graph[index]):", // Line 10
    "           if visited[edge.to]: continue", // Line 11
    "           newDist = distances[index] + edge.cost", // Line 12
    "           if newDist < distances[edge.to]:", // Line 13
    "               distances[edge.to] = newDist", // Line 14
    "               pq.insert((edge.to, newDist))", // Line 15
    "   return distances" // Line 16
};

const std::vector<std::string> PseudocodeManager::GraphAStarCode = {
    "function astar(graph, start, target, heuristic):", // Line 0
    "   visited = [false, false, ... , false] # size V", // Line 1
    "   distances = [inf, inf, ... , inf] # size V", // Line 2
    "   distances[start] = 0", // Line 3
    "   pq = empty priority queue", // Line 4
    "   pq.insert((start, heuristic(start)))", // Line 5
    "   while pq is not empty:", // Line 6
    "       index, _ = pq.poll()", // Line 7
    "       if index == target: return distances[target]", // Line 8
    "       visited[index] = true", // Line 9
    "       for edge in graph[index]:", // Line 10
    "           if visited[edge.to]: continue", // Line 11
    "           newDist = distances[index] + edge.cost", // Line 12
    "           if newDist < distances[edge.to]:", // Line 13
    "               distances[edge.to] = newDist", // Line 14
    "               pq.insert((edge.to, newDist + heuristic(edge.to)))", // Line 15
    "   return inf", // Line 16
    "",
    "function heuristic(vertex):", // Line 17
    "   # Implementation depends on the specific heuristic used (e.g., Manhattan distance, Euclidean distance, etc.)", // Line 18
    "   return estimatedCost" // Line 19
};

const std::vector<std::string> PseudocodeManager::GraphBellmanFordCode = {
    "function bellmanford(graph, start):", // Line 0
    "   distances = [inf, inf, ... , inf] # size V", // Line 1
    "   distances[start] = 0", // Line 2
    "   for i in range(0, V - 1):", // Line 3
    "       for edge in graph.edges:", // Line 4
    "           if distances[edge.from] + edge.cost < distances[edge.to]:", // Line 5
    "               distances[edge.to] = distances[edge.from] + edge.cost", // Line 6
    "   negVertices = empty queue", // Line 7
    "   visited = [false, false, ... , false] # size V", // Line 8
    "   for edge in graph.edges:", // Line 9
    "       if distances[edge.from] + edge.cost < distances[edge.to]:", // Line 10
    "           distances[edge.to] = -inf", // Line 11
    "           negVertices.enqueue(edge.to)", // Line 12
    "   while negVertices is not empty:", // Line 13
    "       index = negVertices.dequeue()", // Line 14
    "       if visited[index]: continue", // Line 15
    "       visited[index] = true", // Line 16
    "       dfs(graph, index, distances, negVertices, visited)", // Line 17
    "   return distances", // Line 18
    "",
    "function dfs(graph, index, distances, negVertices, visited):", // Line 19
    "   for edge in graph[index]:", // Line 20
    "       if visited[edge.to]: continue", // Line 21
    "       distances[edge.to] = -inf", // Line 22
    "       negVertices.enqueue(edge.to)", // Line 23
    "       dfs(graph, edge.to, distances, negVertices, visited)" // Line 24
};

const std::vector<std::string> PseudocodeManager::GraphFloydWarshallCode = {
    "function floydwarshall(graph):", // Line 0
    "   distances = 2D array of size V x V initialized to inf", // Line 1
    "   next = 2D array of size V x V initialized to null", // Line 2
    "   setup(graph, distances, next)", // Line 3
    "   for k in range(0, V):", // Line 4
    "       for i in range(0, V):", // Line 5
    "           for j in range(0, V):", // Line 6
    "               if distances[i][k] + distances[k][j] < distances[i][j]:", // Line 7
    "                   distances[i][j] = distances[i][k] + distances[k][j]", // Line 8
    "                   next[i][j] = next[i][k]", // Line 9
    "   return distances, next", // Line 10
    "",
    "function setup(graph, distances, next):", // Line 11
    "   for i in range(0, V):", // Line 12
    "       distances[i][i] = 0", // Line 13
    "   for edge in graph.edges:", // Line 14
    "       distances[edge.from][edge.to] = edge.cost", // Line 15
    "       next[edge.from][edge.to] = edge.to" // Line 16
};

const std::vector<std::string> PseudocodeManager::GraphJohnsonCode = {
    "function johnson(graph):", // Line 0
    "   newVertex = addNewVertex(graph)", // Line 1
    "   for vertex in graph.vertices:", // Line 2
    "       addEdge(graph, newVertex, vertex, 0)", // Line 3
    "   h = bellmanford(graph, newVertex)", // Line 4
    "   if any h[vertex] == -inf: return null", // Line 5
    "   reweightGraph(graph, h)", // Line 6
    "   distances = 2D array of size V x V", // Line 7
    "   for vertex in graph.vertices:", // Line 8
    "       distances[vertex] = dijkstra(graph, vertex)", // Line 9
    "   return distances", // Line 10
    "",
    "function reweightGraph(graph, h):", // Line 11
    "   for edge in graph.edges:", // Line 12
    "       edge.cost = edge.cost + h[edge.from] - h[edge.to]" // Line 13
};