clear
printf "%s\n%s\n" "START" "-------" 
g++ -Wall -pedantic list.c -fsanitize=address -g
./a.out
printf "%s\n%s\n" "-------" "END"
rm a.out;