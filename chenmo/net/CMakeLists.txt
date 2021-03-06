include(CheckFunctionExists)

check_function_exists(accept4 HAVE_ACCEPT4)
if(NOT HAVE_ACCEPT4)
    set_source_files_properties(SocketsOps.cc PROPERTIES COMPILE_FLAGS "-DNO_ACCEPT4")
endif()

set(net_SRCS
        Acceptor.cc
        Buffer.cc
        Channel.cc
        EventLoop.cc
        EventLoopThread.cc
        EventLoopThreadPool.cc
        InetAddress.cc
        Poller.cc
        poller/DefaultPoller.cc
        poller/PollPoller.cc
        Socket.cc
        SocketsOps.cc
        TcpConnection.cc
        TcpServer.cc
        Timer.cc
        TimerQueue.cc
        )

add_library(chenmo_net ${net_SRCS})
target_link_libraries(chenmo_net chenmo_base)

#add_library(muduo_net_cpp11 ${net_SRCS})
#target_link_libraries(muduo_net_cpp11 muduo_base_cpp11)
#set_target_properties(muduo_net_cpp11 PROPERTIES COMPILE_FLAGS "-std=c++0x")

install(TARGETS chenmo_net DESTINATION lib)
#install(TARGETS muduo_net_cpp11 DESTINATION lib)

set(HEADERS
        Buffer.h
        Callbacks.h
        Channel.h
        Endian.h
        EventLoop.h
        EventLoopThread.h
        InetAddress.h
        TcpConnection.h
        TcpServer.h
        TimerId.h
        )
install(FILES ${HEADERS} DESTINATION include/chenmo/net)

if(MUDUO_BUILD_EXAMPLES)
    add_subdirectory(tests)
endif()
