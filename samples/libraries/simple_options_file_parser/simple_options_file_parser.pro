QT       += core gui widgets

#
# Name of the output target. It depends on the OS,
# (Windows, Linux) the platform (x86, x64) and the
# build mode (release, debug).
#

win32 {

    contains(QT_ARCH, i386) {
      CONFIG(release, debug|release) {
        TARGET = simple_options_file_parser_mtd
      }
      CONFIG(debug, debug|release) {
        TARGET = simple_options_file_parser_dmtd
      }
    } else {
      CONFIG(release, debug|release) {
        TARGET = simple_options_file_parser_x64_mtd
      }
      CONFIG(debug, debug|release) {
        TARGET = simple_options_file_parser_x64_dmtd
      }
    }
}

unix:!macx {

    CONFIG(release, debug|release) {
      TARGET = simple_options_file_parser
    }
    CONFIG(debug, debug|release) {
      TARGET = simple_options_file_parserd
    }
}

TEMPLATE = lib
CONFIG += staticlib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Flags for the compiler.
win32 {
  CONFIG += c++11
  QMAKE_CXXFLAGS_WARN_ON += /wd"4290" /wd"4996" /wd"4251"
  QMAKE_CFLAGS_WARN_ON += /wd"4290" /wd"4996" /wd"4251"
}
unix:!macx: QMAKE_CXXFLAGS += "-std=c++11"

#
# Configure the destination directories here.
#
# Destinations are dependent on the configuration and platform
# being used.
#

ROOT_DIRECTORY = $$PWD

# Build mode.

CONFIG(release, debug|release): CURRENT_CONFIG = Release
else: CONFIG(debug, debug|release): CURRENT_CONFIG = Debug

# For windows only: find out if we're compiling for 32 or 64 bit (platform).

win32{
    contains(QT_ARCH, i386) {
      PLATFORM = Win32
    } else {
      PLATFORM = x64
    }
}

# Set the build and other intermediate directories.

win32: BUILD_DIRECTORY = $${ROOT_DIRECTORY}/$${PLATFORM}/$${CURRENT_CONFIG}
unix:!macx: BUILD_DIRECTORY = $${ROOT_DIRECTORY}/$${CURRENT_CONFIG}

OBJECTS_DIR = $${BUILD_DIRECTORY}
MOC_DIR     = $${BUILD_DIRECTORY}
RCC_DIR     = $${BUILD_DIRECTORY}
UI_DIR      = $${BUILD_DIRECTORY}
DESTDIR     = $${BUILD_DIRECTORY}

win32 {

  #
  # POST BUILD STEPS FOR WINDOWS.
  #

  QMAKE_POST_LINK += $${PWD}\\postbuild.bat $${PWD} $${PLATFORM} $${CURRENT_CONFIG} $${TARGET}
}

unix:!macx {
  #
  # POST BUILD STEP FOR LINUX.
  #
  # Here we only use the configuration to parametrize the file
  # postbuild.sh used to execute the post build step.
  #

  QMAKE_POST_LINK += $${PWD}/postbuild.sh $${CURRENT_CONFIG} $${PWD}
}

# Sources & headers

SOURCES += \
    src/simple_options_file_parser.cpp 

HEADERS += \
    src/simple_options_file_parser.hpp

