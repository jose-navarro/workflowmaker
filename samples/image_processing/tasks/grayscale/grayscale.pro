TARGET = grayscale
QT     -= gui
CONFIG += console
CONFIG -= app_bundle

# We only accept RELEASE configuration (debug is not supported).

!contains(CONFIG, release) {
    error("This project can only be built in release mode. Please set the configuration to 'release'.")
}

# Warn when using deprecated APIs.

DEFINES += QT_DEPRECATED_WARNINGS

# Remove warning normally issued by the MSVC compiler. Set also the
# runtime used by the compiler to match that of our ImageMagick
# library).

win32 {
  QMAKE_CXXFLAGS_WARN_ON += /wd"4290" /wd"4996" /wd"4251"
  QMAKE_CFLAGS_WARN_ON += /wd"4290" /wd"4996" /wd"4251"
}

# Post build step.

win32 {

  #
  # For windows.
  #  - First parameter: build folder.
  #  - Second parameter: build mode (debug, release).
  #  - Third parameter: project folder.
  #

  QMAKE_POST_LINK += $${PWD}\\postbuild.bat $${PWD} $${OUT_PWD}/release $${TARGET}

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

# Source & header files

SOURCES += grayscale.cpp \
           grayscale_options_file_reader.cpp

HEADERS += grayscale_options_file_reader.hpp

# Include directories.

INCLUDEPATH += ../../../libraries/simple_options_file_parser/headers

win32 {
  INCLUDEPATH += ../../../libraries/imagemagick/headers
}

unix:!macx {
  INCLUDEPATH += /usr/include/ImageMagick-6 \
               /usr/include/x86_64-linux-gnu/ImageMagick-6
}

#
# Libraries, either opensource or self-made. Only release versions.
# Note how, on Windows platforms, we must include the typical
# advapi32, user32 (and other) libraries explicitly!!!
#

win32 {
  LIBS += -L"$$PWD/../../../libraries/simple_options_file_parser/binaries"
  LIBS += -L"$$PWD/../../../libraries/imagemagick/binaries"

  LIBS += -lsimple_options_file_parser_x64_mtd
  LIBS += -lCORE_RL_aom_ -lCORE_RL_brotli_ -lCORE_RL_bzlib_ -lCORE_RL_cairo_ -lCORE_RL_coders_ -lCORE_RL_croco_ \
          -lCORE_RL_de265_ -lCORE_RL_exr_ -lCORE_RL_ffi_ -lCORE_RL_filters_ -lCORE_RL_flif_ -lCORE_RL_freetype_ \
          -lCORE_RL_fribidi_ -lCORE_RL_glib_ -lCORE_RL_harfbuzz_ -lCORE_RL_heif_ -lCORE_RL_highway_ -lCORE_RL_jasper_ \
          -lCORE_RL_jpeg-turbo_ -lCORE_RL_jpeg-xl_ -lCORE_RL_lcms_ -lCORE_RL_lqr_ -lCORE_RL_lzma_ -lCORE_RL_MagickCore_ \
          -lCORE_RL_MagickWand_ -lCORE_RL_Magick++_ -lCORE_RL_openjpeg_ -lCORE_RL_pango_ -lCORE_RL_pixman_ -lCORE_RL_png_ \
          -lCORE_RL_raqm_ -lCORE_RL_raw_ -lCORE_RL_rsvg_ -lCORE_RL_tiff_ -lCORE_RL_webp_ -lCORE_RL_xml_ \
          -lCORE_RL_zip_ -lCORE_RL_zlib_
  LIBS += -ladvapi32 -luser32 -lgdi32 -lole32 -lshell32
}

unix:!macx {
  LIBS += -lMagick++-6.Q16 -lMagickWand-6.Q16 -lMagickCore-6.Q16
  LIBS += -L$$PWD/../../../libraries/simple_options_file_parser/binaries/ -lsimple_options_file_parser
}
