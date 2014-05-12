/**
 * \file mesh.cpp
 * \brief The main function.
 * \author Victor Lamoine
 * \version 0.1
 * \date may 2014
 */

/** \mainpage
 *
 * This is the documentation of the Cavendish mesher program.\n
 * For more information please consult the <a href="https://github.com/VictorLamoine/cavendishMesher/blob/master/README.md">readme</a>
 * @image html heart.gif "Heart animation"
 */

#include "mesh.hpp"
#include "GeometryProcessing.hpp"
#include "TextProcessing.hpp"
#include "OpenCV.hpp"

/** \brief Print program help in terminal
 */
void
showHelp (char *argv[])
{
  std::cout << "Usage    " << argv[0] << " [input] [output]" << std::endl << std::endl;
  std::cout << "--help   " << " Print this help" << std::endl;
  std::cout << "input    " << "Path to the input (geo) file. Example : input.geo" << std::endl;
  std::cout << "output   " << "Path to the output (cal) file. Example : output.cal" << std::endl << std::endl;
  std::cout << "Example  " << argv[0] << " ../src/GeoFiles/heart.geo /dev/null " << std::endl;
}

/** \brief The main function
 */
int
main (int argc,
      char *argv[])
{
  std::list<Point> pointsInFile;		// This list holds all points in the file
  std::list<Segment> segmentsInFile;		// This list holds all segments in the file
  std::list<Segment> segmentsInContour;	// This list holds all segments in the contour
  std::list<float> anglesInContour;	// This list holds all angles between segments

  Point point_tmp;
  Segment segment_tmp;
  const char * input_file = NULL;		// Input (.geo) file
  const char * output_file = NULL;	// Output (.cal) file

// ******************************
//     CHECK PROGRAM ARGUMENTS
// ******************************

  if (argc == 2 && (strcmp (argv[1], "--help") == 0))
  {
    showHelp (argv);
    return (-1);
  }
  else if (argc == 3)
  {
    input_file = argv[1];
    output_file = argv[2];
  }
  else
  {
    std::cout << "Wrong usage !" << std::endl;
    showHelp (argv);
    return (-1);
  }

// *******************
//   READ .GEO FILE
// *******************

  std::cout << std::endl << "---------------" << std::endl << "FETCH .GEO DATA" << std::endl << "---------------" << std::endl;

  TextProcessing txtProcessing;
  if (!txtProcessing.readCalFile (input_file, pointsInFile, segmentsInFile))
    return (-1);

  // Print all points in pointsInFile list
  txtProcessing.printPoints (pointsInFile);
  // Print all segments in segmentsInFile list
  txtProcessing.printSegments (segmentsInFile);

  OpenCV open_cv;
  open_cv.scalePoints (pointsInFile);
  open_cv.drawMesh (segmentsInFile, segmentsInContour, pointsInFile);

// ******************************************
// ******************************************
//        RE-ORDERING OF THE SEGMENTS
// ******************************************
// ******************************************
  std::cout << std::endl << "-----------------------------" << std::endl << "RE-ORDERING OF THE SEGMENTS :" << std::endl << "-----------------------------"
      << std::endl;

  GeometryProcessing geom;
  if (!geom.orderSegments (segmentsInContour, segmentsInFile))
    return (-1);

  // The segments are now ordered so that .tail of segment[i] = .head of segment[i+1]
  // Of course for the last segment, segment[i+1] does not exist and [0] must be used
  txtProcessing.printSegments (segmentsInContour);
  // Create a copy of segmentsInContour (it's easier then to export the .CAL file)
  std::list<Segment> segmentsInContour_bak = segmentsInContour;

  open_cv.drawMesh (segmentsInFile, segmentsInContour, pointsInFile);

// *********************************
// *********************************
//        CAVENDISH MESHING
// *********************************
// *********************************
  std::cout << std::endl << "-------------------" << std::endl << "CAVENDISH MESHING :" << std::endl << "-------------------" << std::endl;

  std::list<fElem> fElemMesh;		// This list holds all finite elements
  bool invert = false;
  // Let's measure of angles in contour
  if (!geom.computeAngles (anglesInContour, segmentsInContour, invert))
  {
    return (-1);
  }

  // At this point we don't know if we measured exterior or interior angles:
  // If the mean value of the angles is greater than 180°, we measured exterior angles
  float sum = 0.0;
  for (std::list<float>::iterator it = anglesInContour.begin (); it != anglesInContour.end (); it++)
  {
    sum += (*it);
  }

  if (sum / anglesInContour.size () > 180)
  {
    // Too bad, we measured exterior angles; let's change the boolean value and compute the values again :
    invert = true;  // (vectors turns into a trigonometric way)
    if (!geom.computeAngles (anglesInContour, segmentsInContour, invert))
    {
      return (-1);
    }
  }

  int terminalPrint_count = 0;
  while (segmentsInContour.size () > 4)
  {  // While there's 4 segments left

    if (!geom.computeAngles (anglesInContour, segmentsInContour, invert))
    {
      return (-1);
    }
    int i = 1, j = 1;
    float angle_mini = anglesInContour.front ();

    for (std::list<float>::iterator it = anglesInContour.begin (); it != anglesInContour.end (); it++)
    {
      if (*it < angle_mini)
      {
        angle_mini = *it;
        j = i;
      }
      i++;
    }

    // The smallest angle is between segments j and j+1 (if j+1 > list.size() then j+1 = list.front() )
    open_cv.drawMesh (segmentsInFile, segmentsInContour, pointsInFile);

    if (angle_mini < CASE_1_CAVENDISH)
    {
      // ******************
      // CASE 1 - CAVENDISH
      // ******************
      std::cout << "[Case 1 | " << "Angle " << j << " | " << angle_mini << "°]	";
      std::cout.flush ();
      terminalPrint_count++;
      if (terminalPrint_count == 3)
      {
        std::cout << std::endl;
        terminalPrint_count = 0;
      }
      geom.firstCavendishCase (j, segmentsInFile, segmentsInContour, fElemMesh);

    }
    else if (angle_mini <= CASE_2_CAVENDISH)
    {
      // ******************
      // CASE 2 - CAVENDISH
      // ******************
      std::cout << "[Cas 2 | " << "Angle " << j << " | " << angle_mini << "°]	";
      std::cout.flush ();
      terminalPrint_count++;
      if (terminalPrint_count == 3)
      {
        std::cout << std::endl;
        terminalPrint_count = 0;
      }
      geom.secondCavendishCase (j, segmentsInFile, segmentsInContour, fElemMesh, pointsInFile, angle_mini, invert);

    }
    else if (angle_mini < 360)
    {
      // ******************
      // CASE 3 - CAVENDISH
      // ******************
      std::cout << "[Case 3 | " << "Angle " << j << " | " << angle_mini << "°]	";
      std::cout.flush ();
      terminalPrint_count++;
      if (terminalPrint_count == 3)
      {
        std::cout << std::endl;
        terminalPrint_count = 0;
      }
      geom.thirdCavendishCase (j, segmentsInFile, segmentsInContour, fElemMesh, pointsInFile, invert);

    }
    else
    {
      std::cout << "Error : the angle is greated than 360° !" << std::endl;
      return (-1);
    }
  }

  // There is only 4 segments left, measure the diagonals of the polygon and keep the tiniest
  if (segmentsInContour.size () != 4)
  {
    std::cout << "There is only 4 segments left !" << std::endl;
    return (-1);
  }
  geom.lastPolygonCavendish (segmentsInFile, segmentsInContour, fElemMesh);

  std::cout << std::endl << std::endl << "Number of finite elements added:  " << fElemMesh.size ();
  open_cv.drawMesh (segmentsInFile, segmentsInContour, pointsInFile);

// *************************
// *************************
//	    WRITE .CAL FILE
// *************************
// *************************
  std::cout << std::endl;
  std::cout << std::endl << "------------------------" << std::endl << "WRITE .CAL OUTPUT FILE :" << std::endl << "------------------------" << std::endl;

  if (!txtProcessing.writeCalFile (output_file, fElemMesh, pointsInFile, segmentsInFile, segmentsInContour_bak))
    return (-1);

  return (0);
}
