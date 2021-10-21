#ifndef EXCHANGE_H
#define EXCHANGE_H
#define MIN_CUR_NUMBER 3
#define MAX_NAME_SIZE 30
#define MATRIX_LINE_SIZE 30

typedef struct Exchange
{
    char *bank_name;
    int number_of_currencies;
    float bank_percentage;
    char **currency_names;
    double *exchange_matrix;
} Exchange;

Exchange init_statically(char *bank_name,
                        int number_of_currencies,
                        float bank_percentage,
                        char **currency_names,
                        double *exchange_matrix
);
Exchange init_dynamically();
void print_exchange(const Exchange *const p);
char *get_exchange_scheme(const Exchange *const p, char *cur1, char *cur2, double *multiplier);

#endif