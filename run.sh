clear
printf "%s\n%s\n" "   λ   " "-------" 
g++ -Wall -pedantic list.c -fsanitize=address -g
./a.out
printf "%s\n%s\n" "-------" "  END  "
rm a.out;