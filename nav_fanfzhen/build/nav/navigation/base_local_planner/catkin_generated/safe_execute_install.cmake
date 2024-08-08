execute_process(COMMAND "/home/hyb/nav_fanfzhen/build/nav/navigation/base_local_planner/catkin_generated/python_distutils_install.sh" RESULT_VARIABLE res)

if(NOT res EQUAL 0)
  message(FATAL_ERROR "execute_process(/home/hyb/nav_fanfzhen/build/nav/navigation/base_local_planner/catkin_generated/python_distutils_install.sh) returned error code ")
endif()
