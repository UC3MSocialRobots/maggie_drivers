add_library(irtrans_wrapper
            irtrans_wrapper.cpp irtrans_wrapper.h ir_driver_interface.h)
target_link_libraries(irtrans_wrapper
                      IRTrans ${catkin_LIBRARIES})

install(TARGETS irtrans_wrapper
    ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
