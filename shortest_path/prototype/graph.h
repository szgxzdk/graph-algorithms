#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdio.h>

#include <list>
#include <vector>

using namespace std;

class edge {
public:
  int dst;
  double weight;

  edge(int dst, double weight) {
    this->dst = dst;
    this->weight = weight;
  }
};

class vertex {
private:
  double value;
  bool visited = false;
  list<edge> edges;
  
public:
  vertex() {}
  vertex(double value) { this->value = value; }

  void set_visited() { this->visited = true; }
  void unset_visited() { this->visited = false; }
  bool is_visited() { return this->visited; }

  void set_value(const double & value) { this->value = value; }
  double get_value() { return this->value; }

  void add_edge(edge e) { this->edges.push_back(e); }
  const list<edge> & get_edges() const { return this->edges; }
};

class graph {
private:
  vector<vertex> vertices;

public:
  void add_vertex(vertex v) { vertices.push_back(v); }
  vertex get_vertex(int i) { return vertices[i]; }
    
  void add_edge(int src, int dst, double weight) {
    vertices[src].add_edge(edge(dst, weight));
  }
  
  static vector<int> single_src_sp(const graph & g, int src);
};

#endif
