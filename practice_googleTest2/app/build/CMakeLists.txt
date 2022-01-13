cmake_minimum_required(VERSION 3.1)
project(app)

get_filename_component(PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)
#message(${PARENT_DIR}) 

set(CMAKE_CXX_STANDARD 20)
set(SOURCE_DIR ${PARENT_DIR}/source)
set(INCLUDE_DIR ${PARENT_DIR}/include)
 
set(SOURCE ${SOURCE_DIR}/main.cpp 
		   ${SOURCE_DIR}/app.cpp)
		   
#set(OUTPUT ${PARENT_DIR}/program)
#message(${OUTPUT}) 
 
if (!WIN32)
add_compile_options(-W -Wall)
endif()

include_directories(${INCLUDE_DIR})
add_executable(program #${OUTPUT} 
		       ${SOURCE})