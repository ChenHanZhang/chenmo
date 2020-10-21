add_executable(eventloop_unittest Eventloop_unittest.cc)
target_link_libraries(eventloop_unittest chenmo_net)


add_executable(channel_unittest Channel_unittest.cc)
target_link_libraries(channel_unittest chenmo_net)


add_executable(eventloopthread_unittest EventLoopThread_unittest.cc)
target_link_libraries(eventloopthread_unittest chenmo_net)