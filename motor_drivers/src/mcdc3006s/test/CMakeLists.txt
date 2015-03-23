add_executable(test_mcdc3006sDriver.exe
               testMCDC3006SDriver.cpp)
target_link_libraries(test_mcdc3006sDriver.exe
                      mcdc3006s_module)
