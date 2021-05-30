cmake_minimum_required(VERSION 3.4)
 
project( cerreal )

INCLUDE_DIRECTORIES(
    . include gen/cpp gen messaging visionipc
)

# find_package(capnproto REQUIRED)

SET(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations -std=c++14 -fPIC -O2 -Wall -m64 ")

AUX_SOURCE_DIRECTORY(messaging DIR_MESSAGING_SRCS)
AUX_SOURCE_DIRECTORY(visionipc DIR_VISIONIPC_SRCS)

ADD_LIBRARY(${CMAKE_PROJECT_NAME} SHARED ${DIR_MESSAGING_SRCS} ${DIR_VISIONIPC_SRCS})
# ADD_LIBRARY(${CMAKE_PROJECT_NAME} STATIC ${DIR_MESSAGING_SRCS})
 
# SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME "TPV")
# SET_TARGET_PROPERTIES(TPV_static PROPERTIES OUTPUT_NAME "TPV")
 
SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES VERSION 1.2 SOVERSION 1)

message("Compile depend libs:" ${COMP_DEP_MODULES})
target_link_libraries( ${CMAKE_PROJECT_NAME} "-Wl,--whole-archive" "-Wl,--no-whole-archive" capnp OpenCL zmq)
	 

ADD_SUBDIRECTORY(demo)

############################################################################
file(GLOB_RECURSE PROTOBUF_SRC_LIST ${CMAKE_CURRENT_LIST_DIR}/**.capnp)
execute_process(COMMAND mkdir -p ${CMAKE_CURRENT_LIST_DIR}/gen/cpp)

add_custom_target(generate_capnp_target
    #COMMAND echo " generate protobuf target "
    COMMENT "Generate capnp files"
)

foreach(v ${PROTOBUF_SRC_LIST})
    string(REGEX REPLACE ".*/\(.*\)" "\\1" file_name ${v})
    # string(REGEX REPLACE ${file_name} "" target_path ${v})
    
	add_custom_command(
        TARGET generate_capnp_target
		PRE_BUILD

        COMMAND capnp compile --src-prefix=${CMAKE_CURRENT_LIST_DIR}  -o c++:${CMAKE_CURRENT_LIST_DIR}/gen/cpp  ${v}
        COMMENT "compile capnp file: capnp compile -o c++:${CMAKE_CURRENT_LIST_DIR}/gen/cpp  ${v}"    

    )
endforeach()

add_custom_command(
        TARGET generate_capnp_target
		PRE_BUILD
        COMMAND python3  ${CMAKE_CURRENT_LIST_DIR}/services.py > ${CMAKE_CURRENT_LIST_DIR}/gen/services.h
        COMMENT "Generate sevices.h: python3  ${CMAKE_CURRENT_LIST_DIR}/services.py > ${CMAKE_CURRENT_LIST_DIR}/gen/services.h"
)

############################################################################





