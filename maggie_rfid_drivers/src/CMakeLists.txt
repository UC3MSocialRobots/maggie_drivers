add_library(rfid_driver_wrapper
            rfid_driver_wrapper.cpp rfid_driver_wrapper.h rfid_driver_data.h rfid_driver_interface.h)
target_link_libraries(rfid_driver_wrapper
                      feusb feisc FedmIscCore fecom fefu ${catkin_LIBRARIES})

install(TARGETS rfid_driver_wrapper
    ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
