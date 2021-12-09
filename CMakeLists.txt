# The first line states the minimum CMake version to use with this project
# and is requires by CMake.
cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 11)

# Set <PROJNAME> to be your project name. We set CXX (C++) here as the target language.
project(Mivney_1 VERSION 0.1.0 LANGUAGES CXX)

# If you want to see "under the hood" of CMake, uncomment this:
# set(CMAKE_VERBOSE_MAKEFILE ON)

# Define variables containing compiler flags for gcc
set(MIVNEY_FLAGS_DEBUG "-std=c++11 --pedantic-errors -Wall -Werror")
set(MIVNEY_FLAGS_RELEASE "${MIVNEY_FLAGS_DEBUG} -DNDEBUG")

# Set the flags for gcc
set(CMAKE_CPP_FLAGS ${MIVNEY_FLAGS_DEBUG})


# Add the names of your source files here
set(CPP_FILES_A1 "AVL_Tests/small_main.cpp" "./AVLtree.h" "course_files/library1.h")
set(CPP_FILES_A2 "AVL_Tests/tests1.cpp" "./AVLtree.h" "course_files/library1.h")
set(CPP_FILES_B1 "course_files/main1.cpp" "./library1.cpp" "course_files/library1.h" "./PlayersManager.cpp" "./PlayersManager.h" "./AVLtree.h" )

add_executable(small_AVL_test ${CPP_FILES_A1})
add_executable(tests1_AVL ${CPP_FILES_A2})
add_executable(main_pm ${CPP_FILES_B1})

#  enable_testing()
#  foreach(i RANGE 0 99)
#      message(FATAL_ERROR $<TARGET_FILE:main_pm>$)
#      add_test(NAME almog_${i} COMMAND bash -c "$<TARGET_FILE:main_pm> < ${PROJECT_SOURCE_DIR}/pm_tests/wet1_tests/input_{i}.txt | diff ${PROJECT_SOURCE_DIR}/pm_tests/wet1_tests/out_{i}.txt -")
#  endforeach()

