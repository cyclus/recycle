ADD_SUBDIRECTORY(pyroprocessing)

USE_CYCLUS("recycle" "diverter")

#INSTALL_CYCLUS_MODULE("" "toolkit" "NONE")

#install header files
FILE(GLOB h_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

