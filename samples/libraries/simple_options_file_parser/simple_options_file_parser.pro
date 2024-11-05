QT       += core gui widgets
TEMPLATE  = lib
CONFIG   += staticlib

# Warn when using deprecated APIs.

DEFINES += QT_DEPRECATED_WARNINGS

# Remove warning normally issued by the MSVC compiler. Set also the
# runtime used by the compiler to match that of our ImageMagick
# library).

win32 {
  QMAKE_CXXFLAGS_WARN_ON += /wd"4290" /wd"4996" /wd"4251"
  QMAKE_CFLAGS_WARN_ON += /wd"4290" /wd"4996" /wd"4251"
}

#
# Name of the output target. It depends on the OS,
# (Windows, Linux) the platform (x86, x64) and the
# build mode (release, debug).
#

win32 {

    contains(QT_ARCH, i386) {
      CONFIG(release, debug|release) {
        TARGET = simple_options_file_parser_mtd
        BUILD_FOLDER = $${OUT_PWD}/release
      }
      CONFIG(debug, debug|release) {
        TARGET = simple_options_file_parser_dmtd
        BUILD_FOLDER = $${OUT_PWD}/debug
      }
    } else {
      CONFIG(release, debug|release) {
        TARGET = simple_options_file_parser_x64_mtd
        BUILD_FOLDER = $${OUT_PWD}/release
      }
      CONFIG(debug, debug|release) {
        TARGET = simple_options_file_parser_x64_dmtd
        BUILD_FOLDER = $${OUT_PWD}/debug
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

# Post build step.

win32 {

  #
  # For windows.
  #  - First parameter: build folder.
  #  - Second parameter: build mode (debug, release).
  #  - Third parameter: project folder.
  #

  QMAKE_POST_LINK += $${PWD}\\postbuild.bat $${PWD} $${BUILD_FOLDER} $${TARGET}

}
unix:!macx {

  #
  # For Linux.
  #
  #  - First parameter:  project folder.
  #  - Second parameter: build folder.
  #  - Third parameter:  target (executable) name.
  #

  QMAKE_POST_LINK += $${PWD}/postbuild.sh $${PWD} $${OUT_PWD} $${TARGET}
}

# Sources & headers

SOURCES += \
    src/simple_options_file_parser.cpp 

HEADERS += \
    src/simple_options_file_parser.hpp
