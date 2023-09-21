#include "s21_smartcalc.h"

int parser(char *str, int *count, S **stack) {
  int err = OK;

  char current_symbol = NULL_CHAR;
  S *lexem_stack = NULL;
  char *str_tmp = str;
  for (int i = 0; i < (int)strlen(str_tmp); i++) {
    int flag = 0;
    if (str_tmp[i] == COMMA) {
      str_tmp[i] = DOT;
    } else if (is_letter(str + i)) {
      if (i != 0) {
        if (is_letter(str + i - 1)) {
          flag = 1;
        }
      }
      if (flag == 0 && !is_letter(str + i + 1)) {
        if (str[i] == current_symbol || current_symbol == '\0') {
          current_symbol = str[i];
        } else {
          err = IncorrectExp;
          break;
        }
      }
    }
  }

  while (err == OK) {
    err = get_lexem(&str_tmp, &lexem_stack);
  }

  if (err == OK || err == End_of_str) {
    err = OK;
    reverse(&lexem_stack);
    *count = validate(&lexem_stack);
    if (*count == 0 || *count == 1) {
      err = polish(&lexem_stack);
      reverse(&lexem_stack);
    } else {
      err = IncorrectExp;
      remove_stack(&lexem_stack);
    }
  } else {
    err = IncorrectExp;
    remove_stack(&lexem_stack);
  }

  *stack = lexem_stack;

  return err;
}