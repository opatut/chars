default: bake_release

init_release:
	mkdir -p build

init_debug:
	mkdir -p build_dbg

bake_release: init_release
	cd build; cmake -DCMAKE_BUILD_TYPE=DEBUG ..; make -j3

bake_debug: init_debug
	cd build_dbg; cmake -DCMAKE_BUILD_TYPE=DEBUG ..; make -j3

run_client:
	cd bin; echo; echo; ./chars

run_server:
	cd bin; echo; echo; ./server

gdb_client:
	cd bin; gdb chars

gdb_server:
	cd bin; gdb server

go_client: bake_release run_client

go_server: bake_release run_server

debug_client: bake_release gdb_client

debug_server: bake_release gdb_server

# shortcuts
go: go_client
gdb: debug_client
run: run_client
srv: run_server

#docs:
#	doxygen

clean:
	rm -r build
	rm bin/Impostor*
	#rm -r doc
