#include "credit.h"

int calculate_credit(double sum, int *date, int date_type, double percent,
                     int calculation_type, double **m_pay, double *over_pay,
                     double *total_pay) {
  int response = OK;

  // Проверка аргументов
  if (*date <= 0 || sum <= 0) {
    response = FAIL;
    return response;
  }

  // Преобразование процентной ставки в доли
  percent /= 100;

  // Преобразование периода в месяцы, если он задан в годах
  if (date_type == YEAR) {
    *date *= 12;
    date_type = MONTH;
  }

  // Выделение памяти для m_pay
  *m_pay = calloc(*date, sizeof(double));
  if (*m_pay == NULL) {
    response = FAIL;
    return response;
  }

  // Вычисление кредита на основе выбранного метода
  switch (calculation_type) {
    case ANNUITET:
      response = calculateAnnuityCredit(sum, *date, percent, *m_pay, total_pay);
      break;

    case DIFFERENT:
      response =
          calculateDifferentCredit(sum, *date, percent, *m_pay, total_pay);
      break;

    default:
      response = FAIL;
      break;
  }

  // Вычисление переплаты, если расчет успешен
  if (response == OK) {
    *over_pay = *total_pay - sum;
  }

  return response;
}

int calculateAnnuityCredit(double sum, int date, double percent, double *m_pay,
                           double *total_pay) {
  int response = OK;
  double month_percent = percent / 12.0;
  double annuity_coefficient =
      (month_percent * pow(1 + month_percent, (double)date)) /
      (pow(1 + month_percent, (double)date) - 1);

  double month_payment = sum * annuity_coefficient;

  // Заполнение массива m_pay месячными платежами
  for (int i = 0; i < date; i++) {
    m_pay[i] = month_payment;
  }

  *total_pay = month_payment * date;

  return response;
}

int calculateDifferentCredit(double sum, int date, double percent,
                             double *m_pay, double *total_pay) {
  int response = OK;
  double month_percent = percent / 12.0;
  double month_body_payment = sum / date;

  for (int i = 0; i < date; i++) {
    m_pay[i] = month_body_payment + (sum * month_percent);
    *total_pay += m_pay[i];
    sum -= month_body_payment;
  }

  return response;
}
