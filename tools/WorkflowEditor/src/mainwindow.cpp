/** \file mainwindow.cpp
  \brief Implementation file for mainwindow.hpp.
*/

#include "connection.hpp"
#include "diagramitem.hpp"
#include "diagramscene.hpp"
#include "mainwindow.hpp"

#include <QtWidgets>
#include <QDebug>

void
MainWindow::
about
(void)
{
  {
    about_box_->show();
  }
}

void
MainWindow::
bringToFront
(void)
{
  {
    if (scene->selectedItems().isEmpty())
      return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    const QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    for (const QGraphicsItem *item : overlapItems)
    {
      if (item->zValue() >= zValue && item->type() == DiagramItem::Type)
        zValue = item->zValue() + 0.1;
    }
    selectedItem->setZValue(zValue);
  }
}

bool
MainWindow::
build_wf_status_msg
(int      missing_inputs,
 int      missing_some_outputs,
 int      missing_all_outputs,
 int      disconnected_repos,
 QString& status_msg)
{
  {
    bool able_to_run;

    //
    // Check if the workflow is complete. If so, no confirmation
    // message is needed.
    //

    if ((missing_inputs == 0)       &&
        (missing_some_outputs == 0) &&
        (missing_all_outputs == 0)  &&
        (disconnected_repos == 0))
      return false;

    // There are problems. Let's check the possible situations...

    able_to_run = true;

    status_msg = "The workflow you are about to save is INCOMPLETE.\n\n";
    if (missing_inputs > 0)
    {
      status_msg  = status_msg + "  - "+ QString::number(missing_inputs) + " tasks are missing one or more inputs.\n";
      able_to_run = false;
    }

    if (missing_some_outputs > 0)
    {
      status_msg = status_msg + "  - "+  QString::number(missing_some_outputs) + " tasks have unassigned outputs.\n";
    }

    if (missing_all_outputs > 0)
    {
      status_msg = status_msg + "  - "+  QString::number(missing_all_outputs) + " tasks have none of its outputs assigned.\n";
      able_to_run = false;
    }

    if (disconnected_repos > 0)
    {
      status_msg = status_msg + "  - "+  QString::number(disconnected_repos) + " repositories are completely disconnected.\n";
    }

    // Are we able to run the workflow?

    status_msg += "\n";

    if (!able_to_run)
    {
      status_msg += "If saved, IT WON'T BE POSSIBLE to run this workflow.\n";
    }
    else
    {
      if (missing_some_outputs > 0)
      {
        status_msg += "If this workshop is saved and executed,\nall the unassigned outputs will be deleted.\n";
      }
    }

    status_msg += "\n";

    status_msg += "However, it is possible to save the workflow.\nDo you want yo save it?";

    // That's all.

    return true;
  }
}

void
MainWindow::
buttonGroupClicked
(QAbstractButton *button)
{
  {
    const QList<QAbstractButton *> buttons = buttonGroup->buttons();

    for (QAbstractButton *myButton : buttons)
    {
      if (myButton != button) button->setChecked(false);
    }

    const int id = buttonGroup->id(button);

    scene->setItemType(DiagramItem::DiagramType(id));
    scene->setMode(DiagramScene::InsertItem);
  }
}

void
MainWindow::
createActions
(void)
{
  {
    toFrontAction = new QAction(QIcon(":/resources/bringtofront.png"),
                                tr("Bring to &Front"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Bring item to front"));
    connect(toFrontAction, &QAction::triggered, this, &MainWindow::bringToFront);

    sendBackAction = new QAction(QIcon(":/resources/sendtoback.png"), tr("Send to &Back"), this);
    sendBackAction->setShortcut(tr("Ctrl+T"));
    sendBackAction->setStatusTip(tr("Send item to back"));
    connect(sendBackAction, &QAction::triggered, this, &MainWindow::sendToBack);

    deleteAction = new QAction(QIcon(":/resources/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from workflow"));
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteItem);

    openWorkflowAction = new QAction(tr("&Open"), this);
    openWorkflowAction->setShortcuts(QKeySequence::Open);
    openWorkflowAction->setStatusTip(tr("Open an existing workflow"));
    connect(openWorkflowAction, &QAction::triggered, this, &MainWindow::openWorkflow);

    newWorkflowAction = new QAction(tr("&New"), this);
    newWorkflowAction->setShortcuts(QKeySequence::New);
    newWorkflowAction->setStatusTip(tr("Create a new workflow"));
    connect(newWorkflowAction, &QAction::triggered, this, &MainWindow::newWorkflow);

    saveWorkflowAction = new QAction(tr("&Save"), this);
    saveWorkflowAction->setShortcuts(QKeySequence::Save);
    saveWorkflowAction->setStatusTip(tr("Save the current workflow"));
    connect(saveWorkflowAction, &QAction::triggered, this, &MainWindow::saveWorkflow);

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Leave the application"));
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setShortcut(tr("F1"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
  }
}

QWidget*
MainWindow::
createCellWidget
(const QString&                 text,
       DiagramItem::DiagramType type)
{
  {
    toolkit_task tk;

    DiagramItem item(type, tk, itemMenu);
    QIcon icon(item.image());

    QToolButton *button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    buttonGroup->addButton(button, int(type));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
  }
}

void
MainWindow::
createMenus
(void)
{
  {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openWorkflowAction);
    fileMenu->addAction(newWorkflowAction);
    fileMenu->addAction(saveWorkflowAction);
    fileMenu->addAction(exitAction);

    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);

    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(aboutAction);
  }
}

void
MainWindow::
createToolbars
(void)
{
  {
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(toFrontAction);
    editToolBar->addAction(sendBackAction);

    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/resources/pointer.png"));
    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/resources/linepointer.png"));

    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton, int(DiagramScene::InsertLine));
    connect(pointerTypeGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, &MainWindow::pointerGroupClicked);

    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(2);
    connect(sceneScaleCombo, &QComboBox::currentTextChanged,
            this, &MainWindow::sceneScaleChanged);

    pointerToolbar = addToolBar(tr("Pointer type"));
    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(linePointerButton);
    pointerToolbar->addWidget(sceneScaleCombo);
  }
}

void
MainWindow::
createToolBox
(void)
{
  {
    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(false);
    connect(buttonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, &MainWindow::buttonGroupClicked);
    QGridLayout *layout = new QGridLayout;


    layout->addWidget(createCellWidget(tr("Task"), DiagramItem::Task),0, 0);
    layout->addWidget(createCellWidget(tr("Repository"), DiagramItem::Repository), 1, 0);

    layout->setRowStretch(3, 10);
    layout->setColumnStretch(2, 10);

    QWidget *itemWidget = new QWidget;
    itemWidget->setLayout(layout);

    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox->addItem(itemWidget, tr("Workflow items"));
  }
}

void
MainWindow::
deleteItem
(void)
{
  {
    QList<QGraphicsItem *> selectedItems = scene->selectedItems();
    for (QGraphicsItem *item : qAsConst(selectedItems))
    {
      if (item->type() == connection::Type)
      {
        scene->removeItem(item);
        connection *conn = qgraphicsitem_cast<connection *>(item);

        //
        // Change the status of the start and ending points. In
        // the case of output connections we must decrease the connections
        // counter by one; when dealing with input connections, we
        // must set its status to "Disconnected".
        //
        // Since connections are stored TWICE (once per task involved
        // in the connection) we only need to perform the steps
        // above once; otherwise, we would decrease the connections
        // counter twice instead of once.
        //

        subnode_data start_node, end_node;
        start_node = conn->startNode();
        end_node   = conn->endNode();

        conn->startItem()->update_output_connection(start_node, -1);
        conn->endItem()->update_input_connection(end_node, Disconnected);

        // Remove the connections from the starting and ending tasks.

        conn->startItem()->remove_connection(conn);
        conn->endItem()->remove_connection(conn);

        // Delete the connection itself.

        delete item;
      }
    }

    selectedItems = scene->selectedItems();
    for (QGraphicsItem *item : qAsConst(selectedItems))
    {
       if (item->type() == DiagramItem::Type)
         qgraphicsitem_cast<DiagramItem *>(item)->remove_connections();
       scene->removeItem(item);
       delete item;
     }
  }
}

#ifdef __GNUC__

string
MainWindow::
get_executable_path
(void)
{
  {
    char result[PATH_MAX] = {};
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    const char *path;
    if (count != -1)
    {
      path = dirname(result);
      return string(path);
    }

    return "";
  }
}
#else
string
MainWindow::
get_executable_path
(void)
{
  {
    char buffer[MAX_PATH];

    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");

    return std::string(buffer).substr(0, pos);
  }
}
#endif

void
MainWindow::
itemInserted
(DiagramItem *item)
{
  {
    pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
    buttonGroup->button(int(item->diagramType()))->setChecked(false);
  }
}

bool
MainWindow::
load_toolkit
(QString& tk_path,
 toolkit& tk)
{
  {
    string         error_message;
    QString        schema_file;
    int            status;
    toolkit_parser tk_parser;

    //
    // Get the path to the schema defining the toolkit files.
    // It resides in the same folder than our executable file.
    //

    schema_file = QString::fromStdString(get_executable_path() + "/toolkit.xsd");

    // Check the validity of the input file using the schema.

    status = tk_parser.set_schema(schema_file);

    if (!status)
    {
      vector<string> errors = tk_parser.error_list();

      error_message  = "Unable to load the schema definition for toolkit files.\n";
      error_message += "Maybe reinstalling the application would solve the problem.\n";
      error_message += "The precise error messages detected are listed below:\n";

      for (size_t i = 0; i < errors.size(); i++)
      {
        error_message = error_message + "  " + errors[i] + "\n";
      }

      QMessageBox msgBox;
      msgBox.setText(QString::fromStdString(error_message));
      msgBox.setWindowTitle("Error(s) loading the toolkit schema");
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();
      return false;
    }

    // Try to parse the toolkit file, since it could be erroneous.

    status = tk_parser.parse(tk_path, tk);
    if (!status)
    {
      vector<string> error_list;

      //
      // We've got problems when parsing the toolkit file.
      // Notify about the issue and go away!
      //

      error_list = tk_parser.error_list();

      error_message  = "Errors detected when loading the toolkit file.\n";
      error_message += "See the list of detected errors below:\n";

      for (size_t i = 0; i < error_list.size(); i++)
      {
        error_message = error_message + "  " + error_list[i] + "\n";
      }

      QMessageBox msgBox;
      msgBox.setText(QString::fromStdString(error_message));
      msgBox.setWindowTitle("Error(s) reading the toolkit file");
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();
      return false;
    }

    // That's all.

    return true;
  }
}

bool
MainWindow::
load_workflow
(QString&    wf_path,
 WFWorkflow& wf)
{
  {
    string          error_message;
    QString         schema_file;
    bool            status;
    workflow_parser wf_parser;

    //
    // Get the path to the schema defining the workflow files.
    // It resides in the same folder than our executable file.
    //

    schema_file = QString::fromStdString(get_executable_path() + "/workflow.xsd");

    // Check the validity of the input file using the schema.

    status = wf_parser.set_schema(schema_file);

    if (!status)
    {
      vector<string> errors = wf_parser.error_list();

      error_message  = "Unable to load the schema definition for workflow files.\n";
      error_message += "Maybe reinstalling the application would solve the problem.\n";
      error_message += "The precise error messages detected are listed below:\n";

      for (size_t i = 0; i < errors.size(); i++)
      {
        error_message = error_message + "  " + errors[i] + "\n";
      }

      QMessageBox msgBox;
      msgBox.setText(QString::fromStdString(error_message));
      msgBox.setWindowTitle("Error(s) loading the workflow schema");
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();
      return false;
    }

    // Try to parse the workflow file, since it could be erroneous.

    status = wf_parser.parse(wf_path, wf);
    if (!status)
    {
      vector<string> error_list;

      //
      // We've got problems when parsing the workflow file.
      // Notify about the issue and go away!
      //

      error_list = wf_parser.error_list();

      error_message  = "Errors detected when loading the workflow file.\n";
      error_message += "See the list of detected errors below:\n";

      for (size_t i = 0; i < error_list.size(); i++)
      {
        error_message = error_message + "  " + error_list[i] + "\n";
      }

      QMessageBox msgBox;
      msgBox.setText(QString::fromStdString(error_message));
      msgBox.setWindowTitle("Error(s) reading the workflow file");
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();
      return false;
    }

    // That's all.

    return true;
  }
}

MainWindow::
MainWindow
(void)
{
  {
    string  sversion;
    QString version;
    QString window_title;

    // Get the WorkflowMaker package's current version.

    sversion = version_string();
    version  = QString::fromStdString(sversion);
    window_title = "WorkflowEditor version " + version;

    // Create an instance of the About box, so it may be
    // shown each time the user wants to see it.

    about_box_ = new AboutBox(version);

    // Initialize

    wf_id_          = "";
    wf_description_ = "";
    open_document_  = false;

    // Create the several elements integrating the interface.

    createActions();
    createToolBox();
    createMenus();

    //
    // Create the underlying scene and make the necessary
    // connections to let it work.
    //

    scene = new DiagramScene(itemMenu, this);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));

    connect(scene, &DiagramScene::itemInserted,
            this, &MainWindow::itemInserted);

    //
    // Create the toolbars HERE. Otherwise (?) it
    // does not work.
    //

    createToolbars();

    // Arrange the graphics components.

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(toolBox);
    view = new QGraphicsView(scene);
    layout->addWidget(view);
    task_panel_ = new task_info_panel();
    layout->addWidget(task_panel_);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
    setWindowTitle(window_title);
    setUnifiedTitleAndToolBarOnMac(true);

    // Connections.

    connect (task_panel_,     SIGNAL (row_selected      (int)),
             this,            SLOT   (task_selected     (int)));

    connect (&wf_data_widget, SIGNAL (data_is_available ()),
             this,            SLOT   (new_wf_data_ready ()));

    setWindowIcon(QIcon(":/resources/WFMEditor_128.png"));

    // Set window flags to include all except the close button

    setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
  }
}

void
MainWindow::
newWorkflow
(void)
{
  {
    //
    // We'll open a workflow providing there is not another one
    // already opened.
    //

    if (open_document_)
    {
      QMessageBox msgBox;
      QString     message;

      message  = "There is another workflow already opened\n";
      message += "by this application.\n\n";
      message += "To open a new workflow, please save your work\n";
      message += "and restart the application.";

      msgBox.setText(message);
      msgBox.setWindowTitle("Can not open a new workflow");
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();

      return;
    }

    wf_data_widget.show();
  }
}

void
MainWindow::
new_wf_data_ready
(void)
{
  {
    toolkit tk;
    QString tk_path;
    QString wf_id;
    QString wf_description;

    // Get the data input by the user.

    wf_data_widget.get_dialog_data(wf_id, wf_description, tk_path);

    // Parse the toolkit.

    if (!load_toolkit(tk_path, tk)) return;

    // The toolkit has been properly loaded.

    //
    // Sort the list of tasks, so they are ordered by
    // their task identifier. This will help us later on
    // to identify what's the task selected in our task
    // info panel.
    //

    std::sort(tk.tasks.begin(), tk.tasks.end());

    // Tell our task info panel and scene that we've got a toolkit.

    task_panel_->set_toolkit(tk);
    scene->setToolkit(tk);

    // Set the values of the variables related to the workflow lineage.

    wf_id_          = wf_id.toStdString();
    wf_description_ = wf_description.toStdString();

    // We have an open workflow!!!

    open_document_ = true;
  }
}

void
MainWindow::
openWorkflow
(void)
{
  {
    QString        banner;
    QString        message;
    QMessageBox    msgBox;
    bool           status;
    toolkit        tk;
    QString        tk_id;
    WFWorkflow     wf;
    QString        wf_id;

    //
    // We'll open a workflow providing there is not another one
    // already opened.
    //

    if (open_document_)
    {
      message  = "There is another workflow already opened\n";
      message += "by this application.\n\n";
      message += "To open a new workflow, please save your work\n";
      message += "and restart the application.";

      msgBox.setText(message);
      msgBox.setWindowTitle("Can not open a new workflow");
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();

      return;
    }

    // First of all, load a workflow.

    if (!selectAndOpenWorkflow(wf)) return;

    // Get the identifier of the workflow and underlying toolkit.

    wf_id = QString::fromStdString(wf.id);
    tk_id = QString::fromStdString(wf.toolkit_id);

    // Set the values of the variables related to the workflow lineage.

    wf_id_          = wf.id;
    wf_description_ = wf.description;

    //
    // Tell the user that (s)he must load the toolkit on which
    // the workflow relies.
    //

    message  = "The workflow you are loading relies on a toolkit\n";
    message += "whose identifier is '" + tk_id + "'.\n\n";
    message += "Please, locate the file defining the toolkit above\n";
    message += "to finalize the load of the workflow you selected.";

    msgBox.setText(message);
    msgBox.setWindowTitle("Please, locate the toolkit used by your workflow");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();

    // Load the related toolkit.

    banner = "Select the file with the toolkit with id '" + tk_id + "'";
    status = selectAndOpenToolkit(banner, tk);

    if (!status) return;

    //
    // Check that the toolkit just loaded is the one we need.
    // The user may have loaded a toolkit that is not the
    // one used by our workflow!!!
    //

    QString loaded_tk_id = QString::fromStdString(tk.id);
    if (tk_id != loaded_tk_id)
    {
      message  = "The toolkit just loaded is not the one needed by the workflow,\n";
      message += "since its identifier is '" + loaded_tk_id;
      message += "' instead of '" + tk_id + "'.\n\n";
      message += "Please, try again.";

      msgBox.setText(message);
      msgBox.setWindowTitle("Invalid toolkit selected");
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();

      return;
    }

    //
    // Sort the list of tasks, so they are ordered by
    // their task identifier. This will help us later on
    // to identify what's the task selected in our task
    // info panel.
    //

    std::sort(tk.tasks.begin(), tk.tasks.end());

    // Tell our task info panel and scene that we've got a toolkit.

    task_panel_->set_toolkit(tk);
    scene->setToolkit(tk);

    //
    // Tell our scene to add the items (tasks, repos, connections)
    // defined in our workflow.
    //
    // Note that we will ignore the return code issued by
    // insert_workflow below. This is so because the only reason
    // why this method will fail is the absence of a toolkit, and
    // we've set the scene's toolkit just above... so no
    // errors will happen!
    //

    scene->insert_workflow(wf);

    // We have an open workflow!!!

    open_document_ = true;

    // That's all.

    return;
  }
}

void
MainWindow::
pointerGroupClicked
(void)
{
  {
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
  }
}

void
MainWindow::
saveWorkflow
(void)
{
  {
    QString         destFile;
    int             disconnected_repos;
    QStringList     fileNames;
    int             missing_inputs;
    int             missing_some_outputs;
    int             missing_all_outputs;
    bool            need_to_confirm;
    int             status;
    QString         status_msg;
    WFWorkflow      workflow;
    workflow_writer wfr;

    // Try to build the curren workflow. The scene knows how to do it.

    status = scene->build_workflow(workflow);

    if (status == 1)
    {
      // Nothing to save!

      QMessageBox msgBox;
      msgBox.setText("The current workflow is empty.\nThere is nothing to save.");
      msgBox.setWindowTitle("Empty workflow");
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();

      return;
    }

    //
    // Check what's the status of the workflow to see whether it is
    // incomplete...
    //

    scene->check_wf_status(missing_inputs     , missing_some_outputs,
                           missing_all_outputs, disconnected_repos);

    //
    // Decide whether to warn the users about the state of the
    // workflow, building a message if necessary to show to them...
    //

    need_to_confirm = build_wf_status_msg (missing_inputs,
                                           missing_some_outputs,
                                           missing_all_outputs,
                                           disconnected_repos,
                                           status_msg);

    if (need_to_confirm)
    {
      QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Save the current workflow?", status_msg,
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::No) return;
    }

    //
    // No matter the status of the workflow, we have to save it.
    //
    // Ask for the file name where the workflow will be saved.
    //

    QFileDialog dialog(this);
    dialog.setWindowTitle(tr("Name of the output file to save the workflow"));
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("Workflow files (*.xml)");

    if (!dialog.exec()) return;

    fileNames = dialog.selectedFiles();
    destFile = fileNames[0];

    // Assign the workflow id and description.

    workflow.id = wf_id_;
    workflow.description = wf_description_;

    // Save the workflow to a text file.

    if (!wfr.write(workflow, destFile))
    {
      QMessageBox msgBox;
      QString     message;

      message  = "Unable to save the workflow file.\n";
      message += destFile;

      msgBox.setText(message);
      msgBox.setWindowTitle("Error writing a workflow");
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();

      return;
    }

    // Tell the user that everything went OK.

    QMessageBox msgBox;
    QString     message;

    message  = "Workflow successfully saved to file:\n";
    message += destFile;

    msgBox.setText(message);
    msgBox.setWindowTitle("Successful completion");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();

    // That's all.

    return;
  }
}

void
MainWindow::
sceneScaleChanged
(const QString &scale)
{
  {
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QTransform oldMatrix = view->transform();
    view->resetTransform();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);
  }
}

bool
MainWindow::
selectAndOpenToolkit
(QString& banner,
 toolkit& tk)
{
  {
    string         error_message;
    QStringList    file_names;
    QString        path;
    QString        schema_file;

    // First, get the name of the toolkit file to load.

    QFileDialog dialog(this);
    dialog.setWindowTitle(banner);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("Toolkit file (*.xml)");

    if (!dialog.exec()) return false;

    file_names = dialog.selectedFiles();

    path = file_names[0];

    //
    // Parse the toolkit. Our return code is the one returned
    // by load_toolkit.
    //

    return load_toolkit(path, tk);
  }
}

bool
MainWindow::
selectAndOpenWorkflow
(WFWorkflow& wf)
{
  {
    string         error_message;
    QStringList    file_names;
    QString        path;
    QString        schema_file;

    // First, get the name of the workflow file to load.

    QFileDialog dialog(this);
    dialog.setWindowTitle(tr("Please, select an existing workflow file"));
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("Workflow file (*.xml)");

    if (!dialog.exec()) return false;

    file_names = dialog.selectedFiles();

    path = file_names[0];

    //
    // Parse the workflow. Our return code is the one returned
    // by load_workflow.
    //

    return load_workflow(path, wf);
  }
}

void
MainWindow::
sendToBack
(void)
{
  {
    if (scene->selectedItems().isEmpty())
      return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    const QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    for (const QGraphicsItem *item : overlapItems)
    {
      if (item->zValue() <= zValue && item->type() == DiagramItem::Type)
        zValue = item->zValue() - 0.1;
    }
    selectedItem->setZValue(zValue);
  }
}

void
MainWindow::
task_selected
(int task_number)
{
  {
    scene->setCurrentTask(task_number);
  }
}

string
MainWindow::
version_string
(void)
{
  string version;

  {
    ifstream version_file;
    string   version_file_name;

    //
    // Get the path of the file containing the version string.
    // It is (should be) located in the application's directory.
    //

    version_file_name = get_executable_path()
                        + "/workflowmaker_version.txt";

    //
    // Open and read the version file. Should this file, return
    // 'Unknown WorkflowMaker version' as the result.
    //

    version = "'Unknown WorkflowMaker version'";

    version_file.open(version_file_name);

    if (version_file.is_open())
    {
      getline(version_file, version);
      version_file.close();
    }
  }

  return version;
}
