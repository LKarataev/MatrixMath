CFLAGS = -std=c++2b -Wall -Wextra -Werror
CC = g++

all: clean build

build: matrix_math.a

matrix_math.a: matrix_math.o
	ar rcs $@ $^
	cp $@ lib$@

matrix_math.o:
	$(CC) -std=c++2b -c src/matrix_math.cc

tests_prepare: matrix_math.a
	$(CC) -lgtest -lmatrix_math $(CFLAGS) -o tests/tests tests/tests.cc 

tests: tests_prepare
	./tests/tests

style:
	clang-format --style=Google -n src/*.cc

cppcheck:
	cppcheck --enable=all --suppress=missingInclude --suppress=unusedFunction src/*.cc

leaks: tests_prepare
	CK_FORK=no leaks -atExit -- ./tests/tests

gcov_report:
	$(CC) --coverage -o tests/tests tests/tests.cc src/matrix_math.cc -lgtest -lmatrix_math $(CFLAGS)
	./tests/tests
	lcov --no-external --capture --directory ./ --output-file coverage.info
	genhtml coverage.info --output-directory gcov_report
	open ./gcov_report/index.html

clean:
	rm -rf tests/tests *.gcda *.gcno coverage.info .clang-format valgrind.txt gcov_report *.a *.o
