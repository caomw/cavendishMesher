cmake_minimum_required (VERSION 2.8.8 FATAL_ERROR)

project (mesh)
set (CMAKE_C_FLAGS "-Wall -Wpedantic")
find_package (OpenCV 2.4.6 REQUIRED)

add_library (TextProcessing_obj OBJECT TextProcessing.cpp)
add_library (TextProcessing $<TARGET_OBJECTS:TextProcessing_obj>)

add_library (GeometryProcessing_obj OBJECT GeometryProcessing.cpp)
add_library (GeometryProcessing $<TARGET_OBJECTS:GeometryProcessing_obj>)

add_library (OpenCV_obj OBJECT OpenCV.cpp)
add_library (OpenCV $<TARGET_OBJECTS:OpenCV_obj>)

add_executable (mesh mesh.cpp)
target_link_libraries (mesh TextProcessing GeometryProcessing OpenCV ${OpenCV_LIBS})


option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ON)
IF(BUILD_DOCUMENTATION)

  find_package(Doxygen)
  IF(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation.")
  ENDIF()

  # Path the images
  set(doxy_images            ${PROJECT_SOURCE_DIR}/documentation) # PROJECT_LOGO and IMAGE_PATH in doxy.in
  set(doxy_input             ${PROJECT_SOURCE_DIR}) # INPUT in doxy.in
  set(doxyfile_in            ${CMAKE_CURRENT_SOURCE_DIR}/documentation/cavendishMesher.doxy.in)
  set(doxyfile               ${PROJECT_BINARY_DIR}/cavendishMesher.doxy)
  set(doxy_html_index_file   ${CMAKE_CURRENT_BINARY_DIR}/cavendishMesher_documentation/index.html)
  set(doxy_output_root       ${CMAKE_CURRENT_BINARY_DIR}/cavendishMesher_documentation) # OUTPUT_DIRECTORY in doxy.in

  configure_file(${doxyfile_in} ${doxyfile} @ONLY)

  add_custom_command(OUTPUT ${doxy_html_index_file}
                     COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
                     MAIN_DEPENDENCY ${doxyfile} 
                     DEPENDS mesh
                     COMMENT "Generating HTML documentation")
 
  add_custom_target(doc ALL DEPENDS ${doxy_html_index_file})
ENDIF()
