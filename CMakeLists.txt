cmake_minimum_required (VERSION 3.16)
project (contours)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable (contours contours.cpp)
target_link_libraries(contours ${OpenCV_LIBS} )

add_executable (contours_th contours_th.cpp)
target_link_libraries(contours_th ${OpenCV_LIBS} )

add_executable (contours_inex contours_inex.cpp)
target_link_libraries(contours_inex ${OpenCV_LIBS} )

add_executable (edges edges.cpp)
target_link_libraries(edges ${OpenCV_LIBS} )

add_executable (mocap mocap.cpp)
target_link_libraries(mocap ${OpenCV_LIBS} )

add_executable (histogram histogram.cpp)
target_link_libraries(histogram ${OpenCV_LIBS} )

add_executable (histogram_rc histogram_rc.cpp)
target_link_libraries(histogram_rc ${OpenCV_LIBS} )
