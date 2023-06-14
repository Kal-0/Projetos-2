cmake_minimum_required(VERSION 3.5)  # Set the minimum required version of CMake.

project(HClass)  # Set the project name.

set(SOURCES main.c utils.c sqlite3/sqlite3.c)  # Define the source files for the project.

add_executable(HClass ${SOURCES})  # Create an executable named "HClass" from the source files.



target_include_directories(HClass PUBLIC
    .
    ./sqlite3/

)   # Add the headers directories directory to the include path.


install(TARGETS HClass DESTINATION executables)  # Install the "HClass" executable to the "executables" directory.
