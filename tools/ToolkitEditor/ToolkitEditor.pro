QT       += core gui widgets
TARGET    = toolkiteditor
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

RESOURCES         += ToolkitEditor_resources.qrc
win32:RC_FILE     += resources/ToolkitEditor_windows_resources.rc
win32:OTHER_FILES += resources/ToolkitEditor_windows_resources.rc

