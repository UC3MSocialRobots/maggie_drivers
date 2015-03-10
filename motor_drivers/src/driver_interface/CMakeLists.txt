add_library(driver_interface 
            DriverInterface.cpp DriverConfiguration.cpp
            DriverMotorSensor.cpp DriverStatus.cpp)
target_link_libraries(driver_interface rs232 ${catkin_LIBRARIES})

install(TARGETS driver_interface
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
