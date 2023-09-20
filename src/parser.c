#include "s21_smartcalc.h"

int parser(char *str, int *count, S **stack) {
  int err = OK;

  char symbol = '\0';
  S *tmp = NULL;
  char *str_tmp = str;
  for (int i = 0; i < (int)strlen(str); i++) {
    int flag = 0;
    if (str_tmp[i] == ',') {
      str_tmp[i] = '.';
    } else if (is_letter(str + i)) {
      if (i != 0) {
        if (is_letter(str + i - 1)) {
          flag = 1;
        }
      }
      if (flag == 0 && !is_letter(str + i + 1)) {
        if (str[i] == symbol || symbol == '\0') {
          symbol = str[i];
        } else {
          err = IncorrectExp;
          break;
        }
      }
    }
  }
  while (err == OK) {
    err = get_lexem(&str_tmp, &tmp);
  }
  if (err == OK || err == End_of_str) {
    err = OK;
    reverse(&tmp);
    *count = validate(&tmp);
    if (*count == 0 || *count == 1) {
      err = polish(&tmp);
      reverse(&tmp);
    } else {
      err = IncorrectExp;
      remove_stack(&tmp);
    }
  } else {
    err = IncorrectExp;
    remove_stack(&tmp);
  }
  *stack = tmp;

  return err;
}