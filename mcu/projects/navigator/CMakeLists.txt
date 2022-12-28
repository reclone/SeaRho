cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

set (EXTRA_COMPONENT_DIRS ../../components)

project(navigator)

spiffs_create_partition_image (assets assets FLASH_IN_PROJECT)
