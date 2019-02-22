#! /bin/bash

cd initial-reverse
echo reverse
gcc -o reverse ~/Projects/ostep-solutions/initial-reverse/reverse.c -Wall
./test-reverse.sh
cd ..

for p in wcat wgrep wzip wunzip; do
    echo $p
    cd initial-utilities/$p
    gcc -o $p ~/Projects/ostep-solutions/initial-utilities/$p/$p.c -Wall
    ./test-$p.sh
    cd ../..
done

echo wish
cd processes-shell
gcc -o wish ~/Projects/ostep-solutions/processes-shell/wish/wish.c -Wall
./test-wish.sh
cd ..


