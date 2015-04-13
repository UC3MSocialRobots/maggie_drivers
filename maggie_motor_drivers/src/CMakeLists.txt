add_library(mcdc3006s
            mcdc3006s.cpp mcdc3006s.h mcdc3006s_data.h motor_driver_interface.h)
target_link_libraries(mcdc3006s
                      semaphores rs232 timer ${catkin_LIBRARIES})

install(TARGETS mcdc3006s
    ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
