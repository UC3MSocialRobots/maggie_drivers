add_library(mcdc3006s
            MCDC3006S.c MCDC3006S.h MCDC3006S_data.h)
target_link_libraries(mcdc3006s
                      semaphores rs232 timer ${catkin_LIBRARIES})

add_executable(test_mcdc3006s.exe
               testMCDC3006S.c)
target_link_libraries(test_mcdc3006s.exe
                      mcdc3006s)

install(TARGETS mcdc3006s
    ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
    RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
