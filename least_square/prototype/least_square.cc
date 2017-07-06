#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <vector>

using namespace std;

typedef vector<vector<double> > matrix;

matrix
matrix_divide(matrix m, double num);

matrix
matrix_transpose(matrix m);

matrix
matrix_multiply(matrix m1, matrix m2);

matrix
companion_matrix(matrix m);

matrix
inverse_matrix(matrix m);

double
cofactor(matrix m, int row, int col);

double
determinant(matrix m);

matrix
least_square(matrix x, matrix y);

void
matrix_print(matrix m);

int main(int argc, char **argv)
{
  matrix x, y;

  FILE * fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "can't open \'%s\'\n", argv[1]);
    return 1;
  }
  
  char buffer[512];
  while (fgets(buffer, 512, fp)) {
    char *p;
    vector<double> vars;
    for (p = buffer; *p != '\0'; ) {
      for (; *p != '\0' && !isdigit(*p); p++)
  	;
      if (*p != '\0') {
  	double var = atof(p);
  	vars.push_back(var);
  	for (; *p != '\0' && (isdigit(*p) || *p == '.'); p++)
  	  ;
      }
    }
    
    vector<double>::iterator it = vars.end() - 1;
    double value_y = *it;
    vars.erase(it);
    x.push_back(vars);
    vector<double> row_y;
    row_y.push_back(value_y);
    y.push_back(row_y);
    if (vars.size() != x[0].size()) {
      fprintf(stderr, "input dimensions inconsistent\n");
      return 1;
    }
  }

  fclose(fp);
  
  matrix c = least_square(x, y);

  matrix_print(c);
  
  return 0;
}

void
matrix_print(matrix m)
{
  for (size_t i = 0; i < m.size(); i++) {
    for (size_t j = 0; j < m[i].size(); j++)
      printf("%lf ", m[i][j]);
    printf("\n");
  }
}

matrix
matrix_divide(matrix m, double num)
{
  for (size_t i = 0; i < m.size(); i++)
    for (size_t j = 0; j < m[i].size(); j++)
      m[i][j] /= num;

  return m;
}

matrix
matrix_transpose(matrix m)
{
  matrix t;

  t.resize(m[0].size());
  for (size_t i = 0; i < t.size(); i++) {
    t[i].resize(m.size());
    for (size_t j = 0; j < t[i].size(); j++)
      t[i][j] = m[j][i];
  }

  return t;
}

matrix
matrix_multiply(matrix m1, matrix m2)
{
  if (m1[0].size() != m2.size()) {
    fprintf(stderr, "matrix (%ld, %ld) * matrix (%ld, %ld)\n", m1.size(), m1[0].size(), m2.size(), m2[0].size());
    exit(1);
  }

  matrix t;
  t.resize(m1.size());
  for (size_t i = 0; i < t.size(); i++) {
    t[i].resize(m2[0].size());
    for (size_t j = 0; j < t[i].size(); j++) {
      double sum = 0;
      for (size_t k = 0; k < m1[0].size(); k++)
	sum += m1[i][k] * m2[k][j];
      t[i][j] = sum;
    }
  }

  return t;
}

matrix
companion_matrix(matrix m)
{
  matrix t = m;

  for (size_t i = 0; i < t.size(); i++)
    for (size_t j = 0; j < t[i].size(); j++)
      t[i][j] = cofactor(m, i, j);

  return matrix_transpose(t);
}

double
cofactor(matrix m, int row, int col)
{
  {
    vector<vector<double> >::iterator it = m.begin();
    m.erase(it + row);
  }
  for (size_t i = 0; i < m.size(); i++) {
    vector<double>::iterator it = m[i].begin();
    m[i].erase(it + col);
  }

  return (((row + col) % 2 == 0) ? 1 : -1) * determinant(m);
}

double
determinant(matrix m)
{
  double result;

  if (m.size() != m[0].size()) {
    fprintf(stderr, "matrix (%ld, %ld) 's determinant not exist\n",
	    m.size(), m[0].size());
    exit(1);
  }

  if (m.size() == 1)
    result = m[0][0];
  else if (m.size() == 2)
    result = m[0][0] * m[1][1] - m[0][1] * m[1][0];
  else if (m.size() == 3)
    result = m[0][0] * m[1][1] * m[2][2]
      + m[0][1] * m[1][2] * m[2][0]
      + m[0][2] * m[1][0] * m[2][1]
      - m[0][0] * m[1][2] * m[2][1]
      - m[0][1] * m[1][0] * m[2][2]
      - m[0][2] * m[1][1] * m[2][0];
  else {
    result = 0;
    for (size_t i = 0; i < m[0].size(); i++)
      if (m[0][i] != 0)
	result += m[0][i] * cofactor(m, 0, i);
  }

  return result;
}

matrix
inverse_matrix(matrix m)
{
  return matrix_divide(companion_matrix(m), determinant(m));
}

matrix
least_square(matrix x, matrix y)
{
  matrix x_t = matrix_transpose(x);
  return matrix_multiply(inverse_matrix(matrix_multiply(x_t, x)),
			 matrix_multiply(x_t, y));
}
