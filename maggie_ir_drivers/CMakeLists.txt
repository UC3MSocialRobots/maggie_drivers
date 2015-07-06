cmake_minimum_required(VERSION 2.8.3)
project(maggie_ir_drivers)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS roscpp)

# install third parties
execute_process(COMMAND sudo bash install.bash
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/third_parties/IRTrans_shlib/
)

###################################
## catkin specific configuration ##
###################################

## The catkin_package macro generates cmake config files for your package
catkin_package(
    INCLUDE_DIRS include
    CATKIN_DEPENDS roscpp
    DEPENDS
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
    include src
    ${catkin_INCLUDE_DIRS}
    /usr/local/include/irtrans
)

ADD_SUBDIRECTORY(src)

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}/
    DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
    PATTERN ".svn" EXCLUDE
)

install(DIRECTORY /usr/local/include/irtrans
    DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
    PATTERN ".svn" EXCLUDE
)
