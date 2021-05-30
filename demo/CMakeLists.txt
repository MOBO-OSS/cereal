
add_executable(message_demo demo.cc)

SET(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations -std=c++14 -pthread -fPIC -O2 -Wall -m64 ")

#target_link_libraries( TPV_bin "-Wl,--whole-archive" TPV_static   TOOLS "-Wl,--no-whole-archive" png ${OpenCV_LIBS} glog  EGL osg osgViewer osgGA osgDB osgText osgManipulator OpenThreads ${PROTOBUF_LIBRARY})
target_link_libraries( message_demo cerreal pthread )


