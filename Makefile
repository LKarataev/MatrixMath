CXXFLAGS = -std=c++2b -Wall -Wextra -Werror
LDLIBS = -lgtest -lmatrix_math

all: clean build tests

build: matrix_math.a

matrix_math.a: matrix_math.o
	$(AR) rcs $@ $^
	cp $@ lib$@

matrix_math.o: src/matrix_math.cc
	$(CXX) $(CXXFLAGS) -c $<

tests: tests/tests
	./$<

tests/tests: tests/tests.cc matrix_math.a
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDLIBS)

style:
	clang-format --style=Google -n src/*.cc

cppcheck:
	cppcheck --enable=all --suppress=missingInclude --suppress=unusedFunction src/*.cc

leaks: tests_prepare
	CK_FORK=no leaks -atExit -- ./tests/tests

gcov_report:
	$(CXX) --coverage -o tests/tests tests/tests.cc src/matrix_math.cc $(LDLIBS) $(CXXFLAGS)
	./tests/tests
	lcov --no-external --capture --directory ./ --output-file coverage.info
	genhtml coverage.info --output-directory gcov_report
	open ./gcov_report/index.html

clean:
	rm -rf *.a *.o tests/tests coverage.info gcov_report
