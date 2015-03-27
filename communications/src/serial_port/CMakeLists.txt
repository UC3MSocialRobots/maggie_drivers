add_library(rs232
            rs232.cpp rs232.h)
target_link_libraries(rs232
                      semaphores ${catkin_LIBRARIES})

install(TARGETS rs232
    ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

add_subdirectory(test)
