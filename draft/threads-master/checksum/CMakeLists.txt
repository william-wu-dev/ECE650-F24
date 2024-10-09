add_executable(checksum checksum.c)

add_executable(checksum-threaded checksum-threaded.c)  
target_link_libraries(checksum-threaded ${CMAKE_THREAD_LIBS_INIT})

add_executable(checksum-threaded-bug-enhanced  
		checksum-threaded-bug-enhanced.c)  
target_link_libraries(checksum-threaded-bug-enhanced
		${CMAKE_THREAD_LIBS_INIT})

add_executable(checksum-my-lock checksum-my-lock.c)  
target_link_libraries(checksum-my-lock ${CMAKE_THREAD_LIBS_INIT})

add_executable(checksum-threaded-bug-fix-c checksum-threaded-bug-fix-c.c)  
target_link_libraries(checksum-threaded-bug-fix-c ${CMAKE_THREAD_LIBS_INIT})
