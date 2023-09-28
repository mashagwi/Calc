#ifndef CREDIT_H_
#define CREDIT_H_

#include <stdlib.h>
#include <math.h>

#define OK 0
#define FAIL 1

#define MONTH 0
#define YEAR 1

#define ANNUITET 0
#define DIFFERENT 1

int calculate_credit(double sum, int *date, int date_type, double percent,
                int calculation_type, double **m_pay, double *over_pay,
                double *total_pay);
                                
int calculateAnnuityCredit(double sum, int date, double percent, double *m_pay,
                           double *total_pay);
               
int calculateDifferentCredit(double sum, int date, double percent, double *m_pay,
                                  double *total_pay);

#endif
