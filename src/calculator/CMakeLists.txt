project(Calculator)

add_library(${PROJECT_NAME} STATIC
    Runner.cpp
    State.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Parser
    PRIVATE Evaluator
)
