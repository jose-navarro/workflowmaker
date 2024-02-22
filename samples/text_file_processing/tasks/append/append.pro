QT -= gui

CONFIG += console
CONFIG -= app_bundle

TARGET = append

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Flags for the compiler.

win32: CONFIG += c++11
unix:!macx: QMAKE_CXXFLAGS += "-std=c++11"

#
# Configure the destination directories here.
#
# Destinations are dependent on the configuration and platform
# being used.
#

ROOT_DIRECTORY = $$PWD

CONFIG(release, debug|release): CURRENT_CONFIG = Release
else: CONFIG(debug, debug|release): CURRENT_CONFIG = Debug

# We only compile for 64-bit executables.

PLATFORM = x64

# Build and other directories.

win32: BUILD_DIRECTORY = $${ROOT_DIRECTORY}/$${PLATFORM}/$${CURRENT_CONFIG}
unix:!macx: BUILD_DIRECTORY = $${ROOT_DIRECTORY}/$${CURRENT_CONFIG}

OBJECTS_DIR = $${BUILD_DIRECTORY}
MOC_DIR     = $${BUILD_DIRECTORY}
RCC_DIR     = $${BUILD_DIRECTORY}
UI_DIR      = $${BUILD_DIRECTORY}
DESTDIR     = $${BUILD_DIRECTORY}

# Post build step ONLY FOR RELEASE platforms (not for DEBUG ones.)

CONFIG(release, debug|release) {
  win32 {
    QMAKE_POST_LINK += $${PWD}/postbuild.bat $${PWD} $${PLATFORM} $${CURRENT_CONFIG} $${TARGET}
  }
  unix:!macx {
    QMAKE_POST_LINK += $${PWD}/postbuild.sh $${PWD} $${CURRENT_CONFIG} $${TARGET}
  }
}

# The source code.

SOURCES += \
    src/main.cpp \
  src/append_options_file_reader.cpp

HEADERS += \
  src/append_options_file_reader.hpp

# Avoid some unuseful warnings issued by the MSVC compiler.

win32 {
  QMAKE_CXXFLAGS_WARN_ON += /wd"4290" /wd"4996" /wd"4251"
  QMAKE_CFLAGS_WARN_ON += /wd"4290" /wd"4996" /wd"4251"
}

# Libraries, either opensource or self-made. Only release version for Linux.

INCLUDEPATH += ../../../libraries/simple_options_file_parser/headers
			   
win32 {
  CONFIG(release, debug|release) {
    LIBS += -L$$PWD/../../../libraries/simple_options_file_parser/binaries/ -lsimple_options_file_parser_x64_mtd
  }
  CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/../../../libraries/simple_options_file_parser/binaries/ -lsimple_options_file_parser_x64_dmtd
    }
}

unix:!macx {
  CONFIG(release, debug|release) {
    LIBS += -L$$PWD/../../../libraries/simple_options_file_parser/binaries/ -lsimple_options_file_parser
  }
}
