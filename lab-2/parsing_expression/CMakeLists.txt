project(ParsingExpression)

enable_testing()

add_library(parsing_expression_lib ExpressionParser.cpp ExpressionParser.h)

add_executable(ParsingExpression main.cpp)
target_link_libraries(ParsingExpression parsing_expression_lib)

add_subdirectory(tests)