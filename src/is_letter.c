#include "s21_smartcalc.h"

int is_letter(char *str) {
  int res = 0;
  if (*str >= 'a' && *str <= 'z') res = 1;

  return res;
}