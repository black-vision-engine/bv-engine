b2 toolset=msvc-11.0 --with-serialization --with-thread --with-system --with-filesystem --build-dir=tmp\msvc-11-debug-64 address-model=64 link=static threading=multi runtime-link=shared --layout=system variant=debug
mkdir lib\msvc-11-debug-64
move stage\lib\*.* lib\msvc-11-debug-64

b2 toolset=msvc-11.0 --with-serialization --with-thread --with-system --with-filesystem --build-dir=tmp\msvc-11-release-64 address-model=64 link=static threading=multi runtime-link=shared --layout=system variant=release
mkdir lib\msvc-11-release-64
move stage\lib\*.* lib\msvc-11-release-64

b2 toolset=msvc-11.0 --with-serialization --with-thread --with-system --with-filesystem --build-dir=tmp\msvc-11-debug-32 address-model=32 link=static threading=multi runtime-link=shared --layout=system variant=debug
mkdir lib\msvc-11-debug-32
move stage\lib\*.* lib\msvc-11-debug-32

b2 toolset=msvc-11.0 --with-serialization --with-thread --with-system --with-filesystem --build-dir=tmp\msvc-11-release-32 address-model=32 link=static threading=multi runtime-link=shared --layout=system variant=release
mkdir lib\msvc-11-release-32
move stage\lib\*.* lib\msvc-11-release-32
