QT       += core gui widgets
TARGET    = workflowlauncher
TEMPLATE  = app

# Warn when using deprecated APIs.

DEFINES += QT_DEPRECATED_WARNINGS

# Remove warning normally issued by the MSVC compiler

win32 {
  QMAKE_CXXFLAGS_WARN_ON += /wd"4290" /wd"4996" /wd"4251"
  QMAKE_CFLAGS_WARN_ON += /wd"4290" /wd"4996" /wd"4251"
}

# Build mode.

CONFIG(release, debug|release):     CURRENT_CONFIG = release
else: CONFIG(debug, debug|release): CURRENT_CONFIG = debug

# Post build step ONLY FOR RELEASE platforms (not for DEBUG ones.)

CONFIG(release, debug|release) {
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
    #
    #  - First parameter:  project folder.
    #  - Second parameter: build folder.
    #  - Third parameter:  target (executable) name.
    #

    QMAKE_POST_LINK += $${PWD}/postbuild.sh $${PWD} $${OUT_PWD} $${TARGET}
  }
}

# Source code & headers.

SOURCES += \
    src/WorkflowLauncher_files_widget.cpp \
    src/WorkflowLauncher_repos_widget.cpp \
    src/export_options_widget.cpp \
    src/launcher_data_widget.cpp \
    src/main.cpp \
    src/WorkflowLauncher_parameters_widget.cpp \
    src/WorkflowLauncher_widget.cpp \
    src/single_file_widget.cpp \
    src/single_parameter_widget.cpp \
    src/single_repository_widget.cpp

HEADERS += \
    src/WorkflowLauncher_files_widget.hpp \
    src/WorkflowLauncher_parameters_widget.hpp \
    src/WorkflowLauncher_repos_widget.hpp \
    src/WorkflowLauncher_widget.hpp \
    src/export_options_widget.hpp \
    src/launcher_data_widget.hpp \
    src/single_file_widget.hpp \
    src/single_parameter_widget.hpp \
    src/single_repository_widget.hpp

# Libraries, either opensource or self-made. Only release version for Linux.

INCLUDEPATH += ../../libraries/WorkflowMakerLib/headers \
               ../../libraries/rapidxml
			   
win32 {
  CONFIG(release, debug|release) {
    LIBS += -L$$PWD/../../libraries/WorkflowMakerLib/binaries/ -lWorkflowMakerLib_x64_mtd
  }
  CONFIG(debug, debug|release) {
    LIBS += -L$$PWD/../../libraries/WorkflowMakerLib/binaries/ -lWorkflowMakerLib_x64_dmtd
    }
}

unix:!macx {
  CONFIG(release, debug|release) {
    LIBS += -L$$PWD/../../libraries/WorkflowMakerLib/binaries/ -lWorkflowMakerLib
  }
}


# ICONS and other resources.

RESOURCES         += WorkflowLauncher_resources.qrc
win32:RC_FILE     += resources/WorkflowLauncher_windows_resources.rc
win32:OTHER_FILES += resources/WorkflowLauncher_windows_resources.rc
