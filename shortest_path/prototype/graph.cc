#include <limits.h>

#include <vector>
#include <list>

#include "graph.h"

using namespace std;

const double INF = __DBL_MAX__;

vector<int>
graph::single_src_sp(const graph & g, int src)
{
  const vector<vertex> & vertices = g.vertices;
  vector<int> prevs(vertices.size());
  vector<double> minws(vertices.size());
  vector<bool> s(vertices.size());

  //initialization
  for (size_t i = 0; i < vertices.size(); i++) {
    prevs[i] = -1;
    if ((int)i == src) {
      minws[i] = 0;
      s[i] = true;
    } else {
      minws[i] = INF;
      s[i] = false;
    }
  }

  //iterations
  for (size_t i = 0; i < vertices.size() - 1; i++) {
    double minw = INF;
    int prev, minwi;

    for (size_t j = 0; j < vertices.size(); j++) {
      if (s[j] == true) {
	const list<edge> & nbs = vertices[j].get_edges();
	for (list<edge>::const_iterator it = nbs.begin();
	     it != nbs.end();
	     it++)
	  if (s[it->dst] == false) {
	    double w = minws[j] + it->weight;
	    if (w < minw) {
	      minw = w;
	      minwi = it->dst;
	      prev = j;
	    }
	  }
      }
    }
    
    if (minw == INF)
      break;

    s[minwi] = true;
    prevs[minwi] = prev;
    minws[minwi] = minw;
  }

  return prevs;
}
