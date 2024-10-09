main: main.cpp
	g++ -std=c++11 main.cpp -o main
	chmod +x main

clean: main
	rm main

	
