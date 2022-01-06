all:
	mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make -j64

cleanbuild:
	rm -rf build && mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make -j64

# Create a development container
container:
	./tools/make_container.sh
# docker start -a -i rcdnsdev
