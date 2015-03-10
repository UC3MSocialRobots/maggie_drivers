# directories to be processed by cmake (containing CMakeLists.txt)
SET(DIRECTORIES MCDC3006Slib)

FOREACH(DIR ${DIRECTORIES})
    add_subdirectory(${DIR})
ENDFOREACH(DIR)

add_library(mcdc3006s_module 
            MCDC3006SDriver.cpp MCDC3006SConfiguration.cpp
            MCDC3006SMotorSensor.cpp MCDC3006SStatus.cpp)
target_link_libraries(mcdc3006s_module mxml_utils mcdc3006s driver_interface pthread )

add_executable(test_mcdc3006sDriver.exe testMCDC3006SDriver.cpp)
target_link_libraries(test_mcdc3006sDriver.exe mcdc3006s_module)

install(TARGETS mcdc3006s_module
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
