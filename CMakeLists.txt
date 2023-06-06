cmake_minimum_required(VERSION 3.16)
project(AirportSim)

set(CMAKE_CXX_STANDARD 17)
set(SST_INCLUDE_DIR "$ENV{SST_CORE_HOME}/include")
set(PYTHON_FILE
        execution_scripts/scratch/airport_sim.py
        execution_scripts/scratch/airport_sim2.py

        )

set(SOURCE_FILES
		Common.hpp
		Airport.hpp
		Airport.cpp
		AirportEvent.hpp
		)

add_custom_target(CopyPython ALL
        DEPENDS ${CMAKE_SOURCE_DIR}/${PYTHON_FILE}
        )

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${SST_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/src
)

add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND sst-register ${PROJECT_NAME} ${PROJECT_NAME}_LIBDIR=${CMAKE_CURRENT_BINARY_DIR}
        VERBATIM
)
add_custom_command(TARGET CopyPython
        POST_BUILD
        COMMAND cp -r ${CMAKE_SOURCE_DIR}/execution_scripts ${CMAKE_CURRENT_BINARY_DIR}/
        VERBATIM
)

