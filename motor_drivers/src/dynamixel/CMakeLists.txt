add_library(dynamixel_module 
                lib/DatosDynamixel.c lib/dynamixel.c lib/dxl_hal.c
                DynamixelDriver.cpp DynamixelConfiguration.cpp
                DynamixelMotorSensor.cpp DynamixelStatus.cpp)
target_link_libraries(dynamixel_module mxml_utils timer driver_interface mxml_utils pthread)

install(TARGETS dynamixel_module
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

add_executable(test_Dynamixel.exe TestDynamixel.cpp)
target_link_libraries(test_Dynamixel.exe dynamixel_module)

IF(NOT AD_USE_ROS)
  add_executable(test_DynamixelPV.exe test_DynamixelPV.cpp)
  target_link_libraries(test_DynamixelPV.exe dynamixel_module Actuator)
ENDIF(NOT AD_USE_ROS)
