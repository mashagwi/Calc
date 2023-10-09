#ifndef S21_SMARTCALC_H_
#define S21_SMARTCALC_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EPSILON 0.000001

#define OK 0
#define IncorrectExp -1
#define IncorrectOper -2
#define End_of_str 4

#define IncorrectType 1
#define IncorrectValue 2
#define MEMORY_ALLOCATION_ERROR -3
#define POINTER_TO_NULL 1

#define COMMA ','
#define DOT '.'
#define NULL_CHAR '\0'

/** Список типов type_t лексем, определяет все возможные типы,
 * которые мы можем получить при анализе строки выражения.
 * Это включает числа, операторы (сложение, вычитание, умножение, деление,
 * возведение в степень, остаток от деления), фунции, а также открывающую и
 * закрывающую скобки.
 */
typedef enum {
  number = 1,
  variable = 2,
  oplus = 3,
  omin = 4,
  omult = 5,
  odiv = 6,
  omod = 7,
  opower = 8,
  osin = 9,
  ocos = 10,
  otg = 11,
  oasin = 12,
  oacos = 13,
  oatg = 14,
  osqrt = 15,
  oln = 16,
  olog = 17,
  obracket = 18,
  cbracket = 19,
  start_S = -1,
  end_S = -2
} type_t;

typedef struct S S;

/** Структура S используется для хранения информации о лексем. Она содержит ее
 * значение, тип, приоритет и указатель на следующую позицию. */

struct S {
  S *next;
  double value;
  int priority;
  type_t type;
};

int s21_smartcalc(double *result, char *string, double value);
/**
 * Функция для проверки корректности элементов стека
 */
int validate(S **stack);
/**
 * Функция парсинга строки
 */
int parser(char *str, S **stack, int *count);
/**
 * Конвертирует стек в польскую нотацию
 */
int polish(S **stack);
/**
 * Вычисляет выражение
 */
int calculate(S **stack, double value, double *result);

/**
 * Функций для работы со стеком:
 * bush - функция для добавления элемента в стек после указателя (для обработки
 * случая -1) push - функция для добавления элемента вверх стека pop - функция
 * для удаления и возврата последнего элемента стека remove_stack - для
 * освобождения памяти, выделенной под стек get_lexem - для получения следующей
 * лексемы из строки и добавления в стек
 */

int bush(S *stack, type_t type, int priority, double value);
int push(S **stack, type_t type, int priority, double value);
int pop(S **stack, S *stack_tmp);
int get_lexem(char **str, S **stack);
void remove_stack(S **stack);
void reverse_stack(S **stack);
int is_digit(char *str);
int is_letter(char *str);
int check_priority(type_t type);

#endif  // S21_SMARTCAL_H_