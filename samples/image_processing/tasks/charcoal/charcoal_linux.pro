TARGET = charcoal
QT     -= gui
CONFIG += console
CONFIG -= app_bundle

# Decide what is our configuration.

CONFIG(release, debug|release): CURRENT_CONFIG = Release
else: CONFIG(debug, debug|release): CURRENT_CONFIG = Debug

# Set the build destination directory.

DESTDIR = $$PWD/build/$$CURRENT_CONFIG

# Source & header files

SOURCES += charcoal.cpp \
           charcoal_options_file_reader.cpp

HEADERS += charcoal_options_file_reader.hpp


# Include directories.

INCLUDEPATH += ../../../libraries/simple_options_file_parser/headers
INCLUDEPATH += /usr/include/ImageMagick-6 \
               /usr/include/x86_64-linux-gnu/ImageMagick-6

# Libraries

LIBS += -lMagick++-6.Q16 -lMagickWand-6.Q16 -lMagickCore-6.Q16
LIBS += -L$$PWD/../../../libraries/simple_options_file_parser/binaries/ -lsimple_options_file_parser



# Set the build  directory.

# Post build step ONLY FOR RELEASE configuration.

CONFIG(release, debug|release) {
  QMAKE_POST_LINK += $${PWD}/postbuild.sh $${PWD} $${DESTDIR} $${TARGET}
}

