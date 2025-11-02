g++ -shared gm5.cpp -g -fPIC -olibgm5.so
g++ main.cpp -L. -Wl,-rpath,'$ORIGIN' -lgm5 -o app


# gdb ./app
# bt
# b main
# b *main+20
# layout asm
# layout src
# info sharedlibrary
# ^x o

