add_executable(Tietogra Main.cpp Map.cpp)
target_include_directories(Tietogra PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(Tietogra PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build)
