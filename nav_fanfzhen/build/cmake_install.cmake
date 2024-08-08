# Install script for directory: /home/hyb/nav_fanfzhen/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/hyb/nav_fanfzhen/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  
      if (NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}")
        file(MAKE_DIRECTORY "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}")
      endif()
      if (NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/.catkin")
        file(WRITE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/.catkin" "")
      endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/hyb/nav_fanfzhen/install/_setup_util.py")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/hyb/nav_fanfzhen/install" TYPE PROGRAM FILES "/home/hyb/nav_fanfzhen/build/catkin_generated/installspace/_setup_util.py")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/hyb/nav_fanfzhen/install/env.sh")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/hyb/nav_fanfzhen/install" TYPE PROGRAM FILES "/home/hyb/nav_fanfzhen/build/catkin_generated/installspace/env.sh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/hyb/nav_fanfzhen/install/setup.bash;/home/hyb/nav_fanfzhen/install/local_setup.bash")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/hyb/nav_fanfzhen/install" TYPE FILE FILES
    "/home/hyb/nav_fanfzhen/build/catkin_generated/installspace/setup.bash"
    "/home/hyb/nav_fanfzhen/build/catkin_generated/installspace/local_setup.bash"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/hyb/nav_fanfzhen/install/setup.sh;/home/hyb/nav_fanfzhen/install/local_setup.sh")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/hyb/nav_fanfzhen/install" TYPE FILE FILES
    "/home/hyb/nav_fanfzhen/build/catkin_generated/installspace/setup.sh"
    "/home/hyb/nav_fanfzhen/build/catkin_generated/installspace/local_setup.sh"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/hyb/nav_fanfzhen/install/setup.zsh;/home/hyb/nav_fanfzhen/install/local_setup.zsh")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/hyb/nav_fanfzhen/install" TYPE FILE FILES
    "/home/hyb/nav_fanfzhen/build/catkin_generated/installspace/setup.zsh"
    "/home/hyb/nav_fanfzhen/build/catkin_generated/installspace/local_setup.zsh"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/hyb/nav_fanfzhen/install/.rosinstall")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/hyb/nav_fanfzhen/install" TYPE FILE FILES "/home/hyb/nav_fanfzhen/build/catkin_generated/installspace/.rosinstall")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/hyb/nav_fanfzhen/build/gtest/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/nav/navigation/navigation/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/gamappig/openslam_gmapping/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/gamappig/slam_gmapping/slam_gmapping/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/nav/navigation_msgs/move_base_msgs/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/nav/map_server/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/goal_publisher/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/nav/navigation_msgs/map_msgs/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/gamappig/slam_gmapping/gmapping/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/nav/amcl/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/nav/navigation/fake_localization/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/nav/navigation/voxel_grid/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/nav/navigation/costmap_2d/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/nav/navigation/nav_core/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/nav/navigation/base_local_planner/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/nav/navigation/carrot_planner/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/nav/navigation/clear_costmap_recovery/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/nav/navigation/dwa_local_planner/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/nav/navigation/move_slow_and_clear/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/nav/navigation/navfn/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/nav/navigation/global_planner/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/nav/navigation/rotate_recovery/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/nav/navigation/move_base/cmake_install.cmake")
  include("/home/hyb/nav_fanfzhen/build/urdf_demo/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/hyb/nav_fanfzhen/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
