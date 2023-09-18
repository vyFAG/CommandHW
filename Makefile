all:
	g++ -std=c++17 CommandHW.h -o CommandHW
	g++ -std=c++17 CommandHW.cpp -o CommandHW
	g++ -std=c++17 Test/CommandHWTest.cpp -o CommandHWTest

test:
	chmod +x CommandHW
	./Test/CommandHWTest

clean:
	$(RM) CommandHW CommandHWTest