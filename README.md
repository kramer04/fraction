# fraction
addition et soustraction de fraction

## pour compiler sous LINUX
g++ -g main.cpp src/*.cpp -std=c++20 -I include -o main -Wall -Wextra -O2

## Exemples
### Remarque importante : ne pas mettre d'espace

<pre>$ ./main
Addition soustraction de fraction
-5/8+9/7-10/3+4/7-1/2
-5/8+9/7-10/3+4/7-1/2 = -437/168
</pre>

<pre>$ ./main
Addition soustraction de fraction
2/5-6/9+11/8+12/17-15/19+12/7+1/3+1/4+1/8
2/5-6/9+11/8+12/17-15/19+12/7+1/3+1/4+1/8 = 467669/135660</pre>
