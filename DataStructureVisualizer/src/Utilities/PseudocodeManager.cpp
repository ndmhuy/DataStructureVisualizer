#include "Utilities/PseudocodeManager.h"

const std::vector<std::string> &
PseudocodeManager::getPseudocode(AlgorithmType type) {
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
        // Grid graph algorithms
        case AlgorithmType::GraphGridBFSShortestPath:
            return GraphGridBFSShortestPathCode;
        case AlgorithmType::GraphGridAStar:
            return GraphGridAStarCode;
        // General graph algorithms
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
const std::vector<std::string> PseudocodeManager::GraphGridBFSShortestPathCode =
{
    "function bfs(G, start, target):", // Line 0
    "  vis = 2D array of false",       // Line 1
    "  prev = 2D array of null",       // Line 2
    "  dist = 2D array of inf",        // Line 3
    "  dirs = [(0,1), (0,-1)...]",     // Line 4
    "  q = empty queue",               // Line 5
    "  q.enqueue(start)",              // Line 6
    "  vis[start] = true",             // Line 7
    "  dist[start] = 0",               // Line 8
    "  while q is not empty:",         // Line 9
    "    u = q.dequeue()",             // Line 10
    "    if u == target:",             // Line 11
    "      return getPath(prev, tgt)", // Line 12
    "    for dir in dirs:",            // Line 13
    "      v = u + dir",               // Line 14
    "      if valid(v) and !vis[v]:",  // Line 15
    "        vis[v] = true",           // Line 16
    "        dist[v] = dist[u] + 1",   // Line 17
    "        prev[v] = u",             // Line 18
    "        q.enqueue(v)",            // Line 19
    "  return inf"                     // Line 20
};

const std::vector<std::string> PseudocodeManager::GraphGridAStarCode = {
    "function astar(G, s, tgt):",          // 0
    "  gCost = 2D array of inf",           // 1
    "  fCost = 2D array of inf",           // 2
    "  prev = 2D array of null",           // 3
    "  dirs = [(0,1), (0,-1)...]",         // 4
    "  gCost[s] = 0",                      // 5
    "  fCost[s] = h(s, tgt)",              // 6
    "  pq.insert(s, fCost[s])",            // 7
    "  while pq is not empty:",            // 8
    "    u, f = pq.poll()",                // 9
    "    if u == tgt:",                    // 10
    "      return getPath(prev, tgt)",     // 11
    "    if gCost[u] < f - h(u, tgt):",    // 12
    "      continue",                      // 13
    "    for dir in dirs:",                // 14
    "      v = u + dir",                   // 15
    "      if valid(v) and v != Wall:",    // 16
    "        tentG = gCost[u] + 1",        // 17
    "        if tentG < gCost[v]:",        // 18
    "          gCost[v] = tentG",          // 19
    "          fCost[v] = tentG+h(v,tgt)", // 20
    "          prev[v] = u",               // 21
    "          pq.insert(v, fCost[v])",    // 22
    "  return inf"                         // 23
};

const std::vector<std::string> PseudocodeManager::GraphDAGShortestPathCode = {
    "function dagShortestPath(G, s):",  // Line 0
    "  dist = [inf, ..., inf]",         // Line 1
    "  prev = [null, ..., null]",       // Line 2
    "  dist[s] = 0",                    // Line 3
    "  inDegree = computeInDegrees(G)", // Line 4
    "  q = nodes with inDegree == 0",   // Line 5
    "  while q is not empty:",          // Line 6
    "    u = q.dequeue()",              // Line 7
    "    for e in G[u]:",               // Line 8
    "      v, w = e.to, e.cost",        // Line 9
    "      if dist[u] + w < dist[v]:",  // Line 10
    "        dist[v] = dist[u] + w",    // Line 11
    "        prev[v] = u",              // Line 12
    "      inDegree[v] -= 1",           // Line 13
    "      if inDegree[v] == 0:",       // Line 14
    "        q.enqueue(v)",             // Line 15
    "  return dist, prev"               // Line 16
};

const std::vector<std::string> PseudocodeManager::GraphDijkstraCode = {
    "function dijkstra(G, s):",        // Line 0
    "  dist = [inf, ..., inf]",        // Line 1
    "  prev = [null, ..., null]",      // Line 2
    "  if any edge weight < 0: return error", // Line 3
    "  dist[s] = 0",                   // Line 4
    "  pq = empty priority queue",     // Line 5
    "  pq.insert((s, 0))",             // Line 6
    "  while pq is not empty:",        // Line 7
    "    u, d = pq.poll()",            // Line 8
    "    if dist[u] < d: continue",    // Line 9
    "    for e in G[u]:",              // Line 10
    "      v, w = e.to, e.cost",       // Line 11
    "      if d + w < dist[v]:",       // Line 12
    "        dist[v] = d + w",         // Line 13
    "        prev[v] = u",             // Line 14
    "        pq.insert((v, dist[v]))", // Line 15
    "  return dist, prev"              // Line 16
};

const std::vector<std::string> PseudocodeManager::GraphAStarCode = {
    "function astar(G, s, tgt):",         // Line 0
    "  gCost = [inf, ..., inf]",          // Line 1
    "  hCost = [inf, ..., inf]",          // Line 2
    "  fCost = [inf, ..., inf]",          // Line 3
    "  if any edge weight < 0: return error", // Line 4
    "  prev = [null, ..., null]",         // Line 5
    "  gCost[s] = 0",                     // Line 6
    "  hCost[s] = h(s, tgt)",             // Line 7
    "  fCost[s] = gCost[s] + hCost[s]",   // Line 8
    "  pq.insert((s, fCost[s], gCost[s]))", // Line 9
    "  while pq is not empty:",           // Line 10
    "    u, f, g = pq.poll()",            // Line 11
    "    if u == tgt:",                   // Line 12
    "      return getPath(prev, tgt)",    // Line 13
    "    if g > gCost[u]: continue",      // Line 14
    "    for e in G[u]:",                 // Line 15
    "      v, w = e.to, e.cost",          // Line 16
    "      tentG = g + w",                // Line 17
    "      if tentG < gCost[v]:",         // Line 18
    "        gCost[v] = tentG",           // Line 19
    "        hCost[v] = h(v, tgt)",       // Line 20
    "        fCost[v] = gCost[v] + hCost[v]", // Line 21
    "        prev[v] = u",                // Line 22
    "        pq.insert((v, fCost[v], gCost[v]))", // Line 23
    "  return inf, null"                  // Line 24
};

const std::vector<std::string> PseudocodeManager::GraphBellmanFordCode = {
    "function bellmanford(G, s):",        // Line 0
    "  dist = [inf, ..., inf]",           // Line 1
    "  prev = [null, ..., null]",         // Line 2
    "  dist[s] = 0",                      // Line 3
    "  for i in range(0, V - 1):",        // Line 4
    "    for e in G.edges:",              // Line 5
    "      u,v,w = e.from, e.to, e.cost", // Line 6
    "      if dist[u] + w < dist[v]:",    // Line 7
    "        dist[v] = dist[u] + w",      // Line 8
    "        prev[v] = u",                // Line 9
    "  negQ = empty queue",               // Line 10
    "  for e in G.edges:",                // Line 11
    "    u,v,w = e.from, e.to, e.cost",   // Line 12
    "    if dist[u] + w < dist[v]:",      // Line 13
    "      dist[v] = -inf",               // Line 14
    "      negQ.enqueue(v)",              // Line 15
    "  while negQ is not empty:",         // Line 16
    "    u = negQ.dequeue()",             // Line 17
    "    for e in G[u]:",                 // Line 18
    "      if dist[e.to] != -inf:",       // Line 19
    "        dist[e.to] = -inf",          // Line 20
    "        negQ.enqueue(e.to)",         // Line 21
    "  return dist, prev"                 // Line 22
};

const std::vector<std::string> PseudocodeManager::GraphFloydWarshallCode = {
    "function floydwarshall(G):",          // Line 0
    "  dist = V x V array of inf",         // Line 1
    "  next = V x V array of null",        // Line 2
    "  for i in range(0, V):",             // Line 3
    "    dist[i][i] = 0",                  // Line 4
    "  for e in G.edges:",                 // Line 5
    "    dist[e.from][e.to] = e.cost",     // Line 6
    "    next[e.from][e.to] = e.to",       // Line 7
    "  for k in range(0, V):",             // Line 8
    "    for i in range(0, V):",           // Line 9
    "      for j in range(0, V):",         // Line 10
    "        sum = dist[i][k]+dist[k][j]", // Line 11
    "        if sum < dist[i][j]:",        // Line 12
    "          dist[i][j] = sum",          // Line 13
    "          next[i][j] = next[i][k]",   // Line 14
    "  return dist, next"                  // Line 15
};

const std::vector<std::string> PseudocodeManager::GraphJohnsonCode = {
    "function johnson(G):",              // Line 0
    "  newV = addNewVertex(G)",          // Line 1
    "  for v in G.vertices:",            // Line 2
    "    addEdge(G, newV, v, 0)",        // Line 3
    "  h = bellmanford(G, newV)",        // Line 4
    "  if any h[v] == -inf:",            // Line 5
    "    return null",                   // Line 6
    "  for e in G.edges:",               // Line 7
    "    e.cost += h[e.from] - h[e.to]", // Line 8
    "  dist = V x V array of inf",       // Line 9
    "  next = V x V array of null",      // Line 10
    "  for v in G.vertices:",            // Line 11
    "    d, n = dijkstra(G, v)",         // Line 12
    "    dist[v] = d",                   // Line 13
    "    next[v] = n",                   // Line 14
    "  for i in range(V):",              // Line 15
    "    for j in range(V):",            // Line 16
    "      if dist[i][j] != inf:",       // Line 17
    "        dist[i][j] += h[j] - h[i]", // Line 18
    "  return dist, next"                // Line 19
};