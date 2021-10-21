#include "in_out_helpers.h"
#include <stdio.h>
#include "exchange.h"
#include <stdlib.h>
#include "string.h"
#include "limits.h"
#include "math.h"

void help() {
    printf("This is an exchange configurer.\n");
}

void enter_int(int *num) {
    char buff[256];
    while ( fgets(buff, sizeof(buff), stdin) ) {
       if ( sscanf(buff, "%d", num) == 1 ) {
           if (*num < MIN_CUR_NUMBER) {
               printf("Number of currencies can't be less than %d. Enter again\n", MIN_CUR_NUMBER);
               continue;
           }
           break;
       };
    };
}

void enter_float(float *num) {
    char buff[256];
    while ( fgets(buff, sizeof(buff), stdin) ) {
       if ( sscanf(buff, "%f", num) == 1 ) {
           if (*num < 0.0 || *num > 1.0) {
               printf("Bank percentage must be between 0.0 and 1.0. Try aragin\n");
               continue;
           }
           break;
       };
    };
}

char **enter_curr_names(int num_cur) {
    char **curr_names;
    curr_names = malloc(num_cur * sizeof(char*));
    for (int i = 0; i < num_cur; ++i) {
        curr_names[i] = malloc(MAX_NAME_SIZE * sizeof(char));
    }
    for (int i = 0; i < num_cur; ++i) {
        scanf("%s", curr_names[i]);
    }
    return curr_names;
}

double *enter_exchange_matrix(int num_cur) {
    double *exchange_matrix;
    exchange_matrix = malloc(num_cur * num_cur * sizeof(double));
    char to_trash[MATRIX_LINE_SIZE];
    fgets(to_trash, MATRIX_LINE_SIZE, stdin);
    for (int i = 0; i < num_cur; ++i) {
        char str[MATRIX_LINE_SIZE];
        fgets(str, MATRIX_LINE_SIZE, stdin);
        char *pch;
        int j = 0;
        pch = strtok(str, " ");
        while (pch != NULL && j < num_cur) {
            *(exchange_matrix + i * num_cur + j) = strtod(pch, NULL);
            pch = strtok(NULL, " ");
            j++;
        }
    }
    return exchange_matrix;
}

void print_matrix(const double *matrix, int n, int m) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            printf("%.2f ", *(matrix + n * i + j));
        }
        printf("\n");
    }
}

int get_currency_index(char **curr_names, int n, char *cur) {
    for (int i = 0; i < n; ++i) {
        if (strcmp(*(curr_names + i), cur) == 0) {
            return i;
        }
    }
    return -1;
}

void print_arr(const double *arr, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%lf ", *(arr + i));
    }
}

void fill_rates_array(double *rates, const double *exchange_matrix, int idx1, int idx2, int num_cur) {
    for (int i = 0; i < num_cur; ++i) {
        if (i == idx1 || i == idx2) {
            *(rates + i) = INT_MAX;
        }
        else {
            *(rates + i) = (*(exchange_matrix + idx1 * num_cur + i)) * (*(exchange_matrix + i * num_cur + idx2));
        }
    }
}

double get_max_rate(const double *rates, int *idx, int num_cur) {
    double max_rate = -1.0;
    for (int i = 0; i < num_cur; ++i) {
        if (((*(rates + i)) != INT_MAX) && ((*(rates + i)) > max_rate)) {
            max_rate = (*(rates + i));
            *idx = i;
        }
    }
    return max_rate;
}

char *concat_scheme(const char *first, const char *second, const char *between, const char *divider) {
    //check every curr name is less than 30
    char *res = malloc(sizeof(char) * 100);
    int x = 0;
    while (x < (int)strlen(first)) {res[x] = first[x]; ++x;}
    int i = 0;
    while (i < (int)strlen(divider)) {res[x] = divider[i]; ++x; ++i;}
    i = 0;
    while (i < (int)strlen(between)) {res[x] = between[i]; ++x; ++i;}
    i = 0;
    while (i < (int)strlen(divider)) {res[x] = divider[i]; ++x; ++i;}
    i = 0;
    while (i < (int)strlen(second)) {res[x] = second[i]; ++x; ++i;}
    res[x] = '\n';
    return res;
}

void start_currency_session() {
    Exchange exch = init_dynamically();
    char cur1[MAX_NAME_SIZE];
    char cur2[MAX_NAME_SIZE];
    printf("Enter start currency name: ");
    scanf("%s", cur1);
    printf("Enter end currency name: ");
    scanf("%s", cur2);
    int idx1 = get_currency_index(exch.currency_names, exch.number_of_currencies, cur1);
    int idx2 = get_currency_index(exch.currency_names, exch.number_of_currencies, cur2);
    if (idx1 == -1 || idx2 == -1) {
        printf("You entered wrong currency names. Program exits.");
        return;
    }
    double exchange_rate;
    printf("Exchange line: %s", get_exchange_scheme(&exch, cur1, cur2, &exchange_rate));
    printf("Exchange rate: %0.2f \n", exchange_rate);
}

Exchange start_example_static_session() {
    char *curr_names[] = {"roubles", "dollars", "euro"};
    double exch_matrix[][3] = {
        {1.0, 0.5, 0.45},
        {1.95, 1.0, 0.94},
        {2.5, 1.2, 1.0}
    };
    int num_cur = 3;
    float bank_perc = 0.01;
    char bank_name[] = "Sber";
    Exchange exch = init_statically(
        bank_name,
        num_cur,
        bank_perc,
        curr_names,
        &exch_matrix[0][0]
    );
    print_exchange(&exch);
    char cur1[10] = "roubles";
    char cur2[10] = "dollars";
    double exchange_rate;
    printf("Exchange line: %s", get_exchange_scheme(&exch, cur1, cur2, &exchange_rate));
    printf("Exchange rate: %0.2f \n", exchange_rate);
    return exch;
}
