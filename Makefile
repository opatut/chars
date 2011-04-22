default: bake

go: bake run

goserver: bake server

bake:
	mkdir -p build 
	cd build; cmake ..; make -j3

gdb: bake
	cd bin; gdb chars

gdbserver: bake
	cd bin; gdb server

run:
	cd bin; echo; echo; ./chars

server:
	cd bin; echo; echo; ./server

#docs:
#	doxygen

clean:
	rm -r build
	rm bin/Impostor*
	#rm -r doc
