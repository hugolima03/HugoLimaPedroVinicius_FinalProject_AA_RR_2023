#include <stdio.h>
#include <stdlib.h>
#include <graphviz/cgraph.h>

int main() {
    Agraph_t *graph = agopen("MyGraph", Agstrictdirected, NULL);
    Agnode_t *node1 = agnode(graph, "Node1", 1);
    Agnode_t *node2 = agnode(graph, "Node2", 1);
    Agnode_t *node3 = agnode(graph, "Node3", 1);
    Agedge_t *edge1 = agedge(graph, node1, node2, NULL, 1);
    Agedge_t *edge2 = agedge(graph, node2, node3, NULL, 1);
    
    FILE *file = fopen("graph.dot", "w");
    agwrite(graph, file);
    fclose(file);
    
    agclose(graph);
    
    return 0;
}
