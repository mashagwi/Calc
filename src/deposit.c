#include "deposit.h"

int calculate_deposit(long double *sum, int date, long double percent,
                 long double tax, int payment_period, int capitalisation_type,
                 double *input_arr, double *output_arr, long double *dep_result,
                 long double *total_tax) {
    int response = OK;
    
    if (date <= 0 || *sum <= 0 || percent < 0 || tax < 0) {
        response = FAIL;
        return response;
    }

    percent /= PERCENT_DIVISOR;
    tax /= TAX_DIVISOR;

    double start_sum = *sum;
    double total_percents = 0;
    double temp_tax = 0;
    double temp_percents = 0;

    for (int i = 0; i < date; i++) {
        *sum = *sum + input_arr[i] - output_arr[i];
        start_sum = start_sum + input_arr[i] - output_arr[i];

        if ((i + 1) % payment_period == 0 || i == date - 1) {
            if (i == date - 1 && payment_period != 1 &&
                (i + 1) % payment_period != 0) {
                payment_period = (i + 1) % payment_period;
            }
            temp_percents = *sum * payment_period * percent;
            temp_tax = temp_percents * tax;
            *total_tax += temp_tax;

            if (capitalisation_type == 0) {
                total_percents += temp_percents - temp_tax;
                temp_percents = 0;
            } else if (capitalisation_type == 1) {
                *sum += temp_percents - temp_tax;
                temp_percents = 0;
            }
            temp_tax = 0;
        }
    }

    if (capitalisation_type == 0) {
        *sum += total_percents;
        *dep_result = total_percents;
    } else if (capitalisation_type == 1) {
        *dep_result = *sum - start_sum;
    }

    return response;
}


// int calc_deposit(double *sum, int date, double percent,
//                  double tax, int payment_period, int capitalisation_type,
//                  double *input_arr, double *output_arr, double *dep_result,
//                  double *total_tax) {
//   int response = OK;
//   double start_sum = *sum;
//   double total_percents = 0;
//   double temp_tax = 0;
//   double temp_percents = 0;
//   if (date <= 0 || *sum <= 0 || percent < 0 || tax < 0) response = FAIL;
//   percent /= 1200;
//   tax /= 100;
//   if (response == OK) {
//     for (int i = 0; i < date; i++) {
//       *sum = *sum + input_arr[i] - output_arr[i];
//       start_sum = start_sum + input_arr[i] - output_arr[i];
//       if ((i + 1) % payment_period == 0 || i == date - 1) {
//         if (i == date - 1 && payment_period != 1 &&
//             (i + 1) % payment_period != 0) {
//           payment_period = (i + 1) % payment_period;
//         }
//         temp_percents = *sum * payment_period * percent;
//         temp_tax = temp_percents * tax;
//         *total_tax += temp_tax;
//         if (capitalisation_type == 0) {
//           total_percents += temp_percents - temp_tax;
//           temp_percents = 0;
//         } else if (capitalisation_type == 1) {
//           *sum += temp_percents - temp_tax;
//           temp_percents = 0;
//         }
//         temp_tax = 0;
//       }
//     }
//     if (capitalisation_type == 0) {
//       *sum += total_percents;
//       *dep_result = total_percents;
//     } else if (capitalisation_type == 1) {
//       *dep_result = *sum - start_sum;
//     }
//   }
//   return response;
// }
