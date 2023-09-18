all:
	g++ -std=c++17 CommandHW.h -o ExceptionHW
	g++ -std=c++17 CommandHW.cpp -o ExceptionHW
	g++ -std=c++17 ./Test/CommandHWTest.cpp -o ExceptionHWTest

test:
	chmod +x CommandHW
	./Test/CommandHWTest

clean:
	$(RM) CommandHW CommandHWTest