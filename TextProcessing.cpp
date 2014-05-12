#include "TextProcessing.hpp"

TextProcessing::TextProcessing ()
{
}

bool
TextProcessing::readCalFile (const std::string filename,
                             std::list<Point> &pointsInFile,
                             std::list<Segment> &segmentsInFile)
{
  std::ifstream ifs (filename.c_str ());
  if (ifs.is_open () != 1)
  {
    std::cout << "Error opening file " << filename << std::endl;
    return (false);
  }
  std::string temp;
  Point point_tmp;
  Segment segment_tmp;
  int point_count = 0;				// The number of points (read just after "$points" in the file)
  int curve_count = 0;				// The number of curves (computed): also available with segmentsInFile.size()

  while (getline (ifs, temp))
  {
    // While the file is not finished:
    if (temp.compare (0, strlen ("$points\r"), "$points\r") == 0)
    {
      // We found the line "$points"
      getline (ifs, temp);					// Fetch next line (it holds the number of points)
      std::istringstream iss (temp);

      if (iss >> point_count)
      {
        // If iss casts into an integer
        if (point_count > 3)
        {
          // Le fichier contient au moins 3 points (c'est le minimum pour créer un contour)
          std::cout << "The file " << filename << " should hold " << point_count << " points." << std::endl;
        }
        else
        {
          // There is not enough points in the "shape" !
          std::cout << "\nThe file must hold at least 3 points ! " << std::endl;
          return (false);
        }
      }
      else
      {
        // There is something wrong !
        std::cout << "The line after $points does not hold an integer value !" << std::endl << "Error line: " << temp << std::endl;
        return (false);
      }

      getline (ifs, temp);  // Fetch next line

      while (temp.compare (0, strlen ("$courbes\r"), "$courbes\r") != 0)
      {
        // While we do not meet "$courbes" line: (add points)
        // A point is defined in this format: "   1  0.123456789123E+02 -1.123456789123E+02 0  1.00"
        std::string str_X = temp.substr (5, 13);		// Extract X coordinate (including the sign)
        std::string str_Xp = temp.substr (20, 3);		// Extract X power (including the sign)
        std::string str_Y = temp.substr (24, 13);		// Extract Y coordinate (including the sign)
        std::string str_Yp = temp.substr (39, 3);		// Extract Y power (including the sign)

        float X = 0.0;
        float Y = 0.0;
        int Xp = 0;
        int Yp = 0;

        iss.clear ();			// Empty iss
        iss.str (str_X);
        if (! (iss >> X))
        {		// Try to cast iss into a float
          std::cout << "\nError processing X coordinate" << std::endl << "Error line: " << temp << std::endl;
          return (false);
        }

        iss.clear ();
        iss.str (str_Xp);
        if (! (iss >> Xp))
        {		// Try to cast iss into a integer
          std::cout << "\nError processing X power" << std::endl << "Error line: " << temp << std::endl;
          return (false);
        }

        iss.clear ();
        iss.str (str_Y);
        if (! (iss >> Y))
        {
          std::cout << "\nError processing Y coordinate" << std::endl << "Error line: " << temp << std::endl;
          return (false);
        }

        iss.clear ();
        iss.str (str_Yp);
        if (! (iss >> Yp))
        {
          std::cout << "\nError processing Y power" << std::endl << "Error line: " << temp << std::endl;
          return (false);
        }

        // Compute X, Y coordinates value
        point_tmp.coord_X = X * pow (10.0, Xp);
        point_tmp.coord_Y = Y * pow (10.0, Yp);
        pointsInFile.push_back (point_tmp);					// Push back the point in the list
        getline (ifs, temp);									// Fetch next line
      }
    }

    // We are now on line "$courbes", points are all imported
    if (point_count != pointsInFile.size ())
    {
      // Compute if the number of points imported is the same as in the "header"
      std::cout << "\nError in the number of points: " << point_count << " != " << pointsInFile.size () << std::endl;
      return (false);
    }

    // Processing "$courbes"
    if (temp.compare (0, strlen ("$courbes\r"), "$courbes\r") == 0)
    {
      std::string str_segment = "segment";							// String "segment"
      std::string str_arc = "arc";								// String "arc"
      getline (ifs, temp);		// Fetch next list

      while (temp.compare (0, strlen ("//// fin"), "//// fin") != 0)
      {
        // While "$courbes" remains

        if (temp.compare (0, str_arc.length (), str_arc) == 0)
        {
          // Arc has been detected
          std::cout << "\nArcs are not supported !" << std::endl << "Error line: " << temp << std::endl;
          return (false);
        }
        else if (temp.compare (0, str_segment.length (), str_segment) == 0)
        {
          // Segment
          std::string str_p1 = temp.substr (8, (temp.find (" ", 8) - 8));			// Head point
          std::string str_p2 = temp.substr ( (temp.find (" ", 8), temp.find (" ", temp.find (" ", 8))));  // Tail point
          int p1 = 0;	// Point 1 (head)
          int p2 = 0;	// Point 2 (tail)

          std::istringstream iss (str_p1);
          if (! (iss >> p1))
          {
            std::cout << "\nError, point 1 is incorect for curve " << (curve_count + 1) << std::endl << "Error line: " << temp << std::endl;
            return (false);
          }

          iss.clear ();
          iss.str (str_p2);
          if (! (iss >> p2))
          {
            std::cout << "\nError, point 2 is incorect for curve " << (curve_count + 1) << std::endl << "Error line: " << temp << std::endl;
            return (false);
          }

          if (p1 <= 0 || p1 > point_count || p2 <= 0 || p2 > point_count || p1 == p2)
          {
            // Check if points exists and if the segment is consistent
            std::cout << "\nError in points p1 / p2 of segment " << (curve_count + 1) << std::endl << "Error line: " << temp << std::endl;
            return (false);
          }

          // p1 and p2 holds the number of the points in the list order
          if (p1 < p2)
          {  // p1 < p2, run through the list in normal order
            std::list<Point>::iterator it = pointsInFile.begin ();  // Initialise iterator at list head

            for (int i = 0; i < (p1 - 1); i++)
            {
              it++;  // Loop until p1
            }
            segment_tmp.head = it;	// Store the interator into segment head

            for (int i = 0; i < (p2 - p1); i++)
            {
              it++;  // Loop until p2
            }
            segment_tmp.tail = it;	// Store the interator into segment tail
          }
          else if (p1 > p2)
          {
            // Same as before but run through the list in reverse order !
            std::list<Point>::iterator it = pointsInFile.begin ();

            for (int i = 0; i < (p2 - 1); i++)
            {
              it++;
            }
            segment_tmp.tail = it;

            for (int i = 0; i < (p1 - p2); i++)
            {
              it++;
            }
            segment_tmp.head = it;
          }
          segmentsInFile.push_back (segment_tmp);	// Push back the new segment
          curve_count++;							// Count segments
          getline (ifs, temp);						// Fetch next line
        }
        else
        {
          // Ce n'est ni un arc, ni un segment !
          std::cout << "\nThe curve is not an arc nor a segment !" << std::endl << "Error line: " << temp << std::endl;
          return (false);
        }
      }
      std::cout << std::endl;
    }
  }

  // All points and curves has been imported
  if (point_count < 4 || curve_count < 4)
  {
    std::cout << "\nWe need at least four points and 4 segments to mesh anything !" << std::endl;
    return (false);
  }
  else if (point_count != curve_count)
  {
    std::cout << "\nThe number of points is different of the number of segments, there's a problem !" << std::endl << "Please check file contents" << std::endl;
    return (false);
  }

  return (true);
}

bool
TextProcessing::writeCalFile (const std::string filename,
                              std::list<fElem> &fElemMesh,
                              std::list<Point> &pointsInFile,
                              std::list<Segment> &segmentsInFile,
                              std::list<Segment> &segmentsInContour_bak)
{

  std::ofstream ofs (filename.c_str ());
  if (ofs.is_open () != 1)
  {
    std::cout << "Error opening " << filename << std::endl;
    return (false);
  }

  // Standard beginning of a .CAL file :
  ofs << "\n   RDM - Eléments finis\n";
  "   Calcul des Structures par la Méthode des Eléments Finis\n"
      "\n"
      "   Version  - 6.14 - 22 Janvier 2002\n"
      "\n"
      "   Utilisateur : Ecole Nationale d'Ingénieurs - BREST\n"
      "\n"
      "$debut du fichier\n"
      "$version\n"
      "6.14\n"
      "$SI unites\n"
      "$nom du fichier\n"
      "fichier.cal\n"
      "$date\n"
      "1/01/2014\n"
      "$HEURE\n"
      "10/10/10\n"
      "$PROBLEME\n"
      "CONTRAINTES PLANES\n"
      "";

  std::cout << "$NOEUDS" << "\n" << pointsInFile.size () << std::endl;
  ofs << "$NOEUDS" << "\n" << pointsInFile.size () << "\n";

  std::list<Point>::iterator it_point;
  int i = 1;
  for (it_point = pointsInFile.begin (); it_point != pointsInFile.end (); it_point++)
  {

    // Compute the number of digits in i integer
    int digit = 0;
    int number = i;
    while (number != 0)
    {
      number /= 10;
      digit++;
    }
    //printf("%d has %d digits\n", i, digit);

    // 4 spaces minus the number of digits in the number
    for (int j = 0; j < (4 - digit); j++)
    {
      std::cout << " ";
      ofs << " ";
    }

    std::cout << i << "  " << std::setprecision (10) << std::scientific << it_point->coord_X << "  " << it_point->coord_Y;
    ofs << i << "  " << std::setprecision (10) << std::scientific << it_point->coord_X << "  " << it_point->coord_Y;

    digit = 0;
    number = i;
    while (number != 0)
    {
      number /= 10;
      digit++;
    }

    // 6 spaces minus the number of digits in the number
    for (int j = 0; j < (6 - digit); j++)
    {
      std::cout << " ";
      ofs << " ";
    }

    std::cout << i;
    ofs << i;

    /* TODO "number" is not right */
    digit = 0;
    number = 0;
    while (number != 0)
    {
      number /= 10;
      digit++;
    }

    // 6 spaces minus the number of digits in the number
    for (int j = 0; j < (6 - digit); j++)
    {
      std::cout << " ";
      ofs << " ";
    }

    std::cout << 0 << std::endl;
    ofs << 0 << "\n";

    i++;
  }

  std::cout << "$limites de zones" << std::endl;
  ofs << "$limites de zones" << "\n";

  // Run through segmentsInContour_bak, search for for the point number and add it
  i = 0;
  for (std::list<Segment>::iterator it_seg = segmentsInContour_bak.begin (); it_seg != segmentsInContour_bak.end (); it_seg++)
  {
    i++;
    int head = 0;
    int tail = 0;
    int j = 0;

    // Search for for first point
    for (std::list<Point>::iterator it_pnt = pointsInFile.begin (); it_pnt != pointsInFile.end (); it_pnt++)
    {
      j++;
      if (it_seg->head == it_pnt)
      {
        head = j;
      }
      if (it_seg->tail == it_pnt)
      {
        tail = j;
      }
    }

    // Write the number of the segment
    int digit = 0;
    int number = i;
    while (number != 0)
    {
      number /= 10;
      digit++;
    }

    for (int j = 0; j < (3 - digit); j++)
    {
      std::cout << " ";
      ofs << " ";
    }
    std::cout << i;
    ofs << i;

    // Write head point number
    digit = 0;
    number = head;
    while (number != 0)
    {
      number /= 10;
      digit++;
    }
    for (int j = 0; j < (6 - digit); j++)
    {
      std::cout << " ";
      ofs << " ";
    }
    std::cout << head;
    ofs << head;

    // Write tail point number
    digit = 0;
    number = tail;
    while (number != 0)
    {
      number /= 10;
      digit++;
    }
    for (int j = 0; j < (6 - digit); j++)
    {
      std::cout << " ";
      ofs << " ";
    }
    std::cout << tail << std::endl;
    ofs << tail << "\n";

  }

  //std::cout << "$points a mailler" << std::endl;
  ofs << "$points a mailler" << "\n";

  // Run through the list of headinal points and add them
  it_point = pointsInFile.begin ();
  for (i = 0; i < pointsInFile.size (); i++)
  {
    //std::cout << i+1 << "  " << std::setprecision(9) << std::scientific << it_point->coord_X << "  " << it_point->coord_Y << std::endl;
    it_point++;
    ofs << i + 1 << "  " << std::setprecision (9) << std::scientific << it_point->coord_X << "  " << it_point->coord_Y << "\n";
  }

  ofs << "0\n";
  std::cout << "$ELEMENTS" << std::endl;
  ofs << "$ELEMENTS" << "\n";

  std::cout << fElemMesh.size () << std::endl;
  ofs << fElemMesh.size () << "\n";

  // Run through the list of finite elements and add them in the .cal file
  int j = 0;

  for (std::list<fElem>::iterator it_fElem = fElemMesh.begin (); it_fElem != fElemMesh.end (); it_fElem++)
  {
    j++;

    int digit = 0;
    int number = j;
    while (number != 0)
    {
      number /= 10;
      digit++;
    }

    for (int k = 0; k < (4 - digit); k++)
    {
      std::cout << " ";
      ofs << " ";
    }
    std::cout << j << " TRI3 1   1  11  11";
    ofs << j << " TRI3 1   1  11  11";

    // Fetch p1, p2 and p3 numbers
    int p1, p2, p3;
    int v = 0;
    for (std::list<Point>::iterator it_pnt = pointsInFile.begin (); it_pnt != pointsInFile.end (); it_pnt++)
    {
      v++;
      if (it_fElem->p1 == it_pnt)
      {
        p1 = v;
      }
      if (it_fElem->p2 == it_pnt)
      {
        p2 = v;
      }
      if (it_fElem->p3 == it_pnt)
      {
        p3 = v;
      }
    }

    digit = 0;
    number = p1;
    while (number != 0)
    {
      number /= 10;
      digit++;
    }
    for (int j = 0; j < (7 - digit); j++)
    {
      std::cout << " ";
      ofs << " ";
    }
    std::cout << p1;
    ofs << p1;

    digit = 0;
    number = p2;
    while (number != 0)
    {
      number /= 10;
      digit++;
    }
    for (int j = 0; j < (7 - digit); j++)
    {
      std::cout << " ";
      ofs << " ";
    }
    std::cout << p2;
    ofs << p2;

    digit = 0;
    number = p3;
    while (number != 0)
    {
      number /= 10;
      digit++;
    }
    for (int j = 0; j < (7 - digit); j++)
    {
      std::cout << " ";
      ofs << " ";
    }
    std::cout << p3 << std::endl;
    ofs << p3 << "\n";
  }

  /*
   9 TRI3 1   1  11  11     25     24     23
   10 TRI3 1   1  11  11     22     12      8
   */

  // Standard ending of file
  ofs << "$materiaux\n"
      "11\n"
      "MOD  2.100000E+11\n"
      "POI  3.000000E-01\n"
      "MAS  7.800000E+03\n"
      "DIL  1.300000E-05\n"
      "CON  5.000000E+01\n"
      "LIM  2.500000E+08\n"
      "CAP  4.500000E+02\n"
      "NOM Acier\n"
      "///\n"
      "0\n"
      "$epaisseurs\n"
      "11 0.00000E+00\n"
      "0\n"
      "$modes propres\n"
      "nombre 1\n"
      "methode sous_espace\n"
      "precision 1.00000E-02\n"
      "decalage_spectral 0.00000E+00\n"
      "////\n"
      "$fin du fichier\n"
      "";
  return (true);
}

void
TextProcessing::printPoints (std::list<Point> &points)
{
  std::cout << "List of the " << points.size () << " points [X | Y] :" << std::endl << std::endl;
  for (std::list<Point>::iterator it = points.begin (); it != points.end (); ++it)
  {
    std::cout << "[" << (*it).coord_X << "|" << (*it).coord_Y << "]" << std::endl;
  }
}

void
TextProcessing::printSegments (std::list<Segment> &segments)
{
  std::cout << std::endl << "List of the " << segments.size () << " segments :" << std::endl << std::endl;
  for (std::list<Segment>::iterator it = segments.begin (); it != segments.end (); it++)
  {
    std::cout << "Head  [" << (*it).head->coord_X << "|" << (*it).head->coord_Y << "]" << "		" << "Tail  [" << (*it).tail->coord_X << "|" << (*it).tail->coord_Y
        << "]" << std::endl;
  }
}

/*
 void
 TextProcessing::print_fElems(std::list<fElem>* Elements) {
 std::cout << std::endl << "List of the " << Elements->size() << " finite elements :" << std::endl << std::endl;
 for (std::list<fElem>::iterator it=Elements->begin(); it != Elements->end(); it++) {
 std::cout	<< "P1  [" << (*it).p1->coord_X << "|" << (*it).p1->coord_Y << "]"	 << "		"
 << "P2  [" << (*it).p2->coord_X << "|" << (*it).p2->coord_Y << "]"	 << "		"
 << "P3  [" << (*it).p3->coord_X << "|" << (*it).p3->coord_Y << "]" << std::endl;
 }
 }
 */
