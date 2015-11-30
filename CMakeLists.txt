project(ieProxy)

set(SRC_LIST ieProxy.cpp)
add_executable(ieProxy ${SRC_LIST})
target_link_libraries(ieProxy wininet)