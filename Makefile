check:
	./homework1/linters/run.sh

build:
	cd homework1
	mkdir build
	cd build
	cmake ..
	cmake --build .

test:
	cd ..
	./homework1/build/tests/test_exchange