#include "exchange.h"
#include "in_out_helpers.h"
#include <stdlib.h>
#include <stdio.h>

Exchange init_statically(char *bank_name,
                        int number_of_currencies,
                        float bank_percentage,
                        char **currency_names,
                        double *exchange_matrix
) {
    Exchange exch;
    if (number_of_currencies < 3) {
        printf("Error: Would not be able to convert using different ways.");
        return exch;
    }
    exch.bank_name = bank_name;
    exch.number_of_currencies = number_of_currencies;
    exch.bank_percentage = bank_percentage;
    exch.currency_names = currency_names;
    exch.exchange_matrix = exchange_matrix;
    return exch;
}

Exchange init_dynamically() {
    help();
    printf("Enter bank name:\t");
    char name[MAX_NAME_SIZE];
    scanf("%s", name);
    printf("Enter number of currencies. Can't be less than %d:\t", MIN_CUR_NUMBER);
    int num_cur = 0;
    enter_int(&num_cur);
    printf("Enter bank percentge (float):\t");
    float bank_perc = 0.0;
    enter_float(&bank_perc);
    printf("Enter %d currency names, line by line. Not more than %d sybmols each:\n", num_cur, MAX_NAME_SIZE);
    char **curr_names = enter_curr_names(num_cur);
    printf("Enter exchange matrix, line by line (%d lines), %d doubles in line through spaces:\n", num_cur, num_cur);
    double *exchange_matrix = enter_exchange_matrix(num_cur);
    return init_statically(
        name,
        num_cur,
        bank_perc,
        curr_names,
        exchange_matrix
    );
}

void print_exchange(const Exchange *const p) {
    printf("Bank name: %s \n", p->bank_name);
    printf("Number of currencies: %d \n", p->number_of_currencies);
    printf("Bank percentage: %0.2f \n", p->bank_percentage);
    printf("Currencies names: \n");
    for (int i = 0; i < p->number_of_currencies; ++i) {
        printf("Currency %d: %s \n", i + 1, *(p->currency_names + i));
    }
    print_matrix(p->exchange_matrix, p->number_of_currencies, p->number_of_currencies);
}

char *get_exchange_scheme(const Exchange *const p, char *cur1, char *cur2, double *multiplier) {
    int idx1 = get_currency_index(p->currency_names, p->number_of_currencies, cur1);
    int idx2 = get_currency_index(p->currency_names, p->number_of_currencies, cur2);
    if (idx1 == -1 || idx2 == -1) {
        printf("Wrong currency names. Try again. \n");
        return "Error";
    }
    double *rates = malloc(p->number_of_currencies * sizeof(double));
    fill_rates_array(rates, p->exchange_matrix, idx1, idx2, p->number_of_currencies);
    int idx_of_rate;
    //write to multiplier
    *multiplier = get_max_rate(rates, &idx_of_rate, p->number_of_currencies) * (1 - p->bank_percentage) * (1 - p->bank_percentage);
    char div[5] = " -> ";
    free(rates); 
    return concat_scheme(cur1, cur2, p->currency_names[idx_of_rate], div);
}
