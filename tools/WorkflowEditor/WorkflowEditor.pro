QT       += core gui widgets
TARGET    = workfloweditor
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

RESOURCES         += WorkflowEditor.qrc
win32:RC_FILE     += resources/WFMEditor_windows_resources.rc
win32:OTHER_FILES += resources/WFMEditor_windows_resources.rc
