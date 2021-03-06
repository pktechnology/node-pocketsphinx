project(SphinxBase)
set (CMAKE_CXX_STANDARD 11)

execute_process(COMMAND pkg-config --variable datadir sphinxbase OUTPUT_VARIABLE SB_DATADIR OUTPUT_STRIP_TRAILING_WHITESPACE)
add_custom_command(OUTPUT "sb.cpp"
                   COMMAND swig -I${SB_DATADIR}/swig -javascript -node -DV8_VERSION=0x032872 -o sb.cpp ${SB_DATADIR}/swig/sphinxbase.i
                   COMMENT "Generating swig bindings" )

link_directories(${PROJECT_NAME} ${SB_LIBRARY_DIRS})
add_definitions(-DBUILDING_NODE_EXTENSION)

add_library(${PROJECT_NAME} SHARED "sb.cpp")
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

target_include_directories(${PROJECT_NAME} PRIVATE ${SB_INCLUDE_DIRS} ${CMAKE_JS_INC})
target_link_libraries(${PROJECT_NAME} ${SB_LIBRARIES} ${CMAKE_JS_LIB})
