# npm install -g cmake-js
cmake_minimum_required(VERSION 3.1)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(MODULE_NAME "Test_Module")
SET(MODULE_VERSION 0.0.0)

project(${MODULE_NAME} VERSION ${MODULE_VERSION})

# Include N-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})

set(MODULE_SOURCES
   ${CMAKE_JS_SRC}
   module.cpp
)

set(MODULE_INCLUDE_DIRECTORIES
   ${CMAKE_JS_INC}
   ${NODE_ADDON_API_DIR}
)

set(MODULE_OS ${CMAKE_SYSTEM_NAME})
if ("${MODULE_OS}" STREQUAL "Darwin")
  set(MODULE_OS "Mac")
endif()

add_library(${MODULE_NAME} SHARED ${MODULE_SOURCES})
set_property(TARGET ${MODULE_NAME} PROPERTY CXX_STANDARD 17)
set_target_properties(${MODULE_NAME} PROPERTIES OUTPUT_NAME "${MODULE_NAME}_${MODULE_OS}")
set_target_properties(${MODULE_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_include_directories(${MODULE_NAME} PRIVATE ${MODULE_INCLUDE_DIRECTORIES})
target_link_libraries(${MODULE_NAME} ${CMAKE_JS_LIB})
install(TARGETS ${MODULE_NAME} DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../electron/libs)
