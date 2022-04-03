all: control calc

control:
	g++ control.cpp -lzmq -o control.out -Wall
    
calc:
	g++ calc.cpp -lzmq -o calc.out -Wall

clean:
	rm -rf control.out calc.out
