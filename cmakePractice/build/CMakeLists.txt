project(cmakePractice)
 
set(ROOT ..)
set(LIB_DIR ../lib)
set(MAIN_DIR ../source)
set(COMMON_DIR ../include)
set(OUTPUT_DIR ../out)
 
set(SOURCE 
		${MAIN_DIR}/main.cpp 
		${MAIN_DIR}/app.cpp)
		
set(OUTPUT
		${OUTPUT_DIR}/program)

# This is optional.
#set(EXE_FILE program)

cmake_minimum_required(VERSION 2.8)
message(${CMAKE_PROJECT_NAME})
add_compile_options(-W -Wall)
 
include_directories(${COMMON_DIR})
add_executable(${OUTPUT} ${SOURCE})

# These are optional. 
#install(TARGETS ${OUTPUT} ${LIB_NAME}
#        RUNTIME DESTINATION /home/bin
#        LIBRARY DESTINATION /home/lib)