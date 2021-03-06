#include <stdio.h>

#include <vector>
#include <stack>

#include "graph.h"

using namespace std;

void
print_paths(int src, vector<int> paths);

int main(int argc, char ** argv)
{
  if (argc != 2) {
    fprintf(stderr, "usage: shortest_path_serial input_file\n");
    return 1;
  }

  FILE * fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "can't open \'%s\'\n", argv[1]);
    return 1;
  }

  graph g;
  
  int VNUM;
  if (fscanf(fp, "%d\n", &VNUM) <= 0) {
    fprintf(stderr, "no vertex number read\n");
    return 1;
  }
  for (int i = 0; i < VNUM; i++)
    g.add_vertex(vertex());
  
  int src, dst;
  double weight;
  while (fscanf(fp, "%d %d %lg\n", &src, &dst, &weight) == 3)
    for (int i = 0; i < 9; i++) {
      g.add_edge(src, dst, weight);
      g.add_edge(dst, src, weight);
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
