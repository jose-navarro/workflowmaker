QT       += core gui widgets
TEMPLATE  = lib
CONFIG   += staticlib

# Warn when using deprecated APIs.

DEFINES += QT_DEPRECATED_WARNINGS

# Remove warning normally issued by the MSVC compiler

win32 {
  QMAKE_CXXFLAGS_WARN_ON += /wd"4290" /wd"4996" /wd"4251"
  QMAKE_CFLAGS_WARN_ON += /wd"4290" /wd"4996" /wd"4251"
}

# Build mode.

CONFIG(release, debug|release): CURRENT_CONFIG = Release
else: CONFIG(debug, debug|release): CURRENT_CONFIG = Debug

#
# Name of the output target. It depends on the OS,
# (Windows, Linux) and the build mode (release, debug).
#

win32 {

  CONFIG(release, debug|release) {
    TARGET = WorkflowMakerLib_x64_mtd
  }
  CONFIG(debug, debug|release) {
    TARGET = WorkflowMakerLib_x64_dmtd
  }
}

unix:!macx {

  CONFIG(release, debug|release) {
    TARGET = WorkflowMakerLib
  }
  CONFIG(debug, debug|release) {
    TARGET = WorkflowMakerLibd
  }
}

# Post-build steps.

win32 {

  #
  # For windows.
  #  - First parameter: build folder.
  #  - Second parameter: build mode (debug, release).
  #  - Third parameter: project folder.
  #

  QMAKE_POST_LINK += $${PWD}\\postbuild.bat $${OUT_PWD} $${CURRENT_CONFIG} $${PWD}
}

unix:!macx {

  #
  # For Linux.
  #  - First parameter: build folder.
  #  - Second parameter: project folder.
  #

  QMAKE_POST_LINK += $${PWD}/postbuild.sh $${OUT_PWD} $${PWD}
}

# Sources & headers

SOURCES += \
    src/launcher_parser.cpp \
    src/launcher_writer.cpp \
    src/palette_utils.cpp \
    src/toolkit_parser.cpp \
    src/toolkit_writer.cpp \
    src/workflow_parser.cpp \
    src/workflow_writer.cpp

HEADERS += \
    src/launcher_parser.hpp \
    src/launcher_structures.hpp \
    src/launcher_writer.hpp \
    src/palette_utils.hpp \
    src/toolkit_parser.hpp \
    src/toolkit_structures.hpp \
    src/toolkit_writer.hpp \
    src/workflow_parser.hpp \
    src/workflow_structures.hpp \
    src/workflow_writer.hpp

INCLUDEPATH += ../rapidxml
