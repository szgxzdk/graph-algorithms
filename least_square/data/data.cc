#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>

using namespace std;

#define PARA_BOUND 100

int main(int argc, char ** argv)
{
  if (argc != 4) {
    fprintf(stderr, "usage: data_generator seed dimension record_number\n");
    return 1;
  }

  int seed = atoi(argv[1]), dn = atoi(argv[2]), rn = atoi(argv[3]);
  srandom(seed);

  char filename[128] = "data_s";
  strcat(filename, argv[1]);
  strcat(filename, "_d");
  strcat(filename, argv[2]);
  strcat(filename, "_r");
  strcat(filename, argv[3]);

  FILE * fp = fopen(filename, "w");
  if (fp == NULL) {
    fprintf(stderr, "can't open \'%s\'\n", filename);
    return 1;
  }

  vector<int> paras(dn);
  for (int i = 0; i < (int)paras.size(); i++) {
    paras[i] = random() % PARA_BOUND;
    fprintf(fp, "%d ", paras[i]);
  }
  fprintf(fp, "\n");

  vector<int> x(dn);
  for (int i = 0; i < dn; i++)
    x[i] = 1;
  
  for (int i = 0; i < rn; i++) {
    int y = 0;
    for (int j = 0; j < dn; j++) {
      y += x[j] * paras[j];
      fprintf(fp, "%d ", x[j]);
    }
    fprintf(fp, "%d\n", y);

    x[i % dn]++;
  }

  return 0;
}
