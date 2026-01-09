![blablateam](https://user-images.githubusercontent.com/29018157/121679401-95be4500-cab8-11eb-9115-1afa99deb063.png)

# fraction
Donne le résultat sous forme simplifiée de l'addition, la soustraction, la multiplication et la division d'une série de fractions composées de nombres entiers.

Expressions parenthésées.

Pour la mutltiplication, signes acceptés : x, X, *

## pour compiler sous LINUX
```
g++ -g main.cpp src/*.cpp -std=c++20 -I include -o main -Wall -Wextra -O2
```

## Exemples

<pre>
Entrer une expression:
-5/8+9/7-10/3+4/7-1/2
Resultat: -437/168
</pre>

<pre>
Entrer une expression:
2/5-6/9+11/8+12/17-15/19+12/7+1/3+1/4+1/8
Resultat: 467669/135660
</pre>

<pre>
Entrer une expression:
-5/8-9/6
Resultat: -17/8
</pre>

<pre>
Entrer une expression:
2(3.+1)
Erreur: Caractere invalide: '.' (col 3)
</pre>

<pre>
Entrer une expression:
6/2 + 15/5
Resultat: 6
</pre>

<pre>
Entrer une expression:
7/(8-1)
Resultat: 1
</pre>

<pre>
Entrer une expression:
(45-12*4)/(35-7*(5/8)
Erreur: Parenthese fermante ')' manquante (col 22)
</pre>

<pre>
Entrer une expression:
(45-12*4)/(35-7*(5/8))
Resultat: -24/245
</pre>

<pre>
Entrer une expression:
-2/5+7/8
Resultat: 19/40
</pre>

<pre>
Entrer une expression:
+2/5+7/8
Resultat: 51/40
</pre>

<pre>
Entrer une expression:
45*12
Resultat: 540
</pre>
