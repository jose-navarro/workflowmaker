QT += core widgets gui xml xmlpatterns

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = workfloweditor
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

HEADERS	    =   \
		src/AboutBox.hpp \
		src/WFMEditor_structures.hpp \
		src/connection.hpp \
		src/diagramitem.hpp \
		src/diagramscene.hpp \
		src/file_list_widget.hpp \
		src/file_table_model.hpp \
		src/file_table_view.hpp \
		src/graph.hpp \
		src/mainwindow.hpp \
		src/task_info_panel.hpp \
		src/task_list_widget.hpp \
		src/task_table_model.hpp \
		src/task_table_view.hpp \
		src/workflow_data_widget.hpp
SOURCES	    =   \
        src/mainwindow.cpp \
		src/AboutBox.cpp \
		src/connection.cpp \
		src/diagramitem.cpp \
		src/file_list_widget.cpp \
		src/file_table_model.cpp \
		src/file_table_view.cpp \
		src/graph.cpp \
		src/main.cpp \
		src/diagramscene.cpp \
		src/task_info_panel.cpp \
		src/task_list_widget.cpp \
		src/task_table_model.cpp \
		src/task_table_view.cpp \
		src/workflow_data_widget.cpp

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

RESOURCES          = WorkflowEditor.qrc
win32:RC_FILE     += resources/WFMEditor_windows_resources.rc
win32:OTHER_FILES += resources/WFMEditor_windows_resources.rc
