project(Calculator-Challenge)

include_directories(./)
add_subdirectory(parser)
add_subdirectory(evaluator)
add_subdirectory(calculator)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Calculator
)
