#include "GeometryProcessing.hpp"

GeometryProcessing::GeometryProcessing ()
{
}

bool
GeometryProcessing::orderSegments (std::list<Segment> &segmentsInContour,
                                   std::list<Segment> &segmentsOriginal)
{
  // TODO: Check if segmentsInContour = segmentsOriginal, if yes return (false);

  segmentsInContour.clear ();
  segmentsInContour.push_back (segmentsOriginal.front ());  // Put the first segment of the file into the contour list

  // We now re-order all segments so that the tail of the first segment is the head of the segment segment etcetera.
  // This allow easier computation of the angles for the following steps !
  int count = 1;

  for (std::list<Segment>::iterator itContour = segmentsInContour.begin (); count < segmentsOriginal.size (); itContour++)
  {
    // Run through segmentsInContour while we did not finish segmentsOriginal
    // The segmentsInContour list is modified within this loop

    bool found = false;
    for (std::list<Segment>::iterator itFile = segmentsOriginal.begin (); itFile != segmentsOriginal.end () && found == false; itFile++)
    {
      // Fetch the segment in segmentsOriginal that has headin = tailination of the actual segmentsInContour
      // We must ignore the SegmgentInFile that has headin = tailination of segmentsInContour
      // Warning ! The vector in segmentsOriginal is not necessary in the right order ! Thus; values can be swapped
      if ( (*itContour).tail == (*itFile).tail)
      {
        if ( (*itContour).head != (*itFile).head)
        {
          swap ( (*itFile).head, (*itFile).tail);	// We need to swap values ! .tail et .head
          segmentsInContour.push_back (*itFile);	// Push back the segment in segmentsInContour
          found = true;
          //std::cout << "Push_back type 1" << std::endl;
        }
      }
      else if ( (*itContour).tail == (*itFile).head)
      {
        if ( (*itContour).head != (*itFile).tail)
        {
          segmentsInContour.push_back (*itFile);
          found = true;
          //std::cout << "Push_back type 3" << std::endl;
        }
      }
    }
    count++;	// Now, the next vector !
  }

  if (segmentsInContour.size () != segmentsOriginal.size ())
  {
    std::cout << "\nSomething went terribly wrong, we lost segments somewhere !" << std::endl;
    return (false);
  }
  return (true);
}

bool
GeometryProcessing::computeAngles (std::list<float> &anglesInContour,
                                   std::list<Segment> &segmentsInContour,
                                   bool invert,
                                   bool print)
{
  anglesInContour.clear ();  // Clear the list before beginning
  float p1x, p2x, p1y, p2y, p3x, p3y;
  int j = 0;

  for (std::list<Segment>::iterator it = segmentsInContour.begin (); it != segmentsInContour.end (); it++)
  {
    j++;

    p1x = (*it).head->coord_X;	// Head coordinates of the current segment
    p1y = (*it).head->coord_Y;
    p2x = (*it).tail->coord_X;	// Tail coordinates of the current segment
    p2y = (*it).tail->coord_Y;

    it++;
    if (it == segmentsInContour.end ())
    {
      it = segmentsInContour.begin ();
    }

    p3x = (*it).tail->coord_X;
    p3y = (*it).tail->coord_Y;

    if (j == segmentsInContour.size ())
    {
      it = segmentsInContour.end ();
    }
    it--;

    // Compute the angle between 2 segments (interior angle):
    // This is only correct if the segments are aligned clockwise
    float theta1 = atan2 (p1y - p2y, p1x - p2x);
    float theta2 = atan2 (p3y - p2y, p3x - p2x);
    float angle_rad = theta2 - theta1;			// Angle between the 2 segments
    float angle_degrees = angle_rad * 180 / M_PI;

    if (angle_rad <= 0)
    {
      angle_rad += M_PI;
      angle_degrees += 360;
    }

    // Compute if we measured outer/inner angles
    if (invert == false)
    {
      angle_rad = M_PI - angle_rad;
      angle_degrees = 360 - angle_degrees;
    }

    if (angle_degrees > 360 || angle_degrees < 0)
    {
      anglesInContour.clear ();  // Emtpy the list
      std::cout << "Error while computing an anlge between 2 segments" << std::endl;
      return (false);
    }

    anglesInContour.push_back (angle_degrees);  // Add into the list
    if (print)
    {
      std::cout << "Angle " << j << " = " << angle_degrees << std::endl;
    }  // Print in terminal
  }
  return (true);
}

void
GeometryProcessing::firstCavendishCase (int index,
                                        std::list<Segment> &segmentsInFile,
                                        std::list<Segment> &segmentsInContour,
                                        std::list<fElem> &fElemMesh)
{
  fElem fElem_temp;		// We are going to create 1 element

  std::list<Segment>::iterator it = segmentsInContour.begin ();
  advance (it, index - 1);  // Go to segment 'index'
  fElem_temp.p1 = (*it).head;
  fElem_temp.p2 = (*it).tail;

  std::list<Point>::iterator it_temp_head = (*it).head;

  it++;  // We are now at segment 'index+1'
  // If we are out of the segment list, go to the first segment
  if (it == segmentsInContour.end ())
  {
    it = segmentsInContour.begin ();
  }
  fElem_temp.p3 = (*it).tail;

  Segment Segment_temp;				// A new segment
  Segment_temp.head = it_temp_head;  // head = head of vector 'index'
  Segment_temp.tail = (*it).tail;		// tail = tail of vector 'index+1'
  fElemMesh.push_back (fElem_temp);	// Push the finite new element

  // Gç baci to the last segment of the list if we were on the beginning of the list
  if (it == segmentsInContour.begin ())
  {
    it = segmentsInContour.end ();
  }  // .end() is AFTER the last segment
  it--;  // On est au segment j

  segmentsInContour.insert (it, Segment_temp);  // insert new segment befre j
  segmentsInFile.push_back (Segment_temp);		// insert new segment at the end

  segmentsInContour.erase (it);	// Delete segment j
  it = segmentsInContour.begin ();
  advance (it, index);				// We are now at segment 'index+1'
  if (it == segmentsInContour.end ())
  {
    it = segmentsInContour.begin ();
  }  // Go back to the first segment if we reached end of the list
  segmentsInContour.erase (it);	// Delete segment 'index+1'
}

void
GeometryProcessing::secondCavendishCase (int index,
                                         std::list<Segment> &segmentsInFile,
                                         std::list<Segment> &segmentsInContour,
                                         std::list<fElem> &fElemMesh,
                                         std::list<Point> &pointsInFile,
                                         /* FIXME this should not exist; use index instead*/float &angle_mini,
                                         bool &invert)
{
  fElem fElem_tmp1;		// 1st finite element
  fElem fElem_tmp2;		// 2nd finite element
  Point point_tmp;

  // Compute distance | d = 1/6*( d02 + 2*(d12+d32) + d42 )
  Point d0, d1, d2, d3, d4;

  std::list<Segment>::iterator it = segmentsInContour.begin ();
  advance (it, index - 1);  // Segment 'index'

  fElem_tmp1.p1 = (*it).head;
  fElem_tmp1.p2 = (*it).tail;

  d1.coord_X = (*it).head->coord_X;
  d1.coord_Y = (*it).head->coord_Y;

  d2.coord_X = (*it).tail->coord_X;
  d2.coord_Y = (*it).tail->coord_Y;

  if (it == segmentsInContour.begin ())
  {
    // If we are at the beginning, reach the end of the list
    it = segmentsInContour.end ();
  }
  it--;				// Segment 'index-1'

  d0.coord_X = (*it).head->coord_X;
  d0.coord_Y = (*it).head->coord_Y;

  it = segmentsInContour.begin ();
  advance (it, index - 1);  // Segment 'index'
  it++;				// Segment 'index+1'
  if (it == segmentsInContour.end ())
  {  // If we reached end of the list, go back to the start
    it = segmentsInContour.begin ();
  }
  it++;				// Segment 'index+2'
  if (it == segmentsInContour.end ())
  {
    it = segmentsInContour.begin ();
  }

  fElem_tmp2.p1 = (*it).head;
  fElem_tmp2.p2 = (*it).tail;

  d3.coord_X = (*it).head->coord_X;
  d3.coord_Y = (*it).head->coord_Y;

  d4.coord_X = (*it).tail->coord_X;
  d4.coord_Y = (*it).tail->coord_Y;

  /*
   printf("\nd1 [%f | %f]\nd2 [%f | %f]\nd3 [%f | %f]\nd4 [%f | %f]\n",
   d1.coord_X, d1.coord_Y, d2.coord_X, d2.coord_Y, d3.coord_X, d3.coord_Y, d4.coord_X, d4.coord_Y);
   */

  // Distance between points d0 and d2
  double d02 = sqrt (pow ( (d0.coord_X - d2.coord_X), 2) + pow ( (d0.coord_Y - d2.coord_Y), 2));
  double d12 = sqrt (pow ( (d1.coord_X - d2.coord_X), 2) + pow ( (d1.coord_Y - d2.coord_Y), 2));
  double d32 = sqrt (pow ( (d3.coord_X - d2.coord_X), 2) + pow ( (d3.coord_Y - d2.coord_Y), 2));
  double d42 = sqrt (pow ( (d4.coord_X - d2.coord_X), 2) + pow ( (d4.coord_Y - d2.coord_Y), 2));
  double norme = (1.0 / 6.0) * (d02 + 2.0 * (d12 + d32) + d42);
  /*std::cout	<< "\nd02 = " << d02 << std::endl << "d12 = " << d12 << std::endl
   << "d32 = " << d32 << std::endl << "d42 = " << d42 << std::endl;
   std::cout << "Norme à respecter pour le vecteur : " << norme << std::endl;
   */
  // Compute the bisectrix of vectors 'index' and 'index+1'
  it = segmentsInContour.begin ();
  advance (it, index - 1);  // Go to segment 'index'

  double vect_X = (*it).tail->coord_X - (*it).head->coord_X;	//	X component of the bisectrix
  double vect_Y = (*it).tail->coord_Y - (*it).head->coord_Y;	//	Y component of the bisectrix
  //std::cout << "Vect[X|Y] : [" << vect_X << "|" << vect_Y << "]" << std::endl;
  double angle_rot_rad = (angle_mini * M_PI / 180.0) / 2.0;
  //printf("Rotation angle:%.2f\n", angle_rot_rad);

  // Make sure we are using correct angles
  if (invert)
  {
    angle_rot_rad += M_PI;
  }
  else
  {
    angle_rot_rad = -angle_rot_rad - M_PI;
  }
  //printf("Real rotation angle:%.2f\n", angle_rot_rad);

  // Apply a 2D rotation matrix on the vector:
  double new_vect_X = vect_X * cos (angle_rot_rad) - vect_Y * sin (angle_rot_rad);
  double new_vect_Y = vect_X * sin (angle_rot_rad) + vect_Y * cos (angle_rot_rad);

  // Normalize the vector "d"
  double norme_new_vect = sqrt (pow (new_vect_X, 2) + pow (new_vect_Y, 2));
  //std::cout << "Old norm: " << norme_new_vect << std::endl;

  new_vect_X *= norme / norme_new_vect;
  new_vect_Y *= norme / norme_new_vect;

  norme_new_vect = sqrt (pow (new_vect_X, 2) + pow (new_vect_Y, 2));
  //std::cout << "New norm: " << norme_new_vect << std::endl;

  // New point coordinates :
  point_tmp.coord_X = (*it).tail->coord_X + new_vect_X;
  point_tmp.coord_Y = (*it).tail->coord_Y + new_vect_Y;
  //printf("p.x %.2f	p.y %.2f\n", point_tmp.coord_X, point_tmp.coord_Y);
  pointsInFile.push_back (point_tmp);

  // Add the 3 segments that are linked to the new point "d"
  // Be careful with vector orientations !
  Segment Segment_temp1, Segment_temp2, Segment_temp3;
  std::list<Point>::iterator it_Point_d = pointsInFile.end ();
  it_Point_d--;  // it on the last point (d2)
  fElem_tmp1.p3 = it_Point_d;
  fElem_tmp2.p3 = it_Point_d;

  it = segmentsInContour.begin ();
  advance (it, index - 1);  // Go to segment 'index'

  Segment_temp1.head = (*it).tail;		// head = tail of vector 'index'
  Segment_temp1.tail = it_Point_d;		// tail = new point coordinates

  Segment_temp2.head = (*it).head;		// head = head of vector 'index'
  Segment_temp2.tail = it_Point_d;		// tail = new point coordinates

  it++;
  if (it == segmentsInContour.end ())
  {
    it = segmentsInContour.begin ();
  }

  Segment_temp3.head = it_Point_d;		// head = new point coordinates
  Segment_temp3.tail = (*it).tail;		// tail = tail of vector 'index+1'

  segmentsInFile.push_back (Segment_temp1);	// Add new segment (bisectrix)
  segmentsInFile.push_back (Segment_temp3);	// Add new segment (between new point and d3)
  segmentsInFile.push_back (Segment_temp2);	// Add new segment (between new point and d1)

  // Update the contour: Replace the 2 headinal segments by the 2 new segments
  it = segmentsInContour.begin ();
  advance (it, index - 1);  // Go to segment 'index'

  std::list<Segment>::iterator it_temp = segmentsInFile.end ();
  it_temp--;  // Segment_temp2

  // Replace it par it_temp
  (*it).head = (*it_temp).head;
  (*it).tail = (*it_temp).tail;

  it++;  // 'index+1'
  if (it == segmentsInContour.end ())
  {
    it = segmentsInContour.begin ();
  }

  it_temp--;  // Segment_temp3
  // Replace it par it_temp
  (*it).head = (*it_temp).head;
  (*it).tail = (*it_temp).tail;

  fElemMesh.push_back (fElem_tmp1);	// Add the new finite elements
  fElemMesh.push_back (fElem_tmp2);
}

void
GeometryProcessing::thirdCavendishCase (int index,
                                        std::list<Segment> &segmentsInFile,
                                        std::list<Segment> &segmentsInContour,
                                        std::list<fElem> &fElemMesh,
                                        std::list<Point> &pointsInFile,
                                        bool &invert)
{
  ElemF fElem_temp;		// We are going to create 1 finite element
  Point point_tmp;

  std::list<Segment>::iterator it = segmentsInContour.begin ();
  advance (it, index - 1);  // Go to segment j

  double vect_X = (*it).tail->coord_X - (*it).head->coord_X;	//	X component of the vector
  double vect_Y = (*it).tail->coord_Y - (*it).head->coord_Y;	//	Y component of the vector
  double norme = sqrt (pow (vect_X, 2) + pow (vect_Y, 2));		//	Norm
  //std::cout << "Vect[X|Y] : [" << vect_X << "|" << vect_Y << "]" << std::endl;
  double angle_rot_rad = 60 * M_PI / 180;  // 60° en radians

  if (invert)
  {
    angle_rot_rad += M_PI;
  }
  else
  {
    angle_rot_rad = -angle_rot_rad - M_PI;
  }

  double new_vect_X = vect_X * cos (angle_rot_rad) - vect_Y * sin (angle_rot_rad);
  double new_vect_Y = vect_X * sin (angle_rot_rad) + vect_Y * cos (angle_rot_rad);
  double norme_new_vect = sqrt (pow (new_vect_X, 2) + pow (new_vect_Y, 2));
  //std::cout << "Norm: " << norme_new_vect << std::endl;

  new_vect_X *= norme / norme_new_vect;
  new_vect_Y *= norme / norme_new_vect;

  norme_new_vect = sqrt (pow (new_vect_X, 2) + pow (new_vect_Y, 2));
  //std::cout << "Norm: " << norme_new_vect << std::endl;

  point_tmp.coord_X = (*it).tail->coord_X + new_vect_X;
  point_tmp.coord_Y = (*it).tail->coord_Y + new_vect_Y;
  pointsInFile.push_back (point_tmp);

  fElem_temp.p1 = (*it).head;
  fElem_temp.p2 = (*it).tail;

  Segment Segment_temp1, Segment_temp2;
  std::list<Point>::iterator it_Point_d = pointsInFile.end ();
  it_Point_d--;
  fElem_temp.p3 = it_Point_d;
  fElemMesh.push_back (fElem_temp);

  it = segmentsInContour.begin ();
  advance (it, index - 1);

  Segment_temp1.head = (*it).head;
  Segment_temp1.tail = it_Point_d;

  Segment_temp2.head = it_Point_d;
  Segment_temp2.tail = (*it).tail;

  segmentsInFile.push_back (Segment_temp1);
  segmentsInFile.push_back (Segment_temp2);

  // Delete the 'index'segment in SegmentContour and put the two lasts of segmentsInFile at this place
  it = segmentsInContour.begin ();
  advance (it, index - 1);

  // Insert before the segment pointed by the iterator
  segmentsInContour.insert (it, Segment_temp1);
  // Then replace :
  (*it).head = Segment_temp2.head;
  (*it).tail = Segment_temp2.tail;
}

void
GeometryProcessing::lastPolygonCavendish (std::list<Segment> &segmentsInFile,
                                          std::list<Segment> &segmentsInContour,
                                          std::list<fElem> &fElemMesh)
{
  fElem fElem_tmp1;
  fElem fElem_tmp2;

  Point d1, d2, d3, d4;
  std::list<Segment>::iterator it = segmentsInContour.begin ();

  d1.coord_X = (*it).head->coord_X;
  d1.coord_Y = (*it).head->coord_Y;

  d2.coord_X = (*it).tail->coord_X;
  d2.coord_Y = (*it).tail->coord_Y;

  it++;  // Segment 2
  it++;  // Segment 3

  d3.coord_X = (*it).head->coord_X;
  d3.coord_Y = (*it).head->coord_Y;

  d4.coord_X = (*it).tail->coord_X;
  d4.coord_Y = (*it).tail->coord_Y;

  double vect_1_X = d1.coord_X - d3.coord_X;
  double vect_1_Y = d1.coord_Y - d3.coord_Y;

  double vect_2_X = d2.coord_X - d4.coord_X;
  double vect_2_Y = d2.coord_Y - d4.coord_Y;

  double norme_1 = sqrt (pow (vect_1_X, 2) + pow (vect_1_Y, 2));		//	Norm
  double norme_2 = sqrt (pow (vect_2_X, 2) + pow (vect_2_Y, 2));		//	Norm

  Segment Segment_temp;
  it = segmentsInContour.begin ();

  if (norme_1 < norme_2)
  {	// 1->3 < 2->4
    // Draw vector 1
    Segment_temp.head = (*it).head;  // Point 1
    fElem_tmp1.p1 = (*it).head;
    fElem_tmp1.p2 = (*it).tail;
    fElem_tmp2.p1 = (*it).head;
    it++;
    it++;
    Segment_temp.tail = (*it).head;  // Point 3
    fElem_tmp1.p3 = (*it).head;
    fElem_tmp2.p2 = (*it).head;
    fElem_tmp2.p3 = (*it).tail;

  }
  else
  { // 2->4 < 1->3
    Segment_temp.head = (*it).tail;  // Point 2
    fElem_tmp1.p1 = (*it).head;
    fElem_tmp1.p2 = (*it).tail;
    fElem_tmp2.p1 = (*it).tail;
    it++;
    it++;
    Segment_temp.tail = (*it).tail;  // Point 4
    fElem_tmp1.p3 = (*it).tail;
    fElem_tmp2.p2 = (*it).head;
    fElem_tmp2.p3 = (*it).tail;
  }
  segmentsInFile.push_back (Segment_temp);
  fElemMesh.push_back (fElem_tmp1);
  fElemMesh.push_back (fElem_tmp2);
  segmentsInContour.clear ();
}
