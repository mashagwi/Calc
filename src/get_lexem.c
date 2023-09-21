#include "s21_smartcalc.h"

int get_lexem(char **str, S **stack) {
  int err = OK;
  S *stack_result = *stack;
  char *tmp = *str;
  int priority = 0;
  char flag = NULL_CHAR;
  if (*tmp = NULL_CHAR) {
    err = End_of_str;
  } else {
    while (err == OK && *tmp == ' ') {
      tmp++;
    }
    if (is_digit(tmp)) {
      char *end_tmp = tmp;
      double value = strtod(tmp, &end_tmp);
      tmp = end_tmp;
      priority = check_priority(number);
      err = push(&stack_result, number, priority, value);
      if (err) {
        remove_stack(&stack_result);
      }
    } else if (is_letter(tmp)) {
      if (strncmp(tmp, "mod", 3) == 0) {
        priority = get_priority(omod);
        response = push_back(&stack_result, omod, priority, 0);
        tmp += 3;
      } else if (strncmp(tmp, "sin", 3) == 0) {
        priority = get_priority(osin);
        response = push_back(&stack_result, fsin, priority, 0);
        tmp += 3;
      } else if (strncmp(tmp, "cos", 3) == 0) {
        priority = get_priority(ocos);
        response = push_back(&stack_result, fcos, priority, 0);
        tmp += 3;
      } else if (strncmp(tmp, "tg", 2) == 0) {
        priority = get_priority(otg);
        response = push_back(&stack_result, ftg, priority, 0);
        tmp += 2;
      } else if (strncmp(tmp, "asin", 4) == 0) {
        priority = get_priority(oasin);
        response = push_back(&stack_result, fasin, priority, 0);
        tmp += 4;
      } else if (strncmp(tmp, "acos", 4) == 0) {
        priority = get_priority(oacos);
        response = push_back(&stack_result, facos, priority, 0);
        tmp += 4;
      } else if (strncmp(tmp, "atg", 3) == 0) {
        priority = get_priority(oatg);
        response = push_back(&stack_result, fatg, priority, 0);
        tmp += 3;
      } else if (strncmp(tmp, "sqrt", 4) == 0) {
        priority = get_priority(osqrt);
        response = push_back(&stack_result, fsqrt, priority, 0);
        tmp += 4;
      } else if (strncmp(tmp, "ln", 2) == 0) {
        priority = get_priority(oln);
        response = push_back(&stack_result, fln, priority, 0);
        tmp += 2;
      } else if (strncmp(tmp, "log", 3) == 0) {
        priority = get_priority(olog);
        response = push_back(&stack_result, flog, priority, 0);
        tmp += 3;
      } else if (!is_letter(tmp + 1)) {
        if (flag == NULL_CHAR || flag == *tmp) {
          priority = get(variable);
          err = push(&stack_result, variable, priority, 0);
          tmp++;
        } else {
          err = IncorrectExp;
        }
      }
    } else {
      switch (*tmp) {
        case '+':
          priority = get_priority(oplus);
          break;
        case '-':
          priority = get_priority(omin);
          break;
        case '*':
          priority = get_priority(omult);
          break;
        case '/':
          priority = get_priority(odiv);
          break;
        case '^':
          priority = get_priority(opow);
          break;
        case '(':
          priority = get_priority(obracket);
          break;
        case ')':
          priority = get_priority(cbracket);
          break;
        default:
          response = IncorrectExp;
          break;
      }
      if (response == CORRECT) {
        response = push_back(&stack_result, (type_t)*tmp, priority, 0);
        tmp++;
        if (response) {
          free_stack(&stack_head);
        }
      }
    }
  }
  *stack = stack_result;
  *str = tmp;
  return err;
}