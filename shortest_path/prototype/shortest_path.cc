#include <stdio.h>

#include <vector>
#include <stack>

#include "graph.h"

using namespace std;

void
print_paths(int src, vector<int> paths);

const int VNUM = 6;

int main(void)
{
  int edges[][3] = {
    {0, 1, 6},
    {0, 2, 3},
    {1, 2, 2},
    {1, 3, 5},
    {2, 3, 3},
    {2, 4, 4},
    {3, 4, 2},
    {3, 5, 3},
    {4, 5, 5}
  };
  
  graph g;
  for (int i = 0; i < VNUM; i++)
    g.add_vertex(vertex());
  for (int i = 0; i < 9; i++) {
    g.add_edge(edges[i][0], edges[i][1], edges[i][2]);
    g.add_edge(edges[i][1], edges[i][0], edges[i][2]);
  }

  vector<int> prevs = graph::single_src_sp(g, 0);

  print_paths(0, prevs);

  return 0;
}

void
print_paths(int src, vector<int> prevs)
{
  stack<int> path;

  for (size_t i = 0; i < prevs.size(); i++) {
    for (int j = prevs[i]; j != -1; j = prevs[j])
      path.push(j);
    while (!path.empty()) {
      printf("%d -> ", path.top());
      path.pop();
    }
    printf("%ld", i);
    if ((int) i == src)
      printf(" : source node");
    else if (prevs[i] == -1)
      printf(" : unreachable");
    printf("\n");
  }
}
