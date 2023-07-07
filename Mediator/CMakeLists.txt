add_executable(Mediator Mediator.cpp Main.cpp)
target_include_directories(Mediator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(Mediator PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build)
