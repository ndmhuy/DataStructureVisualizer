#include "Utilities/PseudocodeManager.h"

const std::vector<std::string> &
PseudocodeManager::getPseudocode(AlgorithmType type) {
    switch (type) {
    case AlgorithmType::None:
        return {"No algorithm selected."};
    case AlgorithmType::SinglyLinkedListInsert:
        return SinglyLinkedListInsertCode;
    case AlgorithmType::SinglyLinkedListDelete:
        return SinglyLinkedListDeleteCode;
    case AlgorithmType::SinglyLinkedListSearch:
        return SinglyLinkedListSearchCode;
    case AlgorithmType::SinglyLinkedListUpdate:
        return SinglyLinkedListUpdateCode;
    case AlgorithmType::GraphGridBFSShortestPath:
        return GraphGridBFSShortestPathCode;
    case AlgorithmType::GraphGridAStar:
        return GraphGridAStarCode;
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

const std::vector<std::string> PseudocodeManager::SinglyLinkedListInsertCode = {
    "function insert(value):",    // Line 0
    "  newNode = Node(value)",    // Line 1
    "  if head is null:",         // Line 2
    "    head = newNode",         // Line 3
    "    return",                 // Line 4
    "  curr = head",              // Line 5
    "  while curr.next != null:", // Line 6
    "    curr = curr.next",       // Line 7
    "  curr.next = newNode"       // Line 8
};

const std::vector<std::string> PseudocodeManager::SinglyLinkedListDeleteCode = {
    "function delete(value):",          // Line 0
    "  if head is null: return",        // Line 1
    "  if head.value == value:",        // Line 2
    "    head = head.next",             // Line 3
    "    return",                       // Line 4
    "  curr = head",                    // Line 5
    "  while curr.next != null:",       // Line 6
    "    if curr.next.value == value:", // Line 7
    "      delNode = curr.next",        // Line 8
    "      curr.next = delNode.next",   // Line 9
    "      delete delNode",             // Line 10
    "      return",                     // Line 11
    "    curr = curr.next"              // Line 12
};

const std::vector<std::string> PseudocodeManager::SinglyLinkedListSearchCode = {
    "function search(value):",     // Line 0
    "  curr = head",               // Line 1
    "  while curr is not null:",   // Line 2
    "    if curr.value == value:", // Line 3
    "      return true",           // Line 4
    "    curr = curr.next",        // Line 5
};

const std::vector<std::string> PseudocodeManager::SinglyLinkedListUpdateCode = {
    "function update(oldV, newV):", // Line 0
    "  curr = head",                // Line 1
    "  while curr is not null:",    // Line 2
    "    if curr.value == oldV:",   // Line 3
    "      curr.value = newV",      // Line 4
    "      return true",            // Line 5
    "    curr = curr.next",         // Line 6
    "  return false"                // Line 7
};

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
    "  dist[s] = 0",                   // Line 3
    "  pq = empty priority queue",     // Line 4
    "  pq.insert((s, 0))",             // Line 5
    "  while pq is not empty:",        // Line 6
    "    u, d = pq.poll()",            // Line 7
    "    if dist[u] < d: continue",    // Line 8
    "    for e in G[u]:",              // Line 9
    "      v, w = e.to, e.cost",       // Line 10
    "      if d + w < dist[v]:",       // Line 11
    "        dist[v] = d + w",         // Line 12
    "        prev[v] = u",             // Line 13
    "        pq.insert((v, dist[v]))", // Line 14
    "  return dist, prev"              // Line 15
};

const std::vector<std::string> PseudocodeManager::GraphAStarCode = {
    "function astar(G, s, tgt):",         // Line 0
    "  gCost = [inf, ..., inf]",          // Line 1
    "  fCost = [inf, ..., inf]",          // Line 2
    "  prev = [null, ..., null]",         // Line 3
    "  gCost[s] = 0",                     // Line 4
    "  fCost[s] = h(s, tgt)",             // Line 5
    "  pq.insert((s, fCost[s]))",         // Line 6
    "  while pq is not empty:",           // Line 7
    "    u, f = pq.poll()",               // Line 8
    "    if u == tgt:",                   // Line 9
    "      return gCost[tgt], prev",      // Line 10
    "    if gCost[u] < f - h(u, tgt):",   // Line 11
    "      continue",                     // Line 12
    "    for e in G[u]:",                 // Line 13
    "      v, w = e.to, e.cost",          // Line 14
    "      tentG = gCost[u] + w",         // Line 15
    "      if tentG < gCost[v]:",         // Line 16
    "        gCost[v] = tentG",           // Line 17
    "        fCost[v] = tentG+h(v, tgt)", // Line 18
    "        prev[v] = u",                // Line 19
    "        pq.insert((v, fCost[v]))",   // Line 20
    "  return inf, null"                  // Line 21
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