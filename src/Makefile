# PROJECT= s21_smartcalc
# FLAGS= -Werror -Wextra -Wall -std=c11
# LEAKS_CALL=
# OS= $(shell uname -s)

# ifeq ($(OS), Darwin)
# LIBCHECK_FLAGS= -lcheck
# LEAKS= leaks --at-exit --
# endif
# ifeq ($(OS),Linux)
# LIBCHECK_FLAGS= -lcheck -lcheck_pic -pthread -lrt -lm -lsubunit
# LEAKS= valgrind --leak-check=full --show-leak-kinds=all --
# endif

# DIR_CHECK= @mkdir -p $(@D)
# BUILD_DIR= build
# TEST_DIR= s21_test
# TEST_BUILD_DIR= $(TEST_DIR)/$(BUILD_DIR)
# TEST_COMP_DIR= $(TEST_BUILD_DIR)/compilated
# COVERAGE_DIR= gcov_report

# SRC= $(wildcard ./*.c)
# OBJ= $(patsubst ./%.c, $(BUILD_DIR)/%.o, $(SRC))

# all: install

# install:
# 	cmake -S . -B build
# 	cmake --build build
# 	cp build/SmartCalc ./SmartCalc

# uninstall:
# 	@rm -rf SmartCalc

# gcov_report: test
# 	@mkdir -p $(COVERAGE_DIR)
# 	@rm -rf $(TEST_COMP_DIR)/*.gcno $(TEST_COMP_DIR)/*.gcda
# 	@gcov -f build/*.gcno
# 	@gcovr -r . --html-details --html-self-contained -o $(COVERAGE_DIR)/report.html
# 	@rm -f *.gcov

# test: $(TEST_COMP_DIR)/test.out
# 	$(LEAKS) ./$(TEST_COMP_DIR)/test.out

# $(TEST_COMP_DIR)/test.out: $(TEST_BUILD_DIR)/test.c $(PROJECT).a
# 	$(DIR_CHECK)
# 	@gcc $(FLAGS) $^ -o $@ $(LIBCHECK_FLAGS) $(PROJECT).a --coverage

# $(TEST_BUILD_DIR)/test.c: ./$(TEST_DIR)/test.check
# 	$(DIR_CHECK)
# 	@checkmk clean_mode=1 $^ > $@
# 	@$(LINTER) $@

# #Library
# $(PROJECT).a: $(OBJ)
# 	@ar -rsc $@ $^

# $(BUILD_DIR)/%.o: ./%.c
# 	$(DIR_CHECK)
# 	@gcc $(FLAGS) -c $< -o $@ --coverage

# dist: clean
# 	tar -czf SmartCalc.tgz ./*

# dvi:
# 	@open readme.pdf

# linter_format:
# 	@clang-format -style=Google -i $(SRC) *.h *.cpp

# linter_check:
# 	@clang-format -style=Google -n $(SRC) $(UTILS)

# clean:
# 	@rm -rf *.a *.out $(BUILD_DIR) *.gcov *.gcda *.gcno $(COVERAGE_DIR) SmartCalc $(TEST_BUILD_DIR) *.tgz

# leaks_check: LEAKS_CALL += $(LEAKS)
# leaks_check: test

# $(PROJECT_NAME).a: clean
# 	gcc -c $(FLAGS) $(PROJECT_NAME).c -o $(PROJECT_NAME).o
# 	ar rc $(PROJECT_NAME).a $(PROJECT_NAME).o
# 	ranlib $(PROJECT_NAME).a
	
# lib: clean $(PROJECT_NAME).a
# 	gcc $(FLAGS) main.c $(PROJECT_NAME).a
# 	./a.out 
PROJECT_NAME= s21_smartcalc
FLAGS= -Wall -Wextra -Werror
CHECKFL = $(shell pkg-config --cflags --libs check)

all: $(PROJECT_NAME).a

$(PROJECT_NAME).a: clean
	gcc -c $(FLAGS) $(PROJECT_NAME).c -o $(PROJECT_NAME).o
	ar rc $(PROJECT_NAME).a $(PROJECT_NAME).o
	ranlib $(PROJECT_NAME).a

test: $(PROJECT_NAME).a
	checkmk $(PROJECT_NAME).check > $(PROJECT_NAME)_test.c
	gcc $(FLAGS) $(PROJECT_NAME)_test.c $(PROJECT_NAME).a  -o s21_test $(CHECKFL)
	./s21_test

gcov_report:
	checkmk $(PROJECT_NAME).check > $(PROJECT_NAME)_test.c
	gcc -c $(FLAGS) $(PROJECT_NAME)_test.c -o $(PROJECT_NAME)_test.o
	gcc $(FLAGS) --coverage $(PROJECT_NAME)_test.o $(PROJECT_NAME).c  -o s21_test $(CHECKFL)
	./s21_test
	lcov -d ./ --capture --output-file s21_test.info
	genhtml s21_test.info --output-directory report
	open report/index.html

format: 
	clang-format -style=Google -i *.c *.h

style: 
	clang-format -style=Google -n *.c *.h

clean:
	rm -rf *.o *.a *.out s21_test $(PROJECT_NAME)_test.c *.gcda *.gcno report *.info

lib: clean $(PROJECT_NAME).a
	gcc $(FLAGS) main.c $(PROJECT_NAME).a -lm
	./a.out 
