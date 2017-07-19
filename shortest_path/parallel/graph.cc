#include <limits.h>
#include <pthread.h>

#include <vector>
#include <list>

#include "graph.h"

using namespace std;

const double INF = __DBL_MAX__;

static
void *
thr_fun(void * arg);

int tnum;
const vector<vertex> * vs;

//global result
vector<double> minws;
vector<bool> s;

pthread_barrier_t b1, b2;

//local result
vector<double> lminw;
vector<int> lprev, lminwi;

vector<int>
graph::single_src_sp(const graph & g, int src, int tn)
{
  tnum = tn;
  
  vector<int> prevs;
  vs = &(g.vertices);
  
  prevs.resize(vs->size());
  minws.resize(vs->size());
  s.resize(vs->size());

  //initialization
  for (size_t i = 0; i < vs->size(); i++) {
    prevs[i] = -1;
    if ((int)i == src) {
      minws[i] = 0;
      s[i] = true;
    } else {
      minws[i] = INF;
      s[i] = false;
    }
  }

  //global iterations
  lminw.resize(tnum);
  lminwi.resize(tnum);
  lprev.resize(tnum);
  
  pthread_t tids[tnum];
  pthread_barrier_init(&b1, NULL, tnum + 1);
  pthread_barrier_init(&b2, NULL, tnum + 1);
  for (long i = 0; i < tnum; i++)
    pthread_create(&tids[i], NULL, thr_fun, (void *)i);

  for (size_t i = 0; i < vs->size() - 1; i++) {
    pthread_barrier_wait(&b1);
    
    double gminw = lminw[0];
    int gminwi = lminwi[0], gprev = lprev[0];
    for (int j = 1; j < tnum; j++)
      if (lminw[j] < gminw) {
	gminw = lminw[j];
	gminwi = lminwi[j];
	gprev = lprev[j];
      }
    
    if (gminw == INF)
      break;

    s[gminwi] = true;
    prevs[gminwi] = gprev;
    minws[gminwi] = gminw;

    pthread_barrier_wait(&b2);
  }

  //deallocate pthread resources
  for (int k = 0; k < tnum; k++)
    pthread_cancel(tids[k]);
  //pthread_barrier_destroy(&b1);
  //pthread_barrier_destroy(&b2);

  return prevs;
}

static
void *
thr_fun(void * arg)
{
  long id = (long)arg;

  size_t left = (double)id * vs->size() / tnum;
  size_t right = (double)(id + 1) * vs->size() / tnum;
  if (id == tnum - 1)
    right = vs->size();
  
  //local iterations
  bool done = false;
  while (!done) {
    double minw = INF;
    int prev, minwi;
    bool all_in_s = true;
    for (size_t j = left; j < right; j++) {
      if (s[j] == true) {
	const list<edge> & nbs = (*vs)[j].get_edges();
	for (list<edge>::const_iterator it = nbs.begin();
	     it != nbs.end();
	     it++) {
	  if (s[it->dst] == false) {
	    double w = minws[j] + it->weight;
	    if (w < minw) {
	      minw = w;
	      minwi = it->dst;
	      prev = j;
	    }
	  }
	}
      } else {
	all_in_s = false;
      }
    }

    lminw[id] = minw;
    lminwi[id] = minwi;
    lprev[id] = prev;

    if (all_in_s) 
      done = true;

    pthread_barrier_wait(&b1);
    pthread_barrier_wait(&b2);
  }

  lminw[id] = INF;

  while (true) {
    pthread_barrier_wait(&b1);
    pthread_barrier_wait(&b2);
  }
}
