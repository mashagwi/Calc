#include "s21_smartcalc.h"

int s21_smartcalc(char *str, double value, double *result) {
  int err = OK;
  int count = 0;
  *result = 0;
  S *stack = NULL;

  err = parser(str, &count, &stack);
  if (err == OK) {
    err = calculate(&stack, value, result);
  } else {
    remove_stack(&stack);
  }
  return err;
}

// int validate(S **stack) {
//   int err = OK;
//   int count = 0;
//   S *tmp = *stack;
//   int obracket_count = 0;
//   int cbracket_count = 0;
//   type_t old_type = start_S;
//   type_t cur_type = 0;
//   type_t next_type = 0;

//   while (err == OK && tmp != NULL) {
//     cur_type = tmp->type;
//     if (tmp->next != NULL)
//       next_type = tmp->next->type;
//     else
//       next_type = end_S;
//     if (next_type == end_S && (cur_type >= oplus && cur_type <= obracket)) {
//       err = IncorrectExp;
//       break;
//     } else if (cur_type == oplus || cur_type == ominus) {
//       if (next_type == ominus || next_type == oplus) {
//         err = IncorrectExp;
//         break;
//       } else if (old_type == start_S ||
//                  (old_type >= omult && old_type <= opower) ||
//                  old_type == obracket) {
//         tmp->type = number;
//         tmp->priority = check_priority(number);
//         if (cur_type == ominus)
//           tmp->value = -1;
//         else
//           tmp = 1;
//         err = bush(tmp, omult, 4, 0);
//       }
//     } else if (cur_type >= fsin && cur_type <= flog && next_type != obracket) {
//       err = IncorrectExp;
//       break;
//     } else if (cur_type == obracket) {
//       if (next_type >= omult && next_type <= opower) {
//         err = IncorrectExp;
//         break;
//       }
//       obracket_count++;
//     } else if (cur_type == cbracket) {
//       if (old_type >= oplus && old_type <= obracket) {
//         err = IncorrectExp;
//         break;
//       } else
//         cbracket++;
//     } else if (cur_type == variable || cur_type == number) {
//       if (next_type == variable || next_type == number ||
//           (next_type >= fsin && next_type <= obracket)) {
//         err = bush(tmp, omult, check_priority(omult), 0);
//       }
//       if (cur_type == number) count = 1;
//     }
//     old_type = tmp->type;
//     tmp = tmp->next;
//   }
//   if (obracket != cbracket) err = IncorrectExp;
//   if (err == OK) err = count;
//   return err;
// }

int validate(S **stack) {
    int err = OK;
    S *current_node = *stack;
    int variable_count = 0;
    int open_bracket_count = 0;
    int close_bracket_count = 0;
    type_t old_type = start_S;

    while (err == OK && current_node != NULL) {
        type_t cur_type = current_node->type;
        type_t next_type = (current_node->next != NULL) ? current_node->next->type : end_S;

        switch (cur_type) {
            case oplus:
            case ominus:
                if (next_type == ominus || next_type == oplus) {
                    err = IncorrectExp;
                } else if (old_type == start_S || (old_type >= omult && old_type <= opower) || old_type == obracket) {
                    current_node->type = number;
                    current_node->priority = check_priority(number);
                    current_node->value = (cur_type == ominus) ? -1 : 1;
                    err = bush(current_node, omult, 4, 0);
                }
                break;

            case fsin:
            case flog:
                if (next_type != obracket) {
                    err = IncorrectExp;
                }
                break;

            case obracket:
                if (next_type >= omult && next_type <= opower) {
                    err = IncorrectExp;
                }
                open_bracket_count++;
                break;

            case cbracket:
                if (old_type >= oplus && old_type <= obracket) {
                    err = IncorrectExp;
                } else {
                    close_bracket_count++;
                }
                break;

            case variable:
            case number:
                if (next_type == variable || next_type == number || (next_type >= fsin && next_type <= obracket)) {
                    err = bush(current_node, omult, check_priority(omult), 0);
                }
                if (cur_type == number) {
                    variable_count = 1;
                }
                break;
            // case fcos:
            // case ftan:
            // case facos:
            // case fasin:
            // case fatan:
            // case fsqrt:
            // case fln:
            // case start_S:
            // case end_S:
            //     break;
            default:
                break;    
        }

        old_type = cur_type;
        current_node = current_node->next;
    }

    if (open_bracket_count != close_bracket_count) {
        err = IncorrectExp;
    }

    if (err == OK) {
        err = variable_count;
    }

    return err;
}

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
    reverse_stack(&lexem_stack);
    *count = validate(&lexem_stack);
    if (*count == 0 || *count == 1) {
      err = polish(&lexem_stack);
      reverse_stack(&lexem_stack);
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

int calculate(S **stack, double value, double *result){
  int err = OK;
  S *tmp_stack = NULL;
  S *old_stack = *stack;
  double tmp_result = 0;
  S tmp, var1, var2;

    while (old_stack != NULL && err == OK) {
        pop(&old_stack, &tmp);

        if (tmp.type == variable) {
            tmp.type = number;
            tmp.value = value;
        }

        switch (tmp.type) {
            case number:
            case variable:
                err = push(&tmp_stack, tmp.type, tmp.priority, tmp.value);
                break;

            case oplus:
            case ominus:
            case omult:
            case odiv:
            case omod:
            case opower:
                err = pop(&tmp_stack, &var2);
                if (err == OK) {
                    err = pop(&tmp_stack, &var1);
                    if (err == OK) {
                        switch (tmp.type) {
                            case oplus:
                                tmp_result = var1.value + var2.value;
                                break;

                            case ominus:
                                tmp_result = var1.value - var2.value;
                                break;

                            case omult:
                                tmp_result = var1.value * var2.value;
                                break;

                            case odiv:
                                tmp_result = var1.value / var2.value;
                                break;

                            case omod:
                                tmp_result = fmod(var1.value, var2.value);
                                break;

                            case opower:
                                tmp_result = pow(var1.value, var2.value);
                                break;

                            default:
                                err = IncorrectExp;
                                break;
                        }
                        err = push(&tmp_stack, number, check_priority(number), tmp_result);
                    } else {
                        err = IncorrectExp;
                    }
                } else {
                    err = IncorrectExp;
                }
                break;

            case fsin:
            case fcos:
            case ftan:
            case fasin:
            case facos:
            case fatan:
            case fsqrt:
            case fln:
            case flog:
                err = pop(&tmp_stack, &var1);
                if (err == OK) {
                    switch (tmp.type) {
                        case fsin:
                            tmp_result = sin(var1.value);
                            break;

                        case fcos:
                            tmp_result = cos(var1.value);
                            break;

                        case ftan:
                            tmp_result = sin(var1.value) / cos(var1.value);
                            break;

                        case fasin:
                            tmp_result = asin(var1.value);
                            break;

                        case facos:
                            tmp_result = acos(var1.value);
                            break;

                        case fatan:
                            tmp_result = atan(var1.value);
                            break;

                        case fsqrt:
                            tmp_result = sqrt(var1.value);
                            break;

                        case fln:
                            tmp_result = log(var1.value);
                            break;

                        case flog:
                            tmp_result = log10(var1.value);
                            break;

                        default:
                            err = IncorrectExp;
                            break;
                    }
                    err = push(&tmp_stack, number, check_priority(number), tmp_result);
                } else {
                    err = IncorrectExp;
                }
                break;

            default:
                err = IncorrectExp;
                break;
        }
    }

    if (err == OK && tmp_stack != NULL) {
        S tmp;
        err = pop(&tmp_stack, &tmp);
        if (err == OK && tmp_stack == NULL) {
            *result = tmp.value;
            *stack = NULL;
        } else {
            err = IncorrectExp;
        }
    } else {
        err = IncorrectExp;
    }

    remove_stack(&old_stack);
    remove_stack(&tmp_stack);

    return err;
}



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
      double value = strtod(current_char, &end_char);
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
          {"+", oplus},    {"-", ominus},   {"*", omult},   {"/", odiv},
          {"mod", omod},   {"sin", fsin},   {"cos", fcos},  {"tg", ftan},
          {"asin", fasin}, {"acos", facos}, {"atg", fatan}, {"sqrt", fsqrt},
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
          priority = check_priority(operations[i].type);
          err = push(&stack_result, operations[i].type, priority, 0);
          current_char += strlen(operations[i].name);
          break;
        }
      }

      if (err == OK && !is_letter(current_char + 1)) {
        if (previous_char == NULL_CHAR || previous_char == *current_char) {
          priority = check_priority(variable);
          err = push(&stack_result, variable, priority, 0);
          current_char++;
        } else {
          err = IncorrectExp;
        }
      }
      // if (strncmp(current_char, "mod", 3) == 0) {
      //   priority = check_priority(omod);
      //   err = push(&stack_result, omod, priority, 0);
      //   current_char += 3;
      // } else if (strncmp(current_char, "sin", 3) == 0) {
      //   priority = check_priority(fsin);
      //   err = push(&stack_result, fsin, priority, 0);
      //   current_char += 3;
      // } else if (strncmp(current_char, "cos", 3) == 0) {
      //   priority = check_priority(fcos);
      //   err = push(&stack_result, fcos, priority, 0);
      //   current_char += 3;
      // } else if (strncmp(current_char, "tg", 2) == 0) {
      //   priority = check_priority(ftan);
      //   err = push(&stack_result, ftg, priority, 0);
      //   current_char += 2;
      // } else if (strncmp(current_char, "asin", 4) == 0) {
      //   priority = check_priority(fasin);
      //   err = push(&stack_result, fasin, priority, 0);
      //   current_char += 4;
      // } else if (strncmp(current_char, "acos", 4) == 0) {
      //   priority = check_priority(facos);
      //   err = push(&stack_result, facos, priority, 0);
      //   current_char += 4;
      // } else if (strncmp(current_char, "atg", 3) == 0) {
      //   priority = check_priority(fatan);
      //   err = push(&stack_result, fatg, priority, 0);
      //   current_char += 3;
      // } else if (strncmp(current_char, "sqrt", 4) == 0) {
      //   priority = check_priority(fsqrt);
      //   err = push(&stack_result, fsqrt, priority, 0);
      //   current_char += 4;
      // } else if (strncmp(current_char, "ln", 2) == 0) {
      //   priority = check_priority(fln);
      //   err = push(&stack_result, fln, priority, 0);
      //   current_char += 2;
      // } else if (strncmp(current_char, "log", 3) == 0) {
      //   priority = check_priority(flog);
      //   err = push(&stack_result, flog, priority, 0);
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
          priority = check_priority(ominus);
          break;
        case '*':
          priority = check_priority(omult);
          break;
        case '/':
          priority = check_priority(odiv);
          break;
        case '^':
          priority = check_priority(opower);
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
        err = push(&stack_result, (type_t)*current_char, priority, 0);
        current_char++;
        if (err) {
          remove_stack(&stack_result);
        }
      }
    }
    previous_char = *current_char;
  }
  *stack = stack_result;
  *str = current_char;
  return err;
}

int check_priority(type_t type) {
  int res = -1;

  if (type < number || type > cbracket)
    res = -1;
  else if (type == number)
    res = 0;
  else if (type <= ominus)
    res = 1;
  else if (type <= omod)
    res = 2;
  else if (type <= opower)
    res = 3;
  else if (type <= flog)
    res = 4;
  else if (type <= cbracket)
    res = 5;

  return res;
}

int is_letter(char *str) {
  int res = 0;
  if (*str >= 'a' && *str <= 'z') res = 1;

  return res;
}

int is_digit(char *str) {
  int res = 0;
  if (*str >= '0' && *str <= '9') res = 1;
  return res;
}

void print_stack(S **stack) {
  S *current = *stack;
  while (current != NULL) {
    printf("Type:%d", current->type);
    if (current->type == number) printf("number:%f", current->value);
    printf("\n");

    current = current->next;
  }
}