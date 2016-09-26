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








b2 toolset=msvc-14.0 --with-serialization --with-thread --with-system --with-filesystem --with-log address-model=64 --stagedir=stage/debug -j4 link=static --layout=system variant=debug
mkdir lib\msvc-14-x64-Debug
move stage\lib\*.* lib\msvc-14-debug-64