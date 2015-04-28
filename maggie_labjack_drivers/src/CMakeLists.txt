add_library(ljacklm_wrapper
            ljacklm_wrapper.cpp)
target_link_libraries(ljacklm_wrapper
                      ljacklm m ${catkin_LIBRARIES})

install(TARGETS ljacklm_wrapper
    ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
