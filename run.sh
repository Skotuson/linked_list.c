clear
g++ -Wall -pedantic "$1" -fsanitize=address -g

if [ "$#" -eq 2 ]; then {
    .a/.out > "$2";
} else ./a.out;
fi
rm a.out;