#include "s21_smartcalc.c"

int main() {
  char* input = "14mod3^sin(x)";

  S* stack = NULL;
  get_lexem(&input, &stack);
  // validate(&stack);
  print_stack(&stack);
  return 0;
}