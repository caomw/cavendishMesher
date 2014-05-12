
   RDM - Dessin - Maillage
   Calcul des Structures par la Méthode des Éléments Finis

   Version  - 6.17 - 29 mars 2011

   Utilisateur : Victor Lamoine

$debut du fichier
$version
6.17
$SI unites
$nom du fichier
rectangle_bug.geo
$date
30/11/2013
$points
10  nombre de points
   1  0.00000000000E+01  0.00000000000E+01 0  1.00
   2  0.00000000000E+01  1.50000000000E+01 0  1.00
   3  1.10000000000E+01  1.50000000000E+01 0  1.00
   4  2.00000000000E+01  3.00000000000E+01 0  1.00
   5  2.90000000000E+01  1.50000000000E+01 0  1.00
   6  4.00000000000E+01  1.50000000000E+01 0  1.00
   7  4.00000000000E+01  0.00000000000E+01 0  1.00
   8  3.00000000000E+01  0.00000000000E+01 0  1.00
   9  2.00000000000E+01  1.90000000000E+01 0  1.00
  10  1.00000000000E+01  0.00000000000E+01 0  1.00
$courbes
segment 1 2 101 0
segment 2 3 101 0
segment 3 4 101 0
segment 4 5 101 0
segment 5 6 101 0
segment 6 7 101 0
segment 7 8 101 0
segment 8 9 101 0
segment 9 10 101 0
segment 10 1 101 0
//// fin des courbes
$fenetre
-5.29514831067E+01  2.18117857752E+03 -5.29514831067E+01  1.05295148311E+03
$maillage
elements 800
lissage 3
jacobien 0.70
//// fin maillage
$fin du fichier
