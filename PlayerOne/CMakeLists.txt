add_executable(PlayerOne Actors.cpp Main.cpp OrdersAndAI.cpp)
target_include_directories(PlayerOne PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(PlayerOne PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build)
