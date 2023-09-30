PROJECT_NAME= s21_smartcalc
FLAGS= -Wall -Wextra -Werror
CHECKFL = $(shell pkg-config --cflags --libs check)
SRC = $(PROJECT_NAME).c credit.c deposit.c


all: $(PROJECT_NAME).a

install: clean build
	build; qmake ../SmartCalc/s21_smartcalc.pro && make && rm -f Makefile

uninstall:
	rm -rf build* 

$(PROJECT_NAME).a: clean
	gcc -c $(FLAGS) $(PROJECT_NAME).c -o $(PROJECT_NAME).o
	gcc -c $(FLAGS) credit.c -o credit.o
	gcc -c $(FLAGS) deposit.c -o deposit.o
	ar rc $(PROJECT_NAME).a $(PROJECT_NAME).o credit.o deposit.o
	ranlib $(PROJECT_NAME).a


test: $(PROJECT_NAME).a
	checkmk $(PROJECT_NAME).check > $(PROJECT_NAME)_test.c
	gcc $(FLAGS) $(SRC) $(PROJECT_NAME)_test.c $(PROJECT_NAME).a -o s21_test $(CHECKFL)
	./s21_test


gcov_report:
	checkmk $(PROJECT_NAME).check > $(PROJECT_NAME)_test.c
	gcc -c $(FLAGS) $(PROJECT_NAME)_test.c -o $(PROJECT_NAME)_test.o
	gcc $(FLAGS) --coverage $(PROJECT_NAME)_test.o $(SRC) -o s21_test $(CHECKFL)
	./s21_test
	lcov -d ./ --capture --output-file s21_test.info
	genhtml s21_test.info --output-directory report
	open report/index.html
	
dvi:
	doxygen Doxyfile

dist: clean
	tar -zcf s21_smartcalc.tgz ./*
	
clean:
	rm -rf *.o *.a *.out s21_test $(PROJECT_NAME)_test.c *.gcda *.gcno report *.info *.gz *.tgz html latex ../build

format: 
	clang-format -style=Google -i *.c *.h

style: 
	clang-format -style=Google -n *.c *.h



lib: clean $(PROJECT_NAME).a
	gcc $(FLAGS) main.c $(PROJECT_NAME).a -lm
	./a.out 
