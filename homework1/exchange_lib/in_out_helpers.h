#ifndef HELPERS
#define HELPERS
#include "exchange.h"

void help();
void enter_int(int *num);
void enter_float(float *num);
char **enter_curr_names(int num_cur);
double *enter_exchange_matrix(int num_cur);
void print_matrix(const double *matrix, int n, int m);
int get_currency_index(char **curr_names, int n, char *cur);
void print_arr(const double *arr, int size);
void fill_rates_array(double *rates, const double *exchange_matrix, int idx1, int idx2, int num_cur);
double get_max_rate(const double *rates, int *idx, int num_cur);
char *concat_scheme(const char *first, const char *second, const char *between, const char *divider);
void start_currency_session();
Exchange start_example_static_session();

#endif