#ifndef S21_SMARTCALC_H_
#define S21_SMARTCALC_H_

#include <math.h>
#include <stdlib.h>
#include <string.h>

#define M_PI 3.14159265358979323846
#define EPSILON 0.000001

#define OK 0
#define IncorrectExp -1
#define IncorrectOper -2
#define IncorrectLexem -3
#define End_of_str 4

#define IncorrectType 1
#define IncorrectValue 2
#define IncorrectMatrix 3

#define COMMA ','
#define DOT '.'
#define NULL_CHAR '\0'

typedef enum {
  number = 1,
  variable = 2,
  oplus = 3,
  ominus = 4,
  omult = 5,
  odiv = 6,
  omod = 7,
  opower = 8,
  fcos = 9,
  fsin = 10,
  ftan = 11,
  facos = 12,
  fasin = 13,
  fatan = 14,
  fsqrt = 15,
  fln = 16,
  flog = 17,
  obracket = 18,
  cbracket = 19,
  start_S = -1,
  end_S = -2
} type_t;

typedef struct S S;

struct S {
  S *next;
  double value;
  int priority;
  type_t type;
};

int s21_smartcalc(char *str, double value, double *result);
int validate(S **stack);
int parser(char *str, int *count, S **stack);
int polish(S **stack);
int calculate(S **stack, double value, double *result);

// Helper
int is_digit(char *str);
int is_letter(char *str);
int check_priority(type_t type);
int get_lexem(char **str, S **stack);
int bush(S **stack_pointer, type_t type, int priority, double value);
int push(S **stack_head, type_t type, int priority, double value);
int pop(S **stack_head, S **stack_tmp);
int peek(S **stack_head, S **stack_tmp);
void reverse(S **stack);
void remove_stack(S **stack);

#endif  // S21_SMARTCAL_H_