project(${PROJECT})

add_library(hash-calculator hash-calculator.c)

target_include_directories(
  hash-calculator
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(
  hash-calculator
  PRIVATE
  OpenSSL::Crypto
)
