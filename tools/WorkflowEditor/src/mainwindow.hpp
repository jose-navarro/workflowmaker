/** \file mainwindow.hpp
  \brief Interface of the application, including menus, toolboxes and the view.
*/


#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#ifdef __GNUC__
  #include <libgen.h>         // dirname
  #include <unistd.h>         // readlink
  #include <linux/limits.h>   // PATH_MAX
#else
  #include <Windows.h>
#endif

#include <filesystem>

#include "AboutBox.hpp"
#include "toolkit_structures.hpp"
#include "toolkit_parser.hpp"
#include "diagramitem.hpp"
#include "palette_utils.hpp"
#include "task_info_panel.hpp"
#include "workflow_parser.hpp"
#include "workflow_writer.hpp"
#include "workflow_structures.hpp"

#include <algorithm>
#include <string>
#include <fstream>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileInfo>
#include <QMainWindow>
#include <QMimeData>

#include "workflow_data_widget.hpp"

class DiagramScene;

QT_BEGIN_NAMESPACE
class QAction;
class QToolBox;
class QComboBox;
class QButtonGroup;
class QLineEdit;
class QToolButton;
class QAbstractButton;
class QGraphicsView;
QT_END_NAMESPACE

using namespace std;

namespace fs = std::filesystem;

/// \brief Interface of the application, including menus, toolboxes and the view.

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:

    /// \brief Constructor.

             MainWindow            (void);

  private slots:

    /// \brief Show the about dialog.

    void     about                 (void);

    /// \brief Bring the currently selected item to the front.

    void     bringToFront          (void);

    /// \brief Take care of a click on any of the insertion buttons
    /**
      \param[in] button Pointer to the button clicked.

      Clicking on any of the insertion (task, repository) will
      select the item they represents and then this kind of
      item will be selected if the user clicks on the view.
     */

    void     buttonGroupClicked    (QAbstractButton* button);

    /// \brief Delete the currently selected item.

    void     deleteItem            (void);

    /// \brief Update the interface when a new item has been inserted.
    /**
      \param[in] item The item that has just been inserted.
     */

    void     itemInserted          (DiagramItem* item);

    /// \brief Start the creation of a new workflow.

    void     newWorkflow           (void);

    /// \brief Complete the creation of a new workflow.

    void     new_wf_data_ready     (void);

    /// \brief Open an existing workflow.

    void     openWorkflow          (void);

    /// \brief Update the interface when a button in the pointer toolbar
    ///        has been clicked.

    void     pointerGroupClicked   (void);

    /// \brief Save the current workflow.

    void     saveWorkflow          (void);

    /// \brief Update the interface when the zoom level is changed.
    /**
      \param[in] scale The new scale factor.
     */

    void     sceneScaleChanged     (const QString& scale);

    /// \brief Send back the currently selected item.

    void     sendToBack            (void);

    /// \brief Keep track of the task selected in the task panel.
    /**
      \param task_number The number of the task just selected.
     */

    void     task_selected         (int task_number);

  private:

    /// \brief Build a warning message stating the status of the workflow,
    ///        including the number of tasks and repositories in the several
    ///        possible states.
    /**
      \param[in] missing_inputs Total number of tasks for which at least one
                 input has not been assigned.
      \param[in] missing_some_outputs Total number of tasks for which at least one
                 output has not been assigned.
      \param[in] missing_all_outputs Total number of tasks for which none of their
                 outputs have been assigned.
      \param[in] disconnected_repos Total number of repositories that have neither
                 incoming nor outcoming connections - that is, that are involved
                 in zero connections, so they are disconnected.
      \param[out] status_msg The message with the information about the current status of
                  the workflow.
      \return True if the workflow is complete (that is, all required connections exist),
              false otherwise.
     */

    bool     build_wf_status_msg   (int      missing_inputs,
                                    int      missing_some_outputs,
                                    int      missing_all_outputs,
                                    int      disconnected_repos,
                                    QString& status_msg);

    /// \brief Instantiate the set of actions used by the application.

    void     createActions         (void);

    /// \brief Create a button for the toolbox.
    /**
      \param[in] text Text to show.
      \param[in] type Type of button (task, repository) to create.
      \return The button widget just created.
     */

    QWidget* createCellWidget      (const QString&                 text,
                                          DiagramItem::DiagramType type);

    /// \brief Instantiate the set of menus used by the application.

    void     createMenus           (void);

    /// \brief Instantitate the set of toolbars used by the application.

    void     createToolbars        (void);

    /// \brief Create the toolbox used by the application.

    void     createToolBox         (void);

    /// \brief Re-implementation of the inherited method to accept
    ///        dragging xml workflow files over the application.
    /**
      \param event The object including all the information about
             the drag event.

      This method is re-implemented to add drag capabilities to
      the application. It will check that the event corresponds
      to a file drag and that it contains the appropriate extension.
     */

    void   dragEnterEvent          (QDragEnterEvent *event) override;

    /// \brief Re-implementation of the inherited method handling
    ///        the drop of an xml workflow file over the application.
    /**
      \param event The object including all the information about
             the file dropped.

      This method is re-implemented to add drop capabilities to
      the application. It will check that the object dropped is
      a file; if so, it will parse the xml file and display its
      information on the application screen.
     */

    void   dropEvent               (QDropEvent *event) override;

    /// \brief Retrieve the path where the read-only data files are stored.
    /**
      \return The path (with no slash at the end) where the several read-only
              files loaded by the applications in the WorkflowMaker package
              reside, or the empty string if such path cannot be determined.

      On Windows it is assumed that the data folder is named "data" and that
      it is a sibling of the folder where executables reside.

      This function will fail in Linux systems where the /proc directory
      does not exist. Either for this reason or because the said "data"
      folder does not exist, this function may return the empty string.
     */

    string get_data_path           (void);

    /// \brief Retrieve the path of the current executable.
    /**
      \return The path (with no slash at the end) where the executable
              calling this function resides or the empty string if
              such path cannot be determined.

      This function will fail in Linux systems where the /proc directory
      does not exist. This is the reason why an empty string may be
      returned.
     */

    string get_executable_path     (void);

    /// \brief Load an existing toolkit.
    /**
      \param[in] tk_path Path to the xml file containing the definition
                 of the toolkit.
      \param[out] tk The toolkit, once loaded.
      \return True if the toolkit is correctly loaded, false otherwise.

      Note that this method uses message boxes to tell the user about
      the possible errors detected when loading the toolkit.
     */

    bool     load_toolkit          (QString& tk_path, toolkit& tk);

    /// \brief Load an existing workflow.
    /**
      \param[in] wf_path Path to the xml file containing the definition
                 of the workflow.
      \param[out] wf The workflow, once loaded.
      \return True if the workflow is correctly loaded, false otherwise.

      Note that this method uses message boxes to tell the user about
      the possible errors detected when loading the workflow.
     */

    bool     load_workflow         (QString& wf_path, WFWorkflow& wf);

    /// \brief Load a workflow as well as the toolkit on which it
    ///        relies.
    /**
      \param[in] wf_path Path to the xml file containing the definition
                 of the workflow.

      The method first tries to load the workflow file using load_workflow();
      then, asks for the toolkit on which the launcher relies. If everything
      is correct, then this method displays the workflow on the screen.

      Note that this method uses message boxes to tell the user about
      the possible errors detected when loading the launcher.
     */

    void   load_workflow_chain       (QString& wf_path);

    /// \brief Search for an existing toolkit to load it.
    /**
      \param[in] banner Text to show as the header (banner) of the open file
                 dialog.
      \param[out] tk The toolkit, once loaded.
      \return True if the toolkit is loaded, false otherwise.
     */

    bool     selectAndOpenToolkit  (QString& banner, toolkit& tk);

    /// \brief Search for an existing workflow, and retrieve its path.
    /**
      \param[out] wf_path The path to the selected workflow.
      \return True if the workflow is loaded, false otherwise.
     */

    bool     selectWorkflow        (QString& wf_path);

    /// \brief Read the file containing the version string
    ///        and return it as a string.
    /**
      \return The version string for the WorklflowMaker package.
     */

    string   version_string        (void);

  private:

    /// \brief The action to launch the about box.

    QAction*             aboutAction;

    /// \brief The about box.

    AboutBox*            about_box_;

    /// \brief The about menu.

    QMenu*               aboutMenu;

    /// \brief The group of buttons including those to insert tasks
    ///        or repositories.

    QButtonGroup*        buttonGroup;

    /// \brief Action to delete an item.

    QAction*             deleteAction;

    /// \brief The toolbar with the buttons to edit items (delete,
    ///        send to back / front).

    QToolBar*            editToolBar;

    /// \brief The action to quit the applicatin.

    QAction*             exitAction;

    /// \brief The file menu.

    QMenu*               fileMenu;

    /// \brief The item menu (equivalent to the editToolBar).

    QMenu*               itemMenu;

    /// \brief Action to create a new workflow.

    QAction*             newWorkflowAction;

    /// \brief Flag stating whether a workflow is already open.

    bool                 open_document_;

    /// \brief Action to open an existing workflow.

    QAction*             openWorkflowAction;

    /// \brief The toolbar showing the different pointer modes.

    QToolBar*            pointerToolbar;

    /// \brief The group of buttons to select the pointer mode.

    QButtonGroup*        pointerTypeGroup;

    /// \brief Action to save the current workflow.

    QAction*             saveWorkflowAction;

    /// \brief Actino to send an item back.

    QAction*             sendBackAction;

    /// \brief The scene where all the items will be drawn.

    DiagramScene*        scene;

    /// \brief Combo to select different zoom levels.

    QComboBox*           sceneScaleCombo;

    /// \brief The panel where the information about the avaialbe tasks
    ///        and their input and output files will be shown.

    task_info_panel*     task_panel_;

    /// \brief Action to bring an item to the front.

    QAction*             toFrontAction;

    /// \brief The toolbox to include the buttons to insert repositories
    ///        and tasks.

    QToolBox*            toolBox;

    /// \brief The view handling our scene.

    QGraphicsView*       view;

    /// \brief Dialog to collect the lineage information when creating
    ///        a new workflow.

    workflow_data_widget wf_data_widget;

    /// \brief Description of the current workflow.

    string               wf_description_;

    /// \brief Identifier of the current workflow.

    string               wf_id_;

};

#endif // MAINWINDOW_HPP
