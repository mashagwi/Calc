#include "s21_smartcalc.h"

int get_lexem(char **str, S **stack) {
  int err = OK;
  S *stack_result = *stack;
  char *current_char = *str;
  int priority = 0;
  char previous_char = NULL_CHAR;

  while (err == OK && *current_char == ' ') {
    current_char++;
  }
  if (*current_char == NULL_CHAR) {
    err = End_of_str;
  } else {
    if (is_digit(current_char)) {
      char *end_char = current_char;
      double value = strtod(current_char, &end_current_char);
      current_char = end_char;
      priority = check_priority(number);
      err = push(&stack_result, number, priority, value);
      if (err) {
        remove_stack(&stack_result);
      }
    } else if (is_letter(current_char)) {
      struct Operation {
        const char *name;
        type_t type;
      };

      struct Operation operations[] = {
          {"mod", omod},   {"sin", fsin},   {"cos", fcos}, {"tg", ftg},
          {"asin", fasin}, {"acos", facos}, {"atg", fatg}, {"sqrt", fsqrt},
          {"ln", fln},     {"log", flog},
      };
      /* цикл for выполняет поиск среди операций (например, "mod",
       * "sin", "cos", и тд) в строке, начиная с текущей позиции, которая
       * указана переменной current_char. Цель - проверить, если текущая
       * подстрока (начиная с current_char) совпадает с какой-либо из известных
       * операций. Если совпадение найдено, то соответствующая операция
       * добавляется в стек, а current_char сдвигается на количество символов,
       * соответствующих операции. Затем цикл завершается с использованием
       * break.*/
      for (size_t i = 0; i < sizeof(operations) / sizeof(operations[0]); i++) {
        if (strncmp(current_char, operations[i].name,
                    strlen(operations[i].name)) == 0) {
          priority = get_priority(operations[i].type);
          err = push(&stack_result, operations[i].type, priority, 0);
          current_char += strlen(operations[i].name);
          break;
        }
      }

      if (err == OK && !is_letter(current_char + 1)) {
        if (previous_char == NULL_CHAR || previous_char == *current_char) {
          priority = get_priority(variable);
          err = push(&stack_result, variable, priority, 0);
          current_char++;
        } else {
          err = IncorrectExp;
        }
      }
      // if (strncmp(current_char, "mod", 3) == 0) {
      //   priority = check_priority(omod);
      //   response = push(&stack_result, omod, priority, 0);
      //   current_char += 3;
      // } else if (strncmp(current_char, "sin", 3) == 0) {
      //   priority = check_priority(osin);
      //   response = push(&stack_result, fsin, priority, 0);
      //   current_char += 3;
      // } else if (strncmp(current_char, "cos", 3) == 0) {
      //   priority = check_priority(ocos);
      //   response = push(&stack_result, fcos, priority, 0);
      //   current_char += 3;
      // } else if (strncmp(current_char, "tg", 2) == 0) {
      //   priority = check_priority(otg);
      //   response = push(&stack_result, ftg, priority, 0);
      //   current_char += 2;
      // } else if (strncmp(current_char, "asin", 4) == 0) {
      //   priority = check_priority(oasin);
      //   response = push(&stack_result, fasin, priority, 0);
      //   current_char += 4;
      // } else if (strncmp(current_char, "acos", 4) == 0) {
      //   priority = check_priority(oacos);
      //   response = push(&stack_result, facos, priority, 0);
      //   current_char += 4;
      // } else if (strncmp(current_char, "atg", 3) == 0) {
      //   priority = check_priority(oatg);
      //   response = push(&stack_result, fatg, priority, 0);
      //   current_char += 3;
      // } else if (strncmp(current_char, "sqrt", 4) == 0) {
      //   priority = check_priority(osqrt);
      //   response = push(&stack_result, fsqrt, priority, 0);
      //   current_char += 4;
      // } else if (strncmp(current_char, "ln", 2) == 0) {
      //   priority = check_priority(oln);
      //   response = push(&stack_result, fln, priority, 0);
      //   current_char += 2;
      // } else if (strncmp(current_char, "log", 3) == 0) {
      //   priority = check_priority(olog);
      //   response = push(&stack_result, flog, priority, 0);
      //   current_char += 3;
      // } else if (!is_letter(current_char + 1)) {
      //   if (previous_char == NULL_CHAR || previous_char == *current_char) {
      //     priority = get(variable);
      //     err = push(&stack_result, variable, priority, 0);
      //     current_char++;
      //   } else {
      //     err = IncorrectExp;
      //   }
      // }
    } else {
      switch (*current_char) {
        case '+':
          priority = check_priority(oplus);
          break;
        case '-':
          priority = check_priority(omin);
          break;
        case '*':
          priority = check_priority(omult);
          break;
        case '/':
          priority = check_priority(odiv);
          break;
        case '^':
          priority = check_priority(opow);
          break;
        case '(':
          priority = check_priority(obracket);
          break;
        case ')':
          priority = check_priority(cbracket);
          break;
        default:
          err = IncorrectExp;
          break;
      }
      if (err == OK) {
        err = push_back(&stack_result, (type_t)*current_char, priority, 0);
        current_char++;
        if (err) {
          free_stack(&stack_head);
        }
      }
    }
    previous_char = *current_char;
  }
  *stack = stack_result;
  *str = current_char;
  return err;
}