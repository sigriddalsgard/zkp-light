cmake_minimum_required(VERSION 4.0.2)
project(zkp C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SODIUM REQUIRED libsodium)
pkg_check_modules(GMP REQUIRED gmp)

include_directories(${SODIUM_INCLUDE_DIRS} ${GMP_INCLUDE_DIRS})
link_directories(${SODIUM_LIBRARY_DIRS} ${GMP_LIBRARY_DIRS})

add_executable(commit commit.c)
target_link_libraries(commit ${SODIUM_LIBRARIES} ${GMP_LIBRARIES})

add_executable(verify verify.c)
target_link_libraries(verify ${SODIUM_LIBRARIES} ${GMP_LIBRARIES})
