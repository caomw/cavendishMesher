/**
 * \file TextProcessing.hpp
 * \brief TextProcessing class definition.
 * \author Victor Lamoine
 * \version 0.1
 * \date may 2014
 */

#ifndef TEXTPROCESSING_HPP
#define TEXTPROCESSING_HPP

#include "mesh.hpp"

/** \brief A class to read/write text from/into files/terminal
 *
 * This object provides methods to read or write data text files
 * and print information in terminal.
 */
class TextProcessing
{
  public:
    TextProcessing ();

    /** \brief Read a contour to be meshed from a GEO text file
     * \param[in] filename the name of the input file
     * \param[out] pointsInFile the list of points to be filled
     * \param[out] segmentsInFile the list of segments to be filled
     * \return true if successful, false otherwise.
     *
     * GEO files are generated with RDM6 software.
     * The format/syntax is copied.
     *
     */
    bool
    readCalFile (const std::string filename,
                 std::list<Point> &pointsInFile,
                 std::list<Segment> &segmentsInFile);

    /** \brief Write a computed mesh into a CAL text file
     * \param[out] filename the name of the output file
     * \param[in] fElemMesh the list of finite elements to be written
     * \param[in] pointsInFile the list of points to be written
     * \param[in] segmentsInFile the list of segments to be written
     * \param[in] segmentsInContour_bak the list of segments in the original contour
     * \return true if successful, false otherwise.
     *
     * CAL files can also be generated with RDM6 software.
     * The format/syntax is copied.
     *
     */
    bool
    writeCalFile (const std::string filename,
                  std::list<fElem> &fElemMesh,
                  std::list<Point> &pointsInFile,
                  std::list<Segment> &segmentsInFile,
                  std::list<Segment> &segmentsInContour_bak);

    /** \brief Print a list of points in the terminal
     * \param[in] points the list of points to be displayed
     */
    void
    printPoints (std::list<Point> &points);

    /** \brief Print a list of segments in the terminal
     * \param[in] segments the list of segments to be displayed
     */
    void
    printSegments (std::list<Segment> &segments);

    /*  \brief Print a list of finite elements in the terminal
     * \param[in] Elements the list of finite elements to be displayed
     */
    //void print_fElems(std::list<fElem>* Elements);
};

#endif
