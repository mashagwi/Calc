
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
