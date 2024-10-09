add_executable(cpp-live-bug cpp-live-bug.cpp)  
target_link_libraries(cpp-live-bug ${CMAKE_THREAD_LIBS_INIT})

add_executable(cpp-live-bug-fix cpp-live-bug-fix.cpp)  
target_link_libraries(cpp-live-bug-fix ${CMAKE_THREAD_LIBS_INIT})