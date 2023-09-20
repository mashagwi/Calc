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

typedef enum {
  number = 1,
  variable = 2,
  fcos = 3,
  fsin = 4,
  ftan = 5,
  facos = 6,
  fasin = 7,
  fatan = 8,
  fsqrt = 9,
  fln = 10,
  flog = 11,
  obracket = 12,
  cbracket = 13,
  oplus = 14,
  ominus = 15,
  omult = 16,
  odiv = 17,
  omod = 18,
  opower = 19,
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
int is_number(char *str);
int is_letter(char *str);
int get_priority(type_t type);
int get_lexem(char **str, S **stack);
int bush(S **stack_pointer, type_t type, int priority, double value);
int push(S **stack_head, type_t type, int priority, double value);
int pop(S **stack_head, S **stack_tmp);
int peek(S **stack_head, S **stack_tmp);
void reverse(S **stack);
void remove_stack(S **stack);

#endif  // S21_SMARTCAL_H_