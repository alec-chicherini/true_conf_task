#True conf task CMAKE
cmake_minimum_required (VERSION 3.8)

project ("true_conf_task")

add_executable (true_conf_task "true_conf_task.cpp" )
set_property(TARGET true_conf_task PROPERTY CXX_STANDARD 11)
