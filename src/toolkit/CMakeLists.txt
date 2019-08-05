ADD_SUBDIRECTORY(pyroprocessing)

USE_CYCLUS("toolkit" "diverter")

INSTALL_CYCLUS_MODULE("toolkit" "" "NONE")
TARGET_LINK_LIBRARIES("toolkit" "pyro")

#install header files
FILE(GLOB h_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

