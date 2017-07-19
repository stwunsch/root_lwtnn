find_package(PkgConfig)
pkg_check_modules(PC_LWTNN QUIET liblwtnn)

find_path(
    LWTNN_INCLUDE_DIRS
    NAMES lwtnn/LightweightNeuralNetwork.hh
    HINTS ${PC_LWTNN_INCLUDEDIR}
          ${PC_LWTNN_INCLUDE_DIRS}
    PATH_SUFFIXES lwtnn
)


find_library(
    LWTNN_LIBRARIES
    NAMES liblwtnn.so
    HINTS ${PC_LIBLWTNN_LIBDIR}
          ${PC_LIBLWTNN_LIBRARY_DIRS}
)

if(LWTNN_LIBRARIES AND LWTNN_INCLUDE_DIRS)
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(LWTNN DEFAULT_MSG LWTNN_LIBRARIES LWTNN_INCLUDE_DIRS)
    mark_as_advanced(LWTNN_LIBRARIES LWTNN_INCLUDE_DIRS)
else()
    message(FATAL_ERROR "lwtnn is required, but was not found.")
endif()
