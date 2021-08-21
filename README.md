# fraction
Donne le résultat sous forme simplifiée de l'addition et la soustraction d'une série de fractions composées de nombres entiers.

## pour compiler sous LINUX
```
g++ -g main.cpp src/*.cpp -std=c++20 -I include -o main -Wall -Wextra -O2
```

## Exemples
### Remarque importante : ne pas mettre d'espace

<pre>$ ./main
Addition soustraction de fraction
-5/8+9/7-10/3+4/7-1/2
--------------------------
Résultat :
-437/168
--------------------------
</pre>

<pre>$ ./main
Addition soustraction de fraction
2/5-6/9+11/8+12/17-15/19+12/7+1/3+1/4+1/8
--------------------------
Résultat :
467669/135660
--------------------------
</pre>

<pre>$ ./main
Addition soustraction de fraction
-5/8-9/6
--------------------------
Résultat :
-17/8
--------------------------
</pre>
