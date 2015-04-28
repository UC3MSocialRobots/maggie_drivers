add_library(serial_communication
            serial_communication.cpp)
target_link_libraries(serial_communication
                      ${catkin_LIBRARIES})

install(TARGETS serial_communication
    ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

#ADD_SUBDIRECTORY(test)
