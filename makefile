target: raycast.o
	g++ raycast.o -o target

raycast.o: raycast.cc
	g++ -c raycast.cc

clean:
	rm *.o target