#include "s21_smartcalc.h"

int s21_smartcalc(char *str, double value, double *result) {
  int err = OK;
  int count = 0;
  *result = 0;

  S *stack = NULL;

  err = parser(str, &stack, &count);
  if (err == OK) {
    err = calculate(&stack, value, result);
  } else {
    remove_stack(&stack);
  }
  return err;
}