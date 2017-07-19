#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <stack>

#include "graph.h"

using namespace std;

void
print_paths(int src, vector<int> paths);

int main(int argc, char ** argv)
{
  if (argc != 3) {
    fprintf(stderr, "usage: shortest_path_parallel thread_number input_file\n");
    return 1;
  }

  FILE * fp = fopen(argv[2], "r");
  if (fp == NULL) {
    fprintf(stderr, "can't open \'%s\'\n", argv[2]);
    return 1;
  }

  int tnum;
  tnum = atoi(argv[1]);

  graph g;
  
  int vnum;
  if (fscanf(fp, "%d\n", &vnum) < 1) {
    fprintf(stderr, "no vertex number read\n");
    return 1;
  }
  if (tnum > vnum)
    tnum = vnum;
  for (int i = 0; i < vnum; i++)
    g.add_vertex(vertex());
  
  int src, dst;
  double weight;
  while (fscanf(fp, "%d %d %lg\n", &src, &dst, &weight) == 3)
    for (int i = 0; i < 9; i++) {
      g.add_edge(src, dst, weight);
      g.add_edge(dst, src, weight);
    }

  vector<int> prevs = graph::single_src_sp(g, 0, tnum);

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
