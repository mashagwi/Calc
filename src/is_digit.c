int is_digit(char *str) {
  int res = 0;
  if (*str >= '0' && *str <= '9') res = 1;
  return res;
}