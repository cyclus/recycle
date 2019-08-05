USE_CYCLUS("recycle" "process")
USE_CYCLUS("recycle" "pyre_volox")
USE_CYCLUS("recycle" "pyre_reduction")
USE_CYCLUS("recycle" "pyre_refining")
USE_CYCLUS("recycle" "pyre_winning")

#INSTALL_CYCLUS_MODULE("" "pyro" "NONE")

#install header files
FILE(GLOB h_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
