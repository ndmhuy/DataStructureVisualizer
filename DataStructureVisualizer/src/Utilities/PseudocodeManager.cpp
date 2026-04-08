#include "Utilities/PseudocodeManager.h"

const std::vector<std::string>& PseudocodeManager::getPseudocode(AlgorithmType type) {
    switch (type) {
        case AlgorithmType::None:
            return {"No algorithm selected."};
        case AlgorithmType::SinglyLinkedListInsert:
            return SinglyLinkedListInsertCode;
        case AlgorithmType::SinglyLinkedListDelete:
            return SinglyLinkedListDeleteCode;
        case AlgorithmType::SinglyLinkedListSearch:
            return SinglyLinkedListSearchCode;
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

const std::vector<std::string> PseudocodeManager::SinglyLinkedListDeleteCode = {
    "function delete(value):", // Line 0
    "   if head is null:", // Line 1
    "       return", // Line 2
    "   if head.value == value:", // Line 3
    "       head = head.next", // Line 4
    "       return", // Line 5
    "   current = head", // Line 6
    "   while current.next is not null:", // Line 7
    "       if current.next.value == value:", // Line 8
    "           deleteNode = current.next", // Line 9
    "           current.next = current.next.next", // Line 10
    "           delete deleteNode", // Line 11
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