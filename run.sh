clear
printf "%s\n%s\n" "START" "-------" 
g++ -Wall -pedantic list.c 
./a.out
printf "%s\n%s\n" "-------" "END"
rm a.out;