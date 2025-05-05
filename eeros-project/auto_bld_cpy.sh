cmake -S src -B build
cmake --build build
cmake --install build

scp ./build/first_proj_sh root@192.168.137.71:/home/root/
