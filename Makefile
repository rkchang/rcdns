.PHONY : all debug debugger release tsan container clean

all: debug relase tsan fuzz coverage container

debug:
	mkdir build_debug && cd build_debug && cmake -G Ninja -DABSL_PROPAGATE_CXX_STD=ON -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Debug .. && ninja

debugger:
	mkdir build_debugger && cd build_debugger && cmake -G Ninja -DABSL_PROPAGATE_CXX_STD=ON -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Debugger .. && ninja 

release:
	mkdir build_release && cd build_release && cmake -G Ninja -DABSL_PROPAGATE_CXX_STD=ON -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release .. && ninja 

tsan:
	mkdir build_tsan && cd build_tsan && cmake -G Ninja  -DABSL_PROPAGATE_CXX_STD=ON -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Tsan .. && ninja 

# Create a development container
container:
	./tools/make_container.sh

clean:
	rm -rf build*

# docker start -a -i rcdnsdev
