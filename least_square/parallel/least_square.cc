#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <list>
#include <vector>

using namespace std;

#define THREAD_NUM 4

typedef vector<vector<double> > matrix;

class job {
public:
  int col;
  const matrix &m;

  job(int col, matrix &t) : m(t) {
    this->col = col;
  }
};

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

void *
thr_fun(void *arg);

#define DIM 2
#define NUM 30

int main(void)
{
  matrix x, y;
  double var[DIM][NUM] = {
    {
      1, 1, 1, 1, 1,
      1, 1, 1, 1, 1,
      1, 1, 1, 1, 1,
      1, 1, 1, 1, 1,
      1, 1, 1, 1, 1,
      1, 1, 1, 1, 1,
    }, {
      1, 2, 3, 4, 5,
      6, 7, 8, 9, 10,
      11, 12, 13, 14, 15,
      16, 17, 18, 19, 20,
      21, 22, 23, 24, 25,
      26, 27, 28, 29, 30,
    }
  };
  double values[NUM] = {
    33815, 33981, 34004, 34165, 34212,
    34327, 34344, 34458, 34498, 34476,
    34483, 34488, 34513, 34497, 34511,
    34520, 34507, 34509, 34521, 34513,
    34515, 34517, 34519, 34519, 34521,
    34521, 34523, 34525, 34525, 34527
  };

  x.resize(NUM);
  for (int i = 0; i < NUM; i++) {
    x[i].resize(DIM);
    for (int j = 0; j < DIM; j++) {
      x[i][j] = var[j][i];
    }
  }

  y.resize(30);
  for (int i = 0; i < NUM; i++) {
    y[i].resize(1);
    y[i][0] = values[i];
  }

  matrix c = least_square(x, y);

  for (size_t i = 0; i < c.size(); i++)
    for (size_t j = 0; j < c[i].size(); j++)
      printf("%f ", c[i][j]);
  printf("\n");
  
  return 0;
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

  return t;
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

list<job> jobs;
pthread_mutex_t jobs_lock = PTHREAD_MUTEX_INITIALIZER;
vector<double> results;
pthread_mutex_t rlts_lock = PTHREAD_MUTEX_INITIALIZER;

double 
determinant(matrix m)
{
  static bool flag = false;
  int result;

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
  else if (!flag) {
    //parallel implementation
    flag = true;
    result = 0;

    for (size_t i = 0; i < m[0].size(); i++)
      if (m[0][i] != 0)
	jobs.push_back(job(i, m));

    pthread_t tids[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++)
      pthread_create(&tids[i], NULL, thr_fun, NULL);
    for (int i = 0; i < THREAD_NUM; i++)
      pthread_join(tids[i], NULL);
    
    for (size_t i = 0; i < results.size(); i++)
      result += results[i];
  } else {
    result = 0;
    for (size_t i = 0; i < m[i].size(); i++)
      if (m[0][i] != 0)
	result += m[0][i] * cofactor(m, 0, i);
  }

  return result;
}

void *
thr_fun(void *arg)
{
  while (true) {
    pthread_mutex_lock(&jobs_lock);
    if (jobs.empty()) {
      pthread_mutex_unlock(&jobs_lock);
      break;
    } else {
      job a = jobs.front();
      jobs.pop_front();
      pthread_mutex_unlock(&jobs_lock);
      double result = a.m[0][a.col] * cofactor(a.m, 0, a.col);
      pthread_mutex_lock(&rlts_lock);
      results.push_back(result);
      pthread_mutex_unlock(&rlts_lock);
    }
  }
  pthread_exit((void *)0);
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