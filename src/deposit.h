#ifndef DEPOSIT_H_
#define DEPOSIT_H_

#include <math.h>
#include <stdlib.h>

#define OK 0
#define FAIL 1

#define MONTHS_IN_YEAR 12
#define PERCENT_DIVISOR 1200.0
#define TAX_DIVISOR 100.0

int calculate_deposit(long double *sum, int date, long double percent,
                      long double tax, int payment_period,
                      int capitalisation_type, double *input_arr,
                      double *output_arr, long double *dep_result,
                      long double *total_tax);

#endif
