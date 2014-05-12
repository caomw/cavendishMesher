/**
 * \file OpenCV.hpp
 * \brief OpenCV class definition.
 * \author Victor Lamoine
 * \version 0.1
 * \date may 2014
 */

#ifndef OPENCV_HPP
#define OPENCV_HPP

#include "mesh.hpp"

/** \brief A class to draw with OpenCV
 *
 * This object provides methods to draw with OpenCV.
 */
class OpenCV
{
  public:
    OpenCV ();

    /** \brief Scale a list of points to fit a window
     * \param[in] points the list of points to scale
     */
    void
    scalePoints (std::list<Point> &points);

    /** \brief Draw a mesh using OpenCV
     * \param[in] segmentsInFile the up-to-date list of  segments
     * \param[in] segmentsInContour the contour of the polygon (changes during execution)
     * \param[in] pointsInFile the list of points
     */
    void
    drawMesh (std::list<Segment> &segmentsInFile,
              std::list<Segment> &segmentsInContour,
              std::list<Point> &pointsInFile);

    /** \brief Draw a mesh using OpenCV
     * \param[in] head the starting point
     * \param[in] tail the ending point
     * \param[in] color_r red level color (0-255)
     * \param[in] color_g green level color (0-255)
     * \param[in] color_b blue level color (0-255)
     * \param[in] thickness the thickness of the line
     */
    void
    drawLine (cv::Point head,
              cv::Point tail,
              int color_r,
              int color_g,
              int color_b,
              int thickness);

  private:
    /** \brief the base image of the object*/
    cv::Mat base_image;

};

#endif
