
   RDM - Dessin - Maillage
   Calcul des Structures par la Méthode des Eléments Finis

   Version  - 6.14 - 22 Janvier 2002

   Utilisateur : Ecole Nationale d'Ingénieurs - BREST

$debut du fichier
$version
6.14
$SI unites
$nom du fichier
formeM.geo
$date
4/11/2013
$points
5
   1  0.00000000000E+01  0.00000000000E+01 0  1.00
   2  0.00000000000E+01  5.00000000000E+01 0  1.00
   3  2.50000000000E+01  2.50000000000E+01 0  1.00
   4  5.00000000000E+01  5.00000000000E+01 0  1.00
   5  5.00000000000E+01  0.00000000000E+01 0  1.00
$courbes
segment 1 2 101 0
segment 3 4 101 0
segment 4 5 101 0
segment 1 5 101 0
segment 2 3 101 0
//// fin
$fenetre
-8.10537E-01  2.31383E+01 -8.10537E-01  1.08105E+01
$maillage
elements 300
lissage 3
jacobien 0.70
////
$fin du fichier
