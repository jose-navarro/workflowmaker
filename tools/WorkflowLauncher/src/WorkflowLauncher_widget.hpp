/** \file WorkflowLauncher_widget.hpp
\brief Main application's widget (application GUI).
*/

#ifndef WORKFLOWLAUNCHER_WIDGET_HPP
#define WORKFLOWLAUNCHER_WIDGET_HPP

#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QTabWidget>
#include <QVector>
#include <QVBoxLayout>
#include <QWidget>

#include <vector>
#include <map>
#include <string>
#include <fstream>

#include "toolkit_structures.hpp"
#include "toolkit_parser.hpp"

#include "workflow_structures.hpp"
#include "workflow_parser.hpp"

#include "launcher_structures.hpp"
#include "launcher_parser.hpp"
#include "launcher_writer.hpp"

#include "launcher_data_widget.hpp"
#include "export_options_widget.hpp"
#include "WorkflowLauncher_files_widget.hpp"
#include "WorkflowLauncher_parameters_widget.hpp"
#include "WorkflowLauncher_repos_widget.hpp"

#ifdef __GNUC__
  #include <libgen.h>         // dirname
  #include <unistd.h>         // readlink
  #include <linux/limits.h>   // PATH_MAX
#else
  #include <Windows.h>
#endif

#include <QDebug>

using namespace std;

/// \brief Main application's widget (application GUI).
/**
 This class provides a widget to provide the different data
 required to define a launcher and export the corresponding
 option and shell (batch) files.
*/

class WorkflowLauncher_widget : public QWidget
{
    Q_OBJECT

  public:

    /// \brief Constructor.
    /**
      \param parent This widget's parent.
     */

           WorkflowLauncher_widget   (QWidget* parent = nullptr);

    /// Destructor.

           ~WorkflowLauncher_widget  (void);

  protected slots:

    /// \brief Export a launcher file as a set of option and shell files.

    void   on_export_launcher_file   (void);

    /// \brief Loads an existing launcher file.

    void   on_load_launcher_file     (void);

    /// \brief Finishes the process of creating a new launcher file
    ///        started by on_new_launcher_file().

    void   new_launcher_file         (void);

    /// \brief Create a new launcher file.
    /**
       Callback of QPushButton control_button_load_.
    */

    void   on_new_launcher_file      (void);

    /// \brief Save a launcher file.
    /**
       Callback of QPushButton control_button_save_.
    */

    void   on_save_launcher_file     (void);

  protected:

    /// \brief Create the set of files (connections) for all those
    ///        slots that the user did not connected in the workflow.
    /**
      \param[out] dfiles The list of files whose slots the user
               did not connect in the workflow.
     */

    void   add_disconnected_files    (vector<WLFile>& dfiles);

    /// \brief Build a list of the whole set of files
    ///        intervening in the workflow.
    /**
      This method builds the whole set of files, that is those
      included in any kind of connection. This includes
      files involved in either task-to-task, task-to-repository
      or repository-to-task connections.

      \see build_ttt.filelist();
     */

    void   build_full_filelist       (void);

    /// \brief Build the map including the set of files required by
    ///        each task in the underlying workflow.
    /**
      \param[in] files The set of existing files.
      \param[out] fsets The map task->file set.
     */

    void   build_tasks_file_sets     (vector<WLFile>&               files,
                                      map<WLTaskKey, set<WLFile*>>& fsets);

    /// \brief Build the map including the set of parameters required by
    ///        each task in the underlying workflow.
    /**
      \param[out] psets The map task->file set.
     */

    void   build_tasks_param_sets    (map<WLTaskKey, set<WLParameter*>>& psets);

    /// \brief Build a map with the files that start at a
    ///        task and end at a repository.
    /**
      \param[out] ttr_map The map.
     */

    void   build_ttr_filemap        (map<WLFile_id, WLFile*>& ttr_map);

    /// \brief Build a list of the files involved in
    ///        task-to-task connections only.
    /**
      This method does not build the list of files intervening
      in task-to-repository or repository-to-task connections,
      only those that are involved in task-to-task (ttt) ones.

      \see build_full_filelist()
     */

    void   build_ttt_filelist        (void);

    /// \brief Build a map with the files that start at end
    ///        at a task.
    /**
      \param[out] ttt_map The map.
     */

    void   build_ttt_filemap        (map<WLFile_id, WLFile*>& ttt_map);

    /// \brief Build the list of parameters needed by all the
    ///        tasks involved in the underlying workflow.
    /**
      \param[out] wlp_id_to_wlp The list of parameters, once built.

      The output parameter wlp_id_to_wlp is a map, whose key is
      the concatenation of (task sequential id, task id, parameter id).
      The item pointed by this key is the full description of the
      parameter.

      Such a map allows for an automatic sort of the parameters
      to deal with.
     */

    void   build_parameter_list      (map<string, WLParameter>& wlp_id_to_wlp);

    /// \brief Check that all the required parameters have
    /// been input in the widget's interface.
    /**
      \return True if all parameters have been set, false
              otherwise.
     */

    bool   check_completeness        (void);

    /// \brief Complete the launcher data using the information
    ///        stored in the underlying workflow and toolkit.

    void   complete_launcher_data    (void);

    /// \brief Determine what are the files that must be deleted
    ///        after the execution of each task.
    /**
      \param[out] files_to_delete The names of the files to delete
               after every task in the workflow is executed.

       The array of strings in position n indicate the set of
       files that must be deleted once that the task in position
       n is executed.
     */

    void   find_deletion_points      (vector<vector<string>>& files_to_delete);

    /// \brief Retrieve the path or the executable.
    /**
      \return The path were the executable resides.
     */

    string get_executable_path       (void);

    /// \brief Load an existing launcher.
    /**
      \param[in] lch_path Path to the xml file containing the definition
                 of the launcher.
      \param[out] lch The launcher, once loaded.
      \return True if the launcher is correctly loaded, false otherwise.

      Note that this method uses message boxes to tell the user about
      the possible errors detected when loading the launcher.
     */

    bool   load_launcher             (QString&    lch_path,
                                      WLLauncher& lch);

    /// \brief Load an existing toolkit.
    /**
      \param[in] tk_path Path to the xml file containing the definition
                 of the toolkit.
      \param[out] tk The toolkit, once loaded.
      \return True if the toolkit is correctly loaded, false otherwise.

      Note that this method uses message boxes to tell the user about
      the possible errors detected when loading the toolkit.
     */

    bool   load_toolkit              (QString& tk_path, toolkit& tk);

    /// \brief Load an existing workflow.
    /**
      \param[in] wf_path Path to the xml file containing the definition
                 of the workflow.
      \param[out] wf The workflow, once loaded.
      \return True if the workflow is correctly loaded, false otherwise.

      Note that this method uses message boxes to tell the user about
      the possible errors detected when loading the workflow.
     */

    bool   load_workflow             (QString& wf_path, WFWorkflow& wf);

    /// \brief Prepend the corresponding paths (repository, work
    ///        directory...) to the several files involved in
    ///        the launcher.

    void   prepend_folder_paths      (void);

    /// \brief Replace all the occurrences of some string by
    ///        another string.
    /**
      \param the_string The string where changes will take place.
      \param find_this The string to find and then replace.
      \param replace_with_this The replacement of the string sought.
     */

    void   replace_all_occurrences   (string& the_string,
                                      string& find_this,
                                      string& replace_with_this);

    /// \brief Rename those task-to-task files that are also saved
    ///        to a repository, so its name must be taken from
    ///        the user (task to repository connection).
    /**
      \param[in] ttr_map The map with the task-to-repository connections
                 to check.
      \param[in] ttt_map The map with the task-to-task connections to
                 check.
     */

    void   rename_ttt_repeated_files (map<WLFile_id, WLFile*>& ttr_map,
                                      map<WLFile_id, WLFile*>& ttt_map);

    /// \brief Search for an existing toolkit to load it.
    /**
      \param[in] banner Text to show as the header (banner) of the open file
                 dialog.
      \param[out] tk The toolkit, once loaded.
      \return True if the toolkit is loaded, false otherwise.
     */

    bool   select_and_open_toolkit   (QString& banner, toolkit& tk);

    /// \brief Search for an existing workflow and load it.
    /**
      \param[in]  banner The banner to show in the open file dialog.
      \param[out] wf The workflow, once loaded.
      \return True if the workflow is loaded, false otherwise.
     */

    bool   select_and_open_workflow  (QString&    banner,
                                      WFWorkflow& wf);

    /// \brief Reset the error messages for all tabs.

    void   reset_error_messages      (void);

    /// \brief Read the file containing the version string
    ///        and return it as a string.
    /**
      \return The version string for the WorklflowMaker package.
     */

    string version_string             (void);

    /// \brief Write the options files for all the tasks involved in
    ///        the launcher.
    /**
      \param[in] fsets The map including, for each task, the set of files whose
                 labels and values (file names) must be written.
      \param[out] psets The map including, for each task, the set of parameters
                 whose labels and values must be written.
      \param[out] errors List of errors detected during the writing process. Empty
                 when no errors are detected.
      \return True if all files are written successfully, false otherwise.
     */

    bool   write_option_files        (map<WLTaskKey, set<WLFile*>>&   fsets,
                                      map<WLTaskKey, set<WLParameter*>>& psets,
                                      vector<string>                     errors);

    /// \brief Write the shell file, that is, the batch file that will actually
    ///        run the workflow with the values provided by the launcher.
    /**
      \param[in] files_to_delete The lists of files that must be deleted after the execution
               of every task.
      \return True if the shell file is written successfully, false otherwise.
     */

    bool   write_shell_file          (vector<vector<string>>& files_to_delete);


  protected:

    /// \brief Push button: quit (cancel) the application.

    QPushButton*                        control_button_cancel_;

    /// \brief Push button: export a launcher file as shell and
    ///        options files.

    QPushButton*                        control_button_export_;

    /// \brief Push button: load a launcher file.

    QPushButton*                        control_button_load_;

    /// \brief Push button: generate the shell (batch) and option files.

    QPushButton*                        control_button_generate_;

    /// \brief Push button: create a launcher file.

    QPushButton*                        control_button_new_;

    /// \brief Push button: save launcher file.

    QPushButton*                        control_button_save_;

    /// \brief List of errors detected when checking the
    ///        completeness or validity of the several tabs
    ///        making the widget.

    QVector<QString>                    error_list_;

    /// \brief List of files intervening in the workflow that
    ///        are either read from or written to some
    ///        repository - For these files, the user must
    ///        input a name.

    vector<WLFile>                      file_list_repos_;

    /// \brief List of files intervening in the workflow that
    ///        are created and read by tasks exclusively, that
    ///        is, these are never stored in repositories.

    vector<WLFile>                      file_list_tasks_;

    /// \brief List of files that have never been connected,
    ///        that is, corresponding to output slots in the
    ///        workflow that have never been assigned by
    ///        the user.

    vector<WLFile>                      file_list_unconnected_;

    /// \brief The launcher.

    WLLauncher                          launcher_;

    /// \brief Identifier of the launcher object currently open.

    QString                             launcher_id_;

    /// \brief The dialog used to input the lineage data for
    ///        a new launcher file.

    launcher_data_widget                launcher_data_;

    /// \brief Description of the launcher object currently open.

    QString                             launcher_description_;

    /// \brief Flag stating whether a launcher document is already
    ///        opened.

    bool                                open_document_;

    /// \brief String identifying the operating system for which
    ///        shell files must be exported.

    string                              os_for_export_;

    /// \brief Widget containing the files, options and advanced
    ///        options tabs in our interface.

    QTabWidget*                         tabs_;

    /// \brief The widget with the files.

    WorkflowLauncher_files_widget*      tab_files_;

    /// \brief The widget with the parameters.

    WorkflowLauncher_parameters_widget* tab_parameters_;

    /// \brief The widget with the repositories.

    WorkflowLauncher_repos_widget*      tab_repositories_;

    /// \brief The toolkit.

    toolkit                             toolkit_;

    /// \brief The path to the working directory assigned to
    ///        the launcher.

    string                              work_dir_path_;

    /// \brief The workflow.

    WFWorkflow                          workflow_;
};

#endif // WORKFLOWLAUNCHER_WIDGET_HPP
