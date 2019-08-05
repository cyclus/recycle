USE_CYCLUS("pyro" "process")
USE_CYCLUS("pyro" "pyre_volox")
USE_CYCLUS("pyro" "pyre_reduction")
USE_CYCLUS("pyro" "pyre_refining")
USE_CYCLUS("pyro" "pyre_winning")

INSTALL_CYCLUS_MODULE("pyro" "" "NONE")

#install header files
FILE(GLOB h_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
