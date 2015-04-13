cmake_minimum_required(VERSION 2.8.3)
project(labjack_drivers)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS roscpp)

###################################
## catkin specific configuration ##
###################################

## The catkin_package macro generates cmake config files for your package
catkin_package(
  INCLUDE_DIRS src
  CATKIN_DEPENDS roscpp
  DEPENDS
)

###########
## Build ##
###########

include_directories(
  include src
  ${catkin_INCLUDE_DIRS}
)

add_subdirectory(src)

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

install(DIRECTORY src/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
  PATTERN ".svn" EXCLUDE
)
