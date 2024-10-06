add_executable (lk lk.c)
target_link_libraries(lk helper ${CMAKE_THREAD_LIBS_INIT})

add_executable(lk-cpp lk.cpp)
target_link_libraries(lk-cpp helper-cpp ${CMAKE_THREAD_LIBS_INIT})
