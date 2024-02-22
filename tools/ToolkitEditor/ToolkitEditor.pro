QT += core gui xml xmlpatterns

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = toolkiteditor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
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

# We only compile 64-bit executables.

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

SOURCES += \
    src/ToolkitEditor_description_widget.cpp \
    src/ToolkitEditor_file_list_widget.cpp \
    src/ToolkitEditor_file_model.cpp \
    src/ToolkitEditor_file_table_view.cpp \
    src/ToolkitEditor_filetype_list_widget.cpp \
    src/ToolkitEditor_filetype_model.cpp \
    src/ToolkitEditor_filetype_selector_widget.cpp \
    src/ToolkitEditor_filetype_table_view.cpp \
    src/ToolkitEditor_filetypes_widget.cpp \
    src/ToolkitEditor_parameter_list_widget.cpp \
    src/ToolkitEditor_parameter_model.cpp \
    src/ToolkitEditor_parameter_selector_widget.cpp \
    src/ToolkitEditor_parameter_table_view.cpp \
    src/ToolkitEditor_single_filetype_widget.cpp \
    src/ToolkitEditor_single_parameter_widget.cpp \
    src/ToolkitEditor_single_task_widget.cpp \
    src/ToolkitEditor_task_list_widget.cpp \
    src/ToolkitEditor_task_model.cpp \
    src/ToolkitEditor_task_table_view.cpp \
    src/ToolkitEditor_tasks_widget.cpp \
    src/main.cpp \
    src/ToolkitEditor_parameters_widget.cpp \
    src/ToolkitEditor_widget.cpp

HEADERS += \
    src/ToolkitEditor_description_widget.hpp \
    src/ToolkitEditor_file_list_widget.hpp \
    src/ToolkitEditor_file_model.hpp \
    src/ToolkitEditor_file_table_view.hpp \
    src/ToolkitEditor_filetype_list_widget.hpp \
    src/ToolkitEditor_filetype_model.hpp \
    src/ToolkitEditor_filetype_selector_widget.hpp \
    src/ToolkitEditor_filetype_table_view.hpp \
    src/ToolkitEditor_filetypes_widget.hpp \
    src/ToolkitEditor_parameter_list_widget.hpp \
    src/ToolkitEditor_parameter_model.hpp \
    src/ToolkitEditor_parameter_selector_widget.hpp \
    src/ToolkitEditor_parameter_table_view.hpp \
    src/ToolkitEditor_parameters_widget.hpp \
    src/ToolkitEditor_single_filetype_widget.hpp \
    src/ToolkitEditor_single_parameter_widget.hpp \
    src/ToolkitEditor_single_task_widget.hpp \
    src/ToolkitEditor_task_list_widget.hpp \
    src/ToolkitEditor_task_model.hpp \
    src/ToolkitEditor_task_table_view.hpp \
    src/ToolkitEditor_tasks_widget.hpp \
    src/ToolkitEditor_widget.hpp

win32 {
  QMAKE_CXXFLAGS_WARN_ON += /wd"4290" /wd"4996" /wd"4251"
  QMAKE_CFLAGS_WARN_ON += /wd"4290" /wd"4996" /wd"4251"
}

# Libraries, either opensource or self-made. Only release version for Linux.

INCLUDEPATH += ../../libraries/WorkflowMakerLib/headers
			   
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

RESOURCES += ToolkitEditor_resources.qrc

win32:RC_FILE += resources/ToolkitEditor_windows_resources.rc

win32:OTHER_FILES += resources/ADAfinder_windows_resources.rc


