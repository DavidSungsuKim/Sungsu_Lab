project(example)
 
set(ROOT ..)
set(LIB_DIR ../lib)
set(MAIN_DIR ../main)
set(COMMON_DIR ../common)
 
set(LIB_LIST ${LIB_DIR}/libtest.cpp)
set(LIB_NAME libtest)
set(SOURCE ${MAIN_DIR}/main.cpp ${MAIN_DIR}/function.cpp)
set(OUTPUT ${ROOT}/program.out)
set(EXE_FILE program)
 
cmake_minimum_required(VERSION 2.8)
message(${CMAKE_PROJECT_NAME})
add_compile_options(-W -Wall)
 
include_directories(${COMMON_DIR})
add_executable(${OUTPUT} ${SOURCE})
 
add_library(${LIB_NAME} SHARED ${LIB_LIST})
target_link_libraries(${OUTPUT} PUBLIC ${LIB_NAME})
 
install(TARGETS ${OUTPUT} ${LIB_NAME}
        RUNTIME DESTINATION /home/bin
        LIBRARY DESTINATION /home/lib)