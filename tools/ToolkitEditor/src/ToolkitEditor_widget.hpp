/** \file ToolkitEditor_widget.hpp
\brief Main application's widget (application GUI).
*/

#ifndef TOOLKITEDITOR_WIDGET_HPP
#define TOOLKITEDITOR_WIDGET_HPP

#ifdef __GNUC__
  #include <libgen.h>         // dirname
  #include <unistd.h>         // readlink
  #include <linux/limits.h>   // PATH_MAX
#else
  #include <Windows.h>
#endif

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QMimeData>
#include <QPushButton>
#include <QString>
#include <QTabWidget>
#include <QVector>
#include <QVBoxLayout>
#include <QWidget>

#include <vector>
#include <string>

#include "toolkit_structures.hpp"
#include "toolkit_parser.hpp"
#include "toolkit_writer.hpp"

#include "ToolkitEditor_description_widget.hpp"
#include "ToolkitEditor_parameters_widget.hpp"
#include "ToolkitEditor_filetypes_widget.hpp"
#include "ToolkitEditor_tasks_widget.hpp"

using namespace std;

/// \brief Main application's widget (application GUI).
/**
 This class provides a widget to provide the different data
 required to define a toolkit.
*/

class ToolkitEditor_widget : public QWidget
{
    Q_OBJECT

  public:

    /// \brief Constructor.
    /**
      \param parent This widget's parent.
     */

           ToolkitEditor_widget  (QWidget* parent = nullptr);

    /// Destructor.

           ~ToolkitEditor_widget (void);

  protected slots:

    /// \brief React when the current tab is changed.
    /**
      \param new_tab The number of the new current tab.
     */

    void   on_change_tab         (int new_tab);

    /// \brief Load a toolkit file.
    /**
       Callback of QPushButton control_button_load_.
    */

    void   on_load_toolkit_file  (void);

    /// \brief Saves the toolkit to an external file.
    /**
       Callback of QPushButton control_button_save_.
    */

    void   on_save_toolkit_file  (void);

  protected:

    /// \brief Check that all the required parameters have
    /// been input in the widget's interface.
    /**
      \return True if all parameters have been set, false
              otherwise.
     */

    bool   check_completeness    (void);

    /// \brief Copy the values of the toolkit from the fields
    /// on the screen into a toolkit object.
    /**
      Note that no checks on the completeness or correction of
      the values on the screen is made. Please, call first
      check_completeness() and validate_semantics() to be sure
      that no errors are present in the data.
    */

    void   copy_toolkit_values   (void);

    /// \brief Re-implementation of the inherited method to accept
    ///        dragging toolkit xml files over the application.
    /**
      \param event The object including all the information about
             the drag event.

      This method is re-implemented to add drag capabilities to
      the application. It will check that the event corresponds
      to a file drag and that it contains the appropriate extension.
     */

    void   dragEnterEvent        (QDragEnterEvent *event) override;

    /// \brief Re-implementation of the inherited method handling
    ///        the drop of an xml toolkit file over the application.
    /**
      \param event The object including all the information about
             the file dropped.

      This method is re-implemented to add drop capabilities to
      the application. It will check that the object dropped is
      a file; if so, it will parse the xml file and display its
      information on the application screen.
     */

    void   dropEvent             (QDropEvent *event) override;

    /// \brief Retrieve the path or the executable.
    /**
      \return The path were the executable resides.
     */

    string get_executable_path   (void);

    /// \brief Validate, parse and load a toolkit file.
    /**
      \param path_to_toolkit_file Path to the file with
             the toolkit file to validate, parse and load.

      This method tries first to validate the grammar of
      the input toolkit file using the corresponding XML
      schema; assuming the validation succeeds, then
      it parses the file to retrieve the information
      about the toolkit it contains; if this step is
      also correct, it will load said data and display
      it on the screen.

      Should any error happen, a message box will be shown.

     */

    void   parse_and_load_tk     (QString& path_to_toolkit_file);

    /// \brief Reset the error messages for all tabs.

    void   reset_error_messages  (void);

    /// \brief Check that the values input by the user in the
    ///        interface of the widget are correct, warning the user
    ///        if not (via message boxes).
    /**
      \return True if the values are correct, false otherwise.
    */

    bool   validate_semantics    (void);

    /// \brief Read the file containing the version string
    ///        and return it as a string.
    /**
      \return The version string for the WorklflowMaker package.
     */

    string version_string        (void);

  protected:


    /// \brief Push button: load options file.

    QPushButton*                     control_button_load_;

    /// \brief Push button: quit (cancel) the application.

    QPushButton*                     control_button_cancel_;

    /// \brief Push button: save options file.

    QPushButton*                     control_button_save_;

    /// \brief List of errors detected when checking the
    ///        completeness or validity of the several tabs
    ///        making the widget.

    QVector<QString>                 error_list_;

    /// \brief Widget containing the files, options and advanced
    ///        options tabs in our interface.

    QTabWidget*                      tabs_;

    // \brief The widget with the general information.

    ToolkitEditor_description_widget* tab_description_;

    /// \brief The widget with the file types.

    ToolkitEditor_filetypes_widget*  tab_filetypes_;

    /// \brief The widget with the parameters.

    ToolkitEditor_parameters_widget* tab_parameters_;

    /// \brief The widget with the tasks.

    ToolkitEditor_tasks_widget*      tab_tasks_;

    /// \brief The toolkit to load / edit / save.

    toolkit                          toolkit_;
};

#endif // TOOLKITEDITOR_WIDGET_HPP
