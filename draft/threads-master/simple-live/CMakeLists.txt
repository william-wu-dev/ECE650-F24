add_executable (simple-live-bug simple-live-bug.cpp)
target_link_libraries(simple-live-bug ${CMAKE_THREAD_LIBS_INIT})


add_executable (simple-live-fix-print simple-live-fix-print.cpp)
target_link_libraries(simple-live-fix-print ${CMAKE_THREAD_LIBS_INIT})

add_executable (simple-live-fix-mutex simple-live-fix-mutex.cpp)
target_link_libraries(simple-live-fix-mutex ${CMAKE_THREAD_LIBS_INIT})