default: goserver

go: bake run

goserver: bake server

bake:
	mkdir -p build 
	cd build; cmake ..; make -j3

gdb:
	cd bin; gdb chars

run:
	cd bin; echo; echo; ./chars

server:
	cd bin; echo; echo; ./server

#docs:
#	doxygen

clean:
	rm -r build
	#rm -r doc
