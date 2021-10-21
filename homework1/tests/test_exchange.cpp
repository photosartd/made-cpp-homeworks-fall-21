#include <gtest/gtest.h>
#include <cmath>

extern "C" {
    #include "exchange.h"
    #include "in_out_helpers.h"
    #include "limits.h"
}

TEST(Exchange_, init_statically) {
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
    ASSERT_EQ(num_cur, exch.number_of_currencies);
    ASSERT_EQ(bank_perc, exch.bank_percentage);
    ASSERT_STREQ(bank_name, exch.bank_name);
    EXPECT_TRUE(0 == memcmp(curr_names, exch.currency_names, sizeof(curr_names)));
    EXPECT_TRUE(0 == memcmp(exch_matrix, exch.exchange_matrix, sizeof(exch_matrix)));
}

TEST(Exchange_, get_exchange_scheme) {
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
    double multiplier;
    char *scheme = get_exchange_scheme(&exch, "roubles", "dollars", &multiplier);
    ASSERT_DOUBLE_EQ(0.53, std::floor((multiplier * 100) + .5) / 100);
    //0.53 = round(0.45 * 1.2 * (1 - 0.01)^2, 2)
    scheme = get_exchange_scheme(&exch, "dollars", "roubles", &multiplier);
    ASSERT_DOUBLE_EQ(2.30, std::floor((multiplier * 100) + .5) / 100);
    scheme = get_exchange_scheme(&exch, "euro", "dollars", &multiplier);
    ASSERT_DOUBLE_EQ(1.225, std::floor((multiplier * 1000) + .5) / 1000);
}

TEST(Helpers, get_currency_index) {
    int num_cur = 3;
    char *curr_names[] = {"roubles", "dollars", "euro"};
    ASSERT_EQ(1, get_currency_index(curr_names, num_cur, "dollars"));
    ASSERT_EQ(-1, get_currency_index(curr_names, num_cur, "pounds"));
}

TEST(Helpers, get_max_rate) {
    double rates[] = {INT_MAX, 0.54, INT_MAX, 1.22};
    int num_cur = 4;
    int idx;
    //Максимальный коэффициент перевода (функция не должна обращать внимания на заглушки в виде INT_MAX)
    ASSERT_DOUBLE_EQ(1.22, get_max_rate(rates, &idx, num_cur));
    //Индекс максимального коэффициента перевода
    ASSERT_EQ(3, idx);
}