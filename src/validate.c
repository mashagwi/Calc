#include "s21_smartcalc.h"

int validate(S **stack) {
  int err = OK;
  int count = 0;
  S *tmp = *stack;
  int obracket_count = 0;
  int cbracket_count = 0;
  type_t old_type = start_S;
  type_t cur_type = 0;
  type_t next_type = 0;

  while (err == OK && tmp != NULL) {
    cur_type = tmp->type;
    if (tmp->next != NULL)
      next_type = tmp->next->type;
    else
      next_type = end_S;
  }
  return err;
}