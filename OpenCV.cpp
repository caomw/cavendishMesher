#include "OpenCV.hpp"

OpenCV::OpenCV ()
{
}

void
OpenCV::scalePoints (std::list<Point> &points)
{
  // TODO Improve this
  float marge;
  float min_X = points.begin ()->coord_X;
  float min_Y = points.begin ()->coord_Y;
  float max_X = points.begin ()->coord_X;
  float max_Y = points.begin ()->coord_Y;

  for (std::list<Point>::iterator it = points.begin (); it != points.end (); ++it)
  {
    // Search for minimums / maximums
    if ( (*it).coord_X < min_X)
    {
      min_X = (*it).coord_X;
    }
    if ( (*it).coord_X > max_X)
    {
      max_X = (*it).coord_X;
    }
    if ( (*it).coord_Y < min_Y)
    {
      min_Y = (*it).coord_Y;
    }
    if ( (*it).coord_Y > max_Y)
    {
      max_Y = (*it).coord_Y;
    }
  }

  // Set up the zoom
  float zoom = 1.0;
  float size = ZOOM;
  zoom = (2 * size) / std::max (max_X, max_Y);
  min_X *= zoom;
  min_Y *= zoom;
  max_X *= zoom;
  max_Y *= zoom;

  marge = 5.0;

  max_X += -min_X + marge;
  max_Y += -min_Y + marge;
  min_X -= marge;
  min_Y -= marge;

  for (std::list<Point>::iterator it = points.begin (); it != points.end (); ++it)
  {
    (*it).coord_X *= zoom;
    (*it).coord_Y *= zoom;
    (*it).coord_X -= min_X;
    (*it).coord_Y -= min_Y;
  }
}

void
OpenCV::drawMesh (std::list<Segment> &segmentsInFile,
                  std::list<Segment> &segmentsInContour,
                  std::list<Point> &pointsInFile)
{
  // Draw all segments in the list "segmentsInFile"
  float min_X = pointsInFile.begin ()->coord_X;
  float min_Y = pointsInFile.begin ()->coord_Y;
  float max_X = pointsInFile.begin ()->coord_X;
  float max_Y = pointsInFile.begin ()->coord_Y;

  for (std::list<Point>::iterator it = pointsInFile.begin (); it != pointsInFile.end (); ++it)
  {
    // Search for maximums / minimums
    if ( (*it).coord_X < min_X)
    {
      min_X = (*it).coord_X;
    }
    if ( (*it).coord_X > max_X)
    {
      max_X = (*it).coord_X;
    }
    if ( (*it).coord_Y < min_Y)
    {
      min_Y = (*it).coord_Y;
    }
    if ( (*it).coord_Y > max_Y)
    {
      max_Y = (*it).coord_Y;
    }
  }

  // Create a black empty image a little bit larger than the extremas
  char base_window[] = "File = grey | Contour = colour";	// Name of the window
  base_image = cv::Mat::zeros (1.05 * max_Y, 1.05 * max_X, CV_8UC3);

  int colour = 0;
  for (std::list<Segment>::iterator it = segmentsInFile.begin (); it != segmentsInFile.end (); ++it)
  {
    // Loop to create segments
    this->drawLine (cv::Point ( (*it).head->coord_X, (*it).head->coord_Y), cv::Point ( (*it).tail->coord_X, (*it).tail->coord_Y), 128, 128, 128, 1);
    colour++;
  }

  colour = 0;
  if (segmentsInContour.size () != 0)
  {
    for (std::list<Segment>::iterator it = segmentsInContour.begin (); it != segmentsInContour.end (); ++it)
    {
      // Loop to create segments
      int r, g, b;

      switch (colour)
      {  // Change line colors to differenciate small segments
        case 0:
          r = 255;
          g = 50;
          b = 50;
          break;
        case 1:
          r = 50;
          g = 50;
          b = 255;
          colour = -1;
          break;
        default:
          r = 255;
          g = 255;
          b = 255;
          colour = -1;
          break;
      }

      this->drawLine (cv::Point ( (*it).head->coord_X, (*it).head->coord_Y), cv::Point ( (*it).tail->coord_X, (*it).tail->coord_Y), r, g, b, 2);
      colour++;
    }
  }

  // Draw a white/red point at the beginning of the contour
  cv::circle (base_image, cv::Point (segmentsInContour.front ().head->coord_X, segmentsInContour.front ().head->coord_Y), 2, cv::Scalar (255, 255, 255), 2,
              1/*FIXME CV_AA*/, 0);

  // Draw the window
  //cv::bitwise_not ( base_image, base_image );	// Invert colours
  cv::flip (base_image, base_image, 0);			// Flip image
  cv::imshow (base_window, base_image);			// Show window
  //cv::resizeWindow(base_window, 900, 900);

  if (segmentsInContour.size () != 0)
  {
    cv::waitKey (SPEED);
  }
  else
  {
    cv::waitKey (0);
  }
}

void
OpenCV::drawLine (cv::Point head,
                  cv::Point tail,
                  int color_r,
                  int color_g,
                  int color_b,
                  int thickness)
{
  int lineType = 1/*FIXME CV_AA*/;  // Anti aliasing
  cv::line (base_image, head, tail, cv::Scalar (color_b, color_g, color_r),  // BGR, not RGB !
            thickness, lineType);
}

