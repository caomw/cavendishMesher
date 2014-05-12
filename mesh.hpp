/**
 * \file mesh.hpp
 * \brief The main function.
 * \author Victor Lamoine
 * \version 0.1
 * \date may 2014
 */

#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <iomanip>
#include <cstring>

#include <fstream>
#include <sstream>
#include <string>

#include <list>
#include <math.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

/** \def SPEED
 * \brief Speed of mesh construction animation in milli seconds
 */
#define SPEED 400

/** \def CASE_1_CAVENDISH
 * \brief Limit (angle in degrees) for the first Cavendish case
 * If the angle is lower than CASE_1_CAVENDISH then case 1 is used
 * If the angle is greater than CASE_1_CAVENDISH then case 2 or 3 is used
 */
#define CASE_1_CAVENDISH 90		// Cavendish angle limit 1 (in degrees)

/** \def CASE_2_CAVENDISH
 * \brief Limit (angle in degrees) for the second Cavendish case
 * If the angle is lower than CASE_2_CAVENDISH (and greater than CASE_1_CAVENDISH) then case 2 is used
 * If the angle is greater than CASE_2_CAVENDISH case 3 is used
 */
#define CASE_2_CAVENDISH 120	// Cavendish angle limit 2 (in degrees)

/** \def ZOOM
 * \brief Arbitrary value for OpenCV window size
 */
#define ZOOM 450.0

/** Structure to define points coordinates
 */
typedef struct Point
{
    float coord_X; /**< x axis coordinate */
    float coord_Y; /**< y axis coordinate */
} Point;

/** Structure to define a segment
 */
typedef struct Segment
{
    std::list<Point>::iterator head; /**< iterator on head point */
    std::list<Point>::iterator tail; /**< iterator on tail point */
} Segment;

/** Structure to define a finite element (triangle)
 */
typedef struct fElem
{
    std::list<Point>::iterator p1; /**< iterator on point 1*/
    std::list<Point>::iterator p2; /**< iterator on point 2*/
    std::list<Point>::iterator p3; /**< iterator on point 3*/
} ElemF;

#endif
