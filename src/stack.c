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

int pop(S **stack, S *stack_tmp){
      // Проверка наличия элементов в стеке
  if (*stack == NULL) {
    return POINTER_TO_NULL;
  }

  // Извлечение значения из вершины стека
  S *stack_head = *stack;
  stack_tmp->priority = stack_head->priority;
  stack_tmp->type = stack_head->type;
  stack_tmp->value = stack_head->value;

  // Удаление верхнего элемента стека и обновление указателя на вершину стека
  *stack = stack_head->next;
  free(stack_head);
  
  // Очистка ссылки на следующий элемент в буфере
  stack_tmp->next = NULL;

  return OK;
}

int bush(S *stack, type_t type, int priority, double value){
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
  stack_t *new_element = calloc(1, sizeof(stack_t));
  if (new_element == NULL) {
    return MEMORY_ALLOCATION_ERROR;
  }

  // Инициализация нового элемента
  new_element->priority = priority;
  new_element->type = type;
  new_element->value = value;

  // Вставка нового элемента в стек
  new_element->next = stack_pointer->next;
  stack_pointer->next = new_element;

  return OK;
}

void remove_stack(S **stack){
S *stack_head = *stack;
  while (stack_head != NULL) {
    S *next = stack_head->next;
    free(stack_head);
    stack_head = next;
  }
}

void reverse_stack(S **stack){
    S *tmp_stack = NULL;
    while(*stack !=NULL) {
        // Извлекаем верхний элемент из исходного стека
        S *tmp = *stack;
        *stack = (*stack)->next;
        // Добавляем извлеченный элемент во временный стек
        tmp->next = tmp_stack;
        tmp_stack = tmp;
    }
    // Заменяем исходный стек на обратный
    *stack = tmp_stack;
}