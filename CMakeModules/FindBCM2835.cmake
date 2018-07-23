find_path(
  BCM2835_INCLUDE_DIR
  NAMES bcm2835.h
  PATHS /usr/local/include/ /usr/include/
)

find_library(
  BCM2835_LIBRARY
  NAMES bcm2835
  PATHS /usr/local/lib/ /usr/lib/
)

if (BCM2835_INCLUDE_DIR AND BCM2835_LIBRARY)
  set(BCM2835_FOUND TRUE)
endif (BCM2835_INCLUDE_DIR AND BCM2835_LIBRARY)

if (BCM2835_FOUND)
	message(STATUS "Found BCM 2835 Libs: ${BCM2835_LIBRARY}")
	message(STATUS "Found BCM 2835 Incs: ${BCM2835_INCLUDE_DIR}")
else (BCM2835_FOUND)
	message(FATAL_ERROR "Could not find BCM 2835 library")
endif (BCM2835_FOUND)
