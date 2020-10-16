set(base_SRCS
        CurrentThread.cc
        Date.cc
        Logging.cc
        LogStream.cc
        Timestamp.cc
        TimeZone.cc
        Condition.cc
        CountDownLatch.cc
        Date.cc
        Exception.cc
        Thread.cc
        )

add_library(chenmo_base ${base_SRCS})
target_link_libraries(chenmo_base pthread rt)

#add_library(muduo_base_cpp11 ${base_SRCS})
#target_link_libraries(muduo_base_cpp11 pthread rt)
#set_target_properties(muduo_base_cpp11 PROPERTIES COMPILE_FLAGS "-std=c++0x")

install(TARGETS chenmo_base DESTINATION lib)
#install(TARGETS muduo_base_cpp11 DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/chenmo/base)

#if(MUDUO_BUILD_EXAMPLES)
#    add_subdirectory(tests)
#endif()
