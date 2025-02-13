cmake_minimum_required(VERSION 3.14)
project(practice_googleTest2)

get_filename_component(PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(INCLUDE_DIR ${PARENT_DIR}/app/include)
set(SOURCE_DIR ${PARENT_DIR}/app/source)
set(MOCK_DIR ${PARENT_DIR}/unit_test/mocks)

enable_testing()

add_executable(
	# binary
	hello_test
	
	# tests
	hello_test.cpp
	
	# sources
	${SOURCE_DIR}/app.cpp
	
	# mocks
	${MOCK_DIR}/mockHardware.cpp
)

target_link_libraries(
	hello_test
	gtest_main
	gmock
)

include_directories(${INCLUDE_DIR}
					${MOCK_DIR})
					
include(GoogleTest)
		
gtest_discover_tests(hello_test)