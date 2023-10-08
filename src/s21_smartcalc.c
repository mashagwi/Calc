#include "s21_smartcalc.h"

int s21_smartcalc(double *result, char *str, double value) {
  S *stack = NULL;
  int ret = OK;
  *result = 0;
  int variables_count = 0;
  ret = parser(str, &stack, &variables_count);
  if (ret == OK) {
    ret = calculate(&stack, value, result);
  } else {
    remove_stack(&stack);
  }
  return ret;
}

int validate(S **stack) {
  int ret = OK;
  S *current_node = *stack;
  int variable_count = 0;
  int open_bracket_count = 0;
  int close_bracket_count = 0;
  type_t old_type = start_S;

  while (ret == OK && current_node != NULL) {
    type_t cur_type = current_node->type;
    type_t next_type =
        (current_node->next != NULL) ? current_node->next->type : end_S;

    switch (cur_type) {
      case oplus:
      case omin:
        if (next_type == omin || next_type == oplus) {
          ret = IncorrectExp;
        } else if (old_type == start_S ||
                   (old_type >= omult && old_type <= opower) ||
                   old_type == obracket) {
          current_node->type = number;
          current_node->priority = check_priority(number);
          current_node->value = (cur_type == omin) ? -1 : 1;
          ret = bush(current_node, omult, 4, 0);
        }
        break;

      case osin:
      case olog:
        if (next_type != obracket) {
          ret = IncorrectExp;
        }
        break;

      case obracket:
        if (next_type >= omult && next_type <= opower) {
          ret = IncorrectExp;
        }
        open_bracket_count++;
        break;

      case cbracket:
        if (old_type >= oplus && old_type <= obracket) {
          ret = IncorrectExp;
        } else {
          close_bracket_count++;
        }
        break;

      case variable:
      case number:
        if (next_type == variable || next_type == number ||
            (next_type >= osin && next_type <= obracket)) {
          ret = bush(current_node, omult, check_priority(omult), 0);
        }
        if (cur_type == variable) {
          variable_count = 1;
        }
        break;
      default:
        break;
    }

    old_type = cur_type;
    current_node = current_node->next;
  }

  if (open_bracket_count != close_bracket_count) {
    ret = IncorrectExp;
  }

  if (ret == OK) {
    ret = variable_count;
  }

  return ret;
}

int parser(char *str, S **stack, int *count) {
  int ret = OK;

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
        if (str[i] == current_symbol || current_symbol == NULL_CHAR) {
          current_symbol = str[i];
        } else {
          ret = IncorrectExp;
          break;
        }
      }
    }
  }

  while (ret == OK) {
    ret = get_lexem(&str_tmp, &lexem_stack);
  }

  if (ret == OK || ret == End_of_str) {
    ret = OK;
    reverse_stack(&lexem_stack);
    *count = validate(&lexem_stack);
    if (*count == 0 || *count == 1) {
      ret = polish(&lexem_stack);
      reverse_stack(&lexem_stack);
    } else {
      ret = IncorrectExp;
      remove_stack(&lexem_stack);
    }
  } else {
    ret = IncorrectExp;
    remove_stack(&lexem_stack);
  }

  *stack = lexem_stack;

  return ret;
}

int polish(S **stack) {
  S *old_stack = *stack;
  S *new_stack = NULL;
  S *tmp_stack = NULL;
  int ret = OK;
  if (ret == OK) {
    while (old_stack != NULL) {
      S tmp;
      pop(&old_stack, &tmp);
      if (tmp.type <= variable) {
        push(&new_stack, tmp.type, tmp.priority, tmp.value);
      } else if (tmp.type >= osin && tmp.type <= obracket) {
        push(&tmp_stack, tmp.type, tmp.priority, tmp.value);
      } else if (tmp.type >= oplus && tmp.type <= opower) {
        while (tmp_stack != NULL) {
          if ((tmp_stack->priority > tmp.priority &&
               tmp_stack->priority != check_priority(obracket)) ||
              (tmp_stack->priority == tmp.priority &&
               (tmp.type == omin ||
                (tmp.type >= odiv && tmp.type <= opower)))) {
            S tmp2;
            pop(&tmp_stack, &tmp2);
            push(&new_stack, tmp2.type, tmp2.priority, tmp2.value);
          } else {
            break;
          }
        }
        push(&tmp_stack, tmp.type, tmp.priority, tmp.value);

      } else if (tmp.type == cbracket) {
        while (tmp_stack != NULL) {
          if (tmp_stack->type == obracket) {
            break;
          } else {
            S tmp2;
            pop(&tmp_stack, &tmp2);
            push(&new_stack, tmp2.type, tmp2.priority, tmp2.value);
          }
        }
        if (tmp_stack == NULL) {
          ret = IncorrectExp;
          break;
        } else {
          S tmp2;
          pop(&tmp_stack, &tmp2);
          if (tmp2.type >= osin && tmp2.type <= olog) {
            pop(&tmp_stack, &tmp2);
            push(&new_stack, tmp2.type, tmp2.priority, tmp2.value);
          }
        }
      }
    }
  }
  if (NULL != tmp_stack) {
    if (tmp_stack->type == obracket) {
      ret = IncorrectExp;
    } else {
      while (tmp_stack != NULL) {
        S tmp2;
        pop(&tmp_stack, &tmp2);
        push(&new_stack, tmp2.type, tmp2.priority, tmp2.value);
      }
    }
  }
  if (ret != OK) {
    remove_stack(stack);
    remove_stack(&new_stack);
    remove_stack(&tmp_stack);
  }
  *stack = new_stack;
  return ret;
}

int calculate(S **stack, double value, double *result) {
  int ret = OK;
  S *tmp_stack = NULL;
  S *old_stack = *stack;
  double tmp_result = 0;
  S tmp, var1, var2;

  while (old_stack != NULL && ret == OK) {
    pop(&old_stack, &tmp);

    if (tmp.type == variable) {
      tmp.type = number;
      tmp.value = value;
    }

    switch (tmp.type) {
      case number:
      case variable:
        ret = push(&tmp_stack, tmp.type, tmp.priority, tmp.value);
        break;

      case oplus:
      case omin:
      case omult:
      case odiv:
      case omod:
      case opower:
        ret = pop(&tmp_stack, &var2);
        if (ret == OK) {
          ret = pop(&tmp_stack, &var1);
          if (ret == OK) {
            switch (tmp.type) {
              case oplus:
                tmp_result = var1.value + var2.value;
                break;

              case omin:
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
                ret = IncorrectExp;
                break;
            }
            ret = push(&tmp_stack, number, check_priority(number), tmp_result);
          } else {
            ret = IncorrectExp;
          }
        } else {
          ret = IncorrectExp;
        }
        break;

      case osin:
      case ocos:
      case otg:
      case oasin:
      case oacos:
      case oatg:
      case osqrt:
      case oln:
      case olog:
        ret = pop(&tmp_stack, &var1);
        if (ret == OK) {
          switch (tmp.type) {
            case osin:
              tmp_result = sin(var1.value);
              break;

            case ocos:
              tmp_result = cos(var1.value);
              break;

            case otg:
              tmp_result = sin(var1.value) / cos(var1.value);
              break;

            case oasin:
              tmp_result = asin(var1.value);
              break;

            case oacos:
              tmp_result = acos(var1.value);
              break;

            case oatg:
              tmp_result = atan(var1.value);
              break;

            case osqrt:
              tmp_result = sqrt(var1.value);
              break;

            case oln:
              tmp_result = log(var1.value);
              break;

            case olog:
              tmp_result = log10(var1.value);
              break;

            default:
              ret = IncorrectExp;
              break;
          }
          ret = push(&tmp_stack, number, check_priority(number), tmp_result);
        } else {
          ret = IncorrectExp;
        }
        break;

      default:
        ret = IncorrectExp;
        break;
    }
  }

  if (ret == OK && tmp_stack != NULL) {
    S tmp;
    ret = pop(&tmp_stack, &tmp);
    if (ret == OK && tmp_stack == NULL) {
      *result = tmp.value;
      *stack = NULL;
    } else {
      ret = IncorrectExp;
    }
  } else {
    ret = IncorrectExp;
  }

  remove_stack(&old_stack);
  remove_stack(&tmp_stack);

  return ret;
}

int get_lexem(char **str, S **stack) {
  int ret = OK;
  char *current_char = *str;
  S *stack_head = *stack;
  int priority = 0;
  char char_flag = NULL_CHAR;

  if (*current_char == NULL_CHAR) {
    ret = End_of_str;
  } else {
    while (*current_char == ' ' && ret == OK) {
      current_char++;
    }

    if (is_digit(current_char)) {
      char *end_ptr = current_char;
      double value = strtod(current_char, &end_ptr);
      current_char = end_ptr;
      priority = check_priority(number);
      ret = push(&stack_head, number, priority, value);
      if (ret) {
        remove_stack(&stack_head);
      }
    } else if (is_letter(current_char)) {
      if (strncmp(current_char, "mod", 3) == 0) {
        priority = check_priority(omod);
        ret = push(&stack_head, omod, priority, 0);
        current_char += 3;
      } else if (strncmp(current_char, "sin", 3) == 0) {
        priority = check_priority(osin);
        ret = push(&stack_head, osin, priority, 0);
        current_char += 3;
      } else if (strncmp(current_char, "cos", 3) == 0) {
        priority = check_priority(ocos);
        ret = push(&stack_head, ocos, priority, 0);
        current_char += 3;
      } else if (strncmp(current_char, "tg", 2) == 0) {
        priority = check_priority(otg);
        ret = push(&stack_head, otg, priority, 0);
        current_char += 2;
      } else if (strncmp(current_char, "asin", 4) == 0) {
        priority = check_priority(oasin);
        ret = push(&stack_head, oasin, priority, 0);
        current_char += 4;
      } else if (strncmp(current_char, "acos", 4) == 0) {
        priority = check_priority(oacos);
        ret = push(&stack_head, oacos, priority, 0);
        current_char += 4;
      } else if (strncmp(current_char, "atg", 3) == 0) {
        priority = check_priority(oatg);
        ret = push(&stack_head, oatg, priority, 0);
        current_char += 3;
      } else if (strncmp(current_char, "sqrt", 4) == 0) {
        priority = check_priority(osqrt);
        ret = push(&stack_head, osqrt, priority, 0);
        current_char += 4;
      } else if (strncmp(current_char, "ln", 2) == 0) {
        priority = check_priority(oln);
        ret = push(&stack_head, oln, priority, 0);
        current_char += 2;
      } else if (strncmp(current_char, "log", 3) == 0) {
        priority = check_priority(olog);
        ret = push(&stack_head, olog, priority, 0);
        current_char += 3;
      } else if (!is_letter(current_char + 1)) {
        if (char_flag == '\0' || char_flag == *current_char) {
          priority = check_priority(variable);
          ret = push(&stack_head, variable, priority, 0);
          current_char++;
        } else {
          ret = IncorrectExp;
        }
      }
    } else {
      switch (*current_char) {
        case '+':
          priority = check_priority(oplus);
          ret = push(&stack_head, oplus, priority, 0);
          break;

        case '-':
          priority = check_priority(omin);
          ret = push(&stack_head, omin, priority, 0);
          break;

        case '*':
          priority = check_priority(omult);
          ret = push(&stack_head, omult, priority, 0);
          break;

        case '/':
          priority = check_priority(odiv);
          ret = push(&stack_head, odiv, priority, 0);
          break;

        case '^':
          priority = check_priority(opower);
          ret = push(&stack_head, opower, priority, 0);
          break;

        case '(':
          priority = check_priority(obracket);
          ret = push(&stack_head, obracket, priority, 0);
          break;

        case ')':
          priority = check_priority(cbracket);
          ret = push(&stack_head, cbracket, priority, 0);
          break;

        default:
          ret = IncorrectExp;
          break;
      }
      current_char++;
      if (ret) {
        remove_stack(&stack_head);
      }
    }
  }
  *stack = stack_head;
  *str = current_char;
  return ret;
}

int check_priority(type_t type) {
  int response = -1;
  if ((type < number) || (type > cbracket)) {
    response = -1;
  } else if (type <= number) {
    response = 0;
  } else if (type <= omin) {
    response = 1;
  } else if (type <= omod) {
    response = 2;
  } else if (type <= opower) {
    response = 3;
  } else if (type <= olog) {
    response = 4;
  } else if (type <= cbracket) {
    response = 5;
  }

  return response;
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

// void print_stack(S **stack) {
//   S *current = *stack;
//   while (current != NULL) {
//     printf("Type:%d", current->type);
//     if (current->type == number) printf("number:%f", current->value);
//     printf("\n");

//     current = current->next;
//   }
// }

int push(S **stack, type_t type, int priority, double value) {
  // Проверка корректности типа элемента
  if (type < number || type > cbracket) {
    return IncorrectType;
  }

  // Выделение памяти под новый элемент
  S *new_element = calloc(1, sizeof(S));
  if (new_element == NULL) {
    return MEMORY_ALLOCATION_ERROR;
  }

  // Инициализация нового элемента
  new_element->priority = priority;
  new_element->type = type;
  new_element->value = value;

  // Добавление нового элемента в стек
  new_element->next = *stack;
  *stack = new_element;

  return OK;
}

int pop(S **stack, S *Smp) {
  // Проверка наличия элементов в стеке
  if (*stack == NULL) {
    return POINTER_TO_NULL;
  }

  // Извлечение значения из вершины стека
  S *stack_head = *stack;
  Smp->priority = stack_head->priority;
  Smp->type = stack_head->type;
  Smp->value = stack_head->value;

  // Удаление верхнего элемента стека и обновление указателя на вершину стека
  *stack = stack_head->next;
  free(stack_head);

  // Очистка ссылки на следующий элемент в буфере
  Smp->next = NULL;

  return OK;
}

int bush(S *stack, type_t type, int priority, double value) {
  // Проверка корректности указателя на элемент стека
  if (stack == NULL) {
    return POINTER_TO_NULL;
  }

  // Проверка корректности типа элемента
  if (type < number || type > cbracket) {
    return IncorrectType;
  }

  // Проверка корректности значения
  if (isnan(value) || isinf(value)) {
    return IncorrectType;
  }

  // Выделение памяти под новый элемент
  S *new_element = calloc(1, sizeof(S));
  if (new_element == NULL) {
    return MEMORY_ALLOCATION_ERROR;
  }

  // Инициализация нового элемента
  new_element->priority = priority;
  new_element->type = type;
  new_element->value = value;

  // Вставка нового элемента в стек
  new_element->next = stack->next;
  stack->next = new_element;

  return OK;
}

void remove_stack(S **stack) {
  S *stack_head = *stack;
  while (NULL != stack_head) {
    S *next = stack_head->next;
    free(stack_head);
    stack_head = next;
  }
  *stack = stack_head;
}

void reverse_stack(S **stack) {
  S *tmp_stack = NULL;
  while (*stack != NULL) {
    S tmp_value;
    pop(stack, &tmp_value);
    push(&tmp_stack, tmp_value.type, tmp_value.priority, tmp_value.value);
  }
  *stack = tmp_stack;
}
