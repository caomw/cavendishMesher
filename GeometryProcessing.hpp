/**
 * \file GeometryProcessing.hpp
 * \brief GeometryProcessing class definition.
 * \author Victor Lamoine
 * \version 0.1
 * \date may 2014
 */

#ifndef GEOMETRYPROCESSING_HPP
#define GEOMETRYPROCESSING_HPP

#include "mesh.hpp"

/** \brief A class to process geometry
 *
 * This object provides methods to process geometry features.
 */
class GeometryProcessing
{
  public:
    GeometryProcessing ();

    /** \brief Order planes so that segment j tail = segment j+1 head
     * \param[in] segmentsInContour the list of segments to be updated
     * \param[in] segmentsOriginal the reference list
     * \return true if successful, false otherwise.
     *
     * \warning Do not use with segmentsInContour = segmentsOriginal
     */
    bool
    orderSegments (std::list<Segment> &segmentsInContour,
                   std::list<Segment> &segmentsOriginal);

    /** \brief Compute all angles into a convex non intersecting polygon
     * \param[out] anglesContour the list angles to be filled
     * \param[in] segmentsContour the list of segments representing the polygon
     * \param[in] invert allow to inverse to switch to interior/exterior angles
     * \param[in] print print list angles
     * \return True if success, false otherwise
     */
    bool
    computeAngles (std::list<float> &anglesContour,
                   std::list<Segment> &segmentsContour,
                   bool invert = false,
                   bool print = false);

    /** \brief First Cavendish case, create 1 finite element
     * \param[in] index the index of the smallest angle
     * \param[out] segmentsInFile
     * \param[out] segmentsInContour
     * \param[out] fElemMesh The list filled with the finite elements
     */
    void
    firstCavendishCase (int index,
                        std::list<Segment> &segmentsInFile,
                        std::list<Segment> &segmentsInContour,
                        std::list<fElem> &fElemMesh);

    /** \brief First Cavendish case, create 2 finite elements
     * \param[in] index the index of the smallest angle
     * \param[out] segmentsInFile
     * \param[out] segmentsInContour
     * \param[out] fElemMesh The list filled with the finite elements
     * \param[in] pointsInFile The list of points
     * \param[in] angle_mini The value of the minimum angle
     * \param[in] invert Whether the vectors are inverted or not (clockwise or not)
     *
     */
    void
    secondCavendishCase (int index,
                         std::list<Segment> &segmentsInFile,
                         std::list<Segment> &segmentsInContour,
                         std::list<fElem> &fElemMesh,
                         std::list<Point> &pointsInFile,
                         float &angle_mini,
                         bool &invert);

    /** \brief Third Cavendish case, create 1 finite element
     * \param[in] index the index of the smallest angle
     * \param[out] segmentsInFile
     * \param[out] segmentsInContour
     * \param[out] fElemMesh The list filled with the finite elements
     * \param[out] pointsInFile
     * \param[in] invert Whether the vectors are inverted or not (clockwise or not)
     */
    void
    thirdCavendishCase (int index,
                        std::list<Segment> &segmentsInFile,
                        std::list<Segment> &segmentsInContour,
                        std::list<fElem> &fElemMesh,
                        std::list<Point> &pointsInFile,
                        bool &invert);

    /** \brief Last Cavendish iteration, only 4 segments left
     * \param[out] segmentsInFile
     * \param[out] segmentsInContour
     * \param[out] fElemMesh The list filled with the finite elements
     *
     * Compute diagonals length and keep the smallest one.
     */
    void
    lastPolygonCavendish (std::list<Segment> &segmentsInFile,
                          std::list<Segment> &segmentsInContour,
                          std::list<fElem> &fElemMesh);
};

#endif
