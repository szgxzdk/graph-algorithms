#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WEIGHT_BOUND 30

int main(int argc, char ** argv)
{
  if (argc != 4) {
    fprintf(stderr, "usage: data_generator seed vertex_number edge_number\n");
    return 1;
  }

  int vn = atoi(argv[2]), en = atoi(argv[3]), seed = atoi(argv[1]);

  srandom(seed);

  char filename[128] = "data_s";
  strcat(filename, argv[1]);
  strcat(filename, "_v");
  strcat(filename, argv[2]);
  strcat(filename, "_e");
  strcat(filename, argv[3]);

  FILE * fp = fopen(filename, "w");
  if (fp == NULL) {
    fprintf(stderr, "can't open \'%s\'\n", filename);
    return 1;
  }
  
  fprintf(fp, "%d\n", vn);
  for (int i = 0; i < en; i++) {
    fprintf(fp, "%ld %ld %f\n",
	    random() % vn, 
	    random() % vn,
	    (float)(random() % (WEIGHT_BOUND * 101)) / 101);
  }

  fclose(fp);
  return 0;
}
