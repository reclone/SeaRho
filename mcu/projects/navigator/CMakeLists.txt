cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(navigator
        EXTRA_COMPONENT_DIRS ../../components
        COMPONENTS spartan7_sserial_config)
