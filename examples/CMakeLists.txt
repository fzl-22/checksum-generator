project(${PROJECT})

add_executable(checksum-gen main.c)

target_link_libraries(
  checksum-gen
  PRIVATE
  hash-calculator
  OpenSSL::Crypto
)