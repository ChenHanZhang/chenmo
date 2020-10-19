add_executable(eventloop_unittest Eventloop_unittest.cc)
target_link_libraries(eventloop_unittest chenmo_net)


add_executable(reactor_unittest Reactor_unittest.cc)
target_link_libraries(reactor_unittest chenmo_net)