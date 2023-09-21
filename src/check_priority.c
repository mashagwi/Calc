int check_priority(type_t type) {
  int res = -1;

  if (type < number || type > cbracket)
    res = -1;
  else if (type == number)
    res = 0;
  else if (type <= omin)
    res = 1;
  else if (type <= omod)
    res = 2;
  else if (type <= opow)
    res = 3;
  else if (type <= flog)
    res = 4;
  else if (type <= cbracket)
    res = 5;

  return res;
}