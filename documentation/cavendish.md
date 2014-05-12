Cavendish Mesher
================
This document explains the method of Cavendish to mesh a convex/concave contour.
This version does not support holes nor complex shapes (= not a segment).
Of course the contour must not self intersect.

This method allow to create only triangle finite elements.

First step
----------
The first step is to measure all interior angles of the shape :

![AnglesImage](https://raw.githubusercontent.com/VictorLamoine/cavendishMesher/master/documentation/interior_exterior_angles.png)

On this picture the contour is the black shape. Note that interior angles can be greater than 180° !

Then find the smallest angle in the shape, there are three cases :
* Case 1 - Angle is inferior to 90°
* Case 2 - Angle is between 90° and 120°
* Case 3 - Angle is greater than 120°

Cavendish case 1
----------------

Simply joint the neighbours around the smallest angle to create one finite element:

![Case1](https://raw.githubusercontent.com/VictorLamoine/cavendishMesher/master/documentation/case1.png)

Update the contour

Cavendish case 2
----------------


![Case2](https://raw.githubusercontent.com/VictorLamoine/cavendishMesher/master/documentation/case2.png)
Update the contour

Cavendish case 3
----------------

![Case3](https://raw.githubusercontent.com/VictorLamoine/cavendishMesher/master/documentation/case3.png)
Update the contour

Last iteration
--------------
At some point the contour is composed of 4 segments. In that case do no apply cavendish cases;
compute the diagonal sizes and keep the smallest to create the two last elements.

RDM 6
=====
We use files from ![RDM6 software](http://iut.univ-lemans.fr/ydlogi/rdm_version_6.html)
; but we can't rely on the vector order.
So we have to re-order all the vectors before starting anything.
