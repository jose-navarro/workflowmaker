/** \file WorkflowLauncher_widget.cpp
\brief Implementation file for WorkflowLauncher_widget.hpp
*/

#include "WorkflowLauncher_widget.hpp"

void
WorkflowLauncher_widget::
add_disconnected_files
(vector<WLFile>& dfiles)
{
  {
    // Clean the list of disconnected files.

    dfiles.clear();

    //
    // Create a set with the connections that do exist,
    // so we have an inventory of what are the output
    // files that have been assigned by the user.
    // This means looking for the ttr and ttt
    // connections. The rtt ones are ignored since
    // these files are not the output of any task.
    //
    // Note that the set will keep only a single
    // copy of potentially duplicated connections.
    //

    //
    // First, search the ttr / rtt connections set. Keep only
    // the ttr ones. We do NOT look for rtt connections since
    // these are NOT output files.
    //

    set<WLConnKey> conn_set;

    for (size_t i = 0; i < file_list_repos_.size(); i++)
    {
      WLFile fil = file_list_repos_[i];
      if (fil.start_node_type == TaskEndPoint)
      {
        WLConnKey key;

        key.num_id = fil.start_node_nid;
        key.slot   = fil.start_node_slot;

        conn_set.insert(key);
      }

    }

    //
    // Now, the ttt connections. These always start and end
    // at a task, so no checks about what kind of endpoint
    // is the starting one is needed.
    //

    for (size_t i = 0; i < file_list_tasks_.size(); i++)
    {
      WLFile fil = file_list_tasks_[i];
      WLConnKey key;

      key.num_id = fil.start_node_nid;
      key.slot   = fil.start_node_slot;

      conn_set.insert(key);
    }

    //
    // Build a map that will let us find a tookit task using
    // its string identifier.
    //

    map<string, int> task_id_to_pos;

    for (size_t i = 0; i < toolkit_.tasks.size(); i++)
    {
      toolkit_task tkt = toolkit_.tasks[i];
      task_id_to_pos.insert({tkt.id, (int)i});
    }

    //
    // Build another map that will let us find a file type
    // using its file type identifier.
    //

    map<string, int> ftyp_to_pos;

    for (size_t i = 0; i < toolkit_.file_types.size(); i++)
    {
      ftyp_to_pos.insert({toolkit_.file_types[i].id, (int)i});
    }

    //
    // Now, we'll iterate over the tasks in the workflow and
    // see what are their output slots that have never been
    // assigned. For these, we'll create new connections!
    //

    for (size_t tskidx = 0; tskidx < workflow_.tasks.size(); tskidx++)
    {
      // Get the current task.

      WFNode wftask = workflow_.tasks[tskidx];

      // Get the toolkit task describing the workflow one.

      map<string,int>::iterator tsk_finder;
      tsk_finder       = task_id_to_pos.find(wftask.id);
      toolkit_task tkt = toolkit_.tasks[tsk_finder->second];

      //
      // Iterate over the output files, to see if any of these
      // has been left unassigned by the user.
      //

      for (size_t slotidx = 0; slotidx < tkt.output_files.size(); slotidx++)
      {
        set<WLConnKey>::iterator key_finder;
        WLConnKey                slotkey;

        slotkey.num_id = wftask.numerical_id;
        slotkey.slot   = (int)slotidx;

        key_finder = conn_set.find(slotkey);

        if (key_finder == conn_set.end())
        {
          //
          // We have found an unassigned slot!!! We must create a new
          // connection (file).
          //

          map<string, int>::iterator ftyp_finder;
          int                        ftpos;
          string                     numid;
          toolkit_file               tfile;
          WLFile                  wfile;

          // Get the description of the current file.

          tfile                       = tkt.output_files[slotidx];

          //
          // Assign the fields for the new file, using the information found
          // in the current workflow task and the description of the current file.
          //

          wfile.start_node_id         = wftask.id;
          wfile.start_node_nid        = wftask.numerical_id;
          wfile.start_node_seqid      = wftask.sequential_id;
          wfile.start_node_slot       = (int)slotidx;
          wfile.start_node_type       = TaskEndPoint;

          wfile.end_node_id           = "";
          wfile.end_node_nid          = -1;
          wfile.end_node_seqid        = -1;
          wfile.end_node_slot         = -1;
          wfile.end_node_type         = WasteBasketEndPoint;

          wfile.delete_after_task     = (int)tskidx;
          wfile.erasable              = true;

          wfile.start_node_opf_label  = tfile.options_file_label;
          wfile.end_node_opf_label    = wfile.start_node_opf_label;
          wfile.description           = tfile.description;
          wfile.file_type_id          = tfile.file_type_id;

          ftyp_finder = ftyp_to_pos.find(tfile.file_type_id);
          ftpos = ftyp_finder->second;

          wfile.file_type_description = toolkit_.file_types[ftpos].description;
          wfile.extension             = toolkit_.file_types[ftpos].extension;

          numid                       = (QString::number(wftask.sequential_id) + "_" + QString::number(slotidx)).toStdString();

          wfile.filename              = work_dir_path_ + wftask.id + "_" + numid + "." + wfile.extension;

          // Let's add the new connection to our array of unassigned ones.

          dfiles.push_back(wfile);

        }
      }
    }

    // That's all.

    return;
  }
}

void
WorkflowLauncher_widget::
build_full_filelist
(void)
{
  {
    map<string, string> file_type_to_description;
    map<string, string> file_type_to_extension;
    map<int,    string> repo_nid_to_id;
    map<string, int>    task_id_to_idx;
    map<int,    string> task_nid_to_id;
    map<int,    int>    task_nid_to_seqid;


    //
    // Build the maps that will let us go from numerical to
    // string identifiers.
    //

    repo_nid_to_id.clear();

    for (size_t i = 0; i < workflow_.repos.size(); i++)
    {
      WFNode repo = workflow_.repos[i];
      string id   = repo.id;
      int    nid  = repo.numerical_id;

      repo_nid_to_id.insert({nid, id});
    }

    task_nid_to_id.clear();
    task_nid_to_seqid.clear();

    for (size_t i = 0; i < workflow_.tasks.size(); i++)
    {
      WFNode task  = workflow_.tasks[i];
      string id    = task.id;
      int    nid   = task.numerical_id;
      int    seqid = task.sequential_id;

      task_nid_to_id.insert({nid, id});
      task_nid_to_seqid.insert({nid, seqid});
    }

    //
    // Build the maps that will let us go from tasks and repositories
    // string identifiers to array positions.
    //

    task_id_to_idx.clear();

    for (size_t i = 0; i < toolkit_.tasks.size(); i++)
    {
      task_id_to_idx.insert({toolkit_.tasks[i].id, (int)i});
    }

    //
    // Build the maps that will let us find quickly the
    // description / extension related to some file type.
    //

    file_type_to_description.clear();
    file_type_to_extension.clear();

    for (size_t i = 0; i < toolkit_.file_types.size(); i++)
    {
      toolkit_file_type ft   = toolkit_.file_types[i];
      string            type = ft.id;
      string            desc = ft.description;
      string            ext  = ft.extension;

      file_type_to_description.insert({type, desc});
      file_type_to_extension.insert({type, ext});
    }

    //
    // Now, build the whole list of files.
    //

    file_list_repos_.clear();
    file_list_tasks_.clear();

    for (size_t i = 0; i < workflow_.connections.size(); i++)
    {
      WFConnection   conn;
      WLFile      file;
      ConnectionType type_start;
      ConnectionType type_end;

      conn = workflow_.connections[i];

      type_start = conn.from.type;
      type_end   = conn.to.type;

      // Set the attributes that we already know.

      file.start_node_type = type_start;
      file.end_node_type   = type_end;

      //
      // Let's analyze the initial endpoint. We must tell apart repo vs.
      // task endpoints.
      //

      if (type_start == TaskEndPoint)
      {
        string                        file_type;
        map<string, string>::iterator ftyp_finder;
        map<int, string>::iterator    id_finder;
        string                        id;
        int                           nid;
        int                           pos;
        int                           pos_file;
        map<string, int>::iterator    pos_finder;
        int                           seqid;
        map<int, int>::iterator       seqid_finder;
        toolkit_task                  task;

        // Get the numerical identifier of the task.

        nid                        = conn.from.endpoint_id;

        // From numerical id to string id.

        id_finder                  = task_nid_to_id.find(nid);
        id                         = id_finder->second;

        // From id to array position.

        pos_finder                 = task_id_to_idx.find(id);
        pos                        = pos_finder->second;

        // Get the sequential identifier of the task.

        seqid_finder               = task_nid_to_seqid.find(nid);
        seqid                      = seqid_finder->second;

        // Get the definition of the task.

        task = toolkit_.tasks[pos];

        // Retrieve the info related to the file.

        file.start_node_id         = id;
        file.start_node_nid        = nid;
        file.start_node_seqid      = seqid;

        pos_file                   = conn.from.slot;
        file.start_node_slot       = pos_file;

        file.description           = task.output_files[pos_file].description;
        file.start_node_opf_label  = task.output_files[pos_file].options_file_label;

        file_type                  = task.output_files[pos_file].file_type_id;
        file.file_type_id          = file_type;
        ftyp_finder                = file_type_to_description.find(file_type);
        file.file_type_description = ftyp_finder->second;
        ftyp_finder                = file_type_to_extension.find(file_type);
        file.extension             = ftyp_finder->second;
      }
      else
      {
        // This is a RepoEndPoint endpoint.

        map<int, string>::iterator id_finder;
        string                     id;
        int                        nid;

        // Get the numerical identifier of the task.

        nid                 = conn.from.endpoint_id;

        // From numerical id to string id.

        id_finder           = repo_nid_to_id.find(nid);
        id                  = id_finder->second;

        // Retrieve the info related to the file.

        file.start_node_id   = id;
        file.start_node_nid  = nid;
        file.start_node_slot = 0;
      }

      //
      // Now, it's time to check the final endpoint. Again, repos
      // are treated in a different way than tasks.
      //

      if (type_end == TaskEndPoint)
      {
        string                        file_type;
        map<string, string>::iterator ftyp_finder;
        map<int, string>::iterator    id_finder;
        string                        id;
        int                           nid;
        int                           pos;
        int                           pos_file;
        map<string, int>::iterator    pos_finder;
        int                           seqid;
        map<int, int>::iterator       seqid_finder;
        toolkit_task                  task;

        // Get the numerical identifier of the task.

        nid                        = conn.to.endpoint_id;

        // From numerical id to string id.

        id_finder                  = task_nid_to_id.find(nid);
        id                         = id_finder->second;

        // From id to array position.

        pos_finder                 = task_id_to_idx.find(id);
        pos                        = pos_finder->second;

        // Get the sequential identifier of the task.

        seqid_finder               = task_nid_to_seqid.find(nid);
        seqid                      = seqid_finder->second;

        // Get the definition of the task.

        task = toolkit_.tasks[pos];

        // Retrieve the info related to the file.

        file.end_node_id           = id;
        file.end_node_nid          = nid;
        file.end_node_seqid        = seqid;

        pos_file                   = conn.to.slot;
        file.end_node_slot         = pos_file;

        file.description           = task.input_files[pos_file].description;
        file.end_node_opf_label    = task.input_files[pos_file].options_file_label;
        file_type                  = task.input_files[pos_file].file_type_id;
        file.file_type_id          = file_type;
        ftyp_finder                = file_type_to_description.find(file_type);
        file.file_type_description = ftyp_finder->second;
        ftyp_finder                = file_type_to_extension.find(file_type);
        file.extension             = ftyp_finder->second;
      }
      else
      {
        // This is a RepoEndPoint endpoint.

        map<int, string>::iterator id_finder;
        string                     id;
        int                        nid;

        // Get the numerical identifier of the task.

        nid                 = conn.to.endpoint_id;

        // From numerical id to string id.

        id_finder           = repo_nid_to_id.find(nid);
        id                  = id_finder->second;

        // Retrieve the info related to the file.

        file.end_node_id   = id;
        file.end_node_nid  = nid;
        file.end_node_slot = 0;
      }

      //
      // Insert the file definition into the appropriate array:
      // Task-to-task connections are separated from these in
      // which a repository is involved.
      //

      if ((file.start_node_type == TaskEndPoint) && (file.end_node_type == TaskEndPoint))
      {
        // Task to task connection / file.

        file_list_tasks_.push_back(file);

        //
        // We build the name of the file using the string and sequential
        // identifiers and slots involved in each task.
        //

        QString first, second;

        first  = QString::fromStdString(file.start_node_id)
               + "_"
               + QString::number(file.start_node_seqid)
               + "_"
               + QString::number(file.start_node_slot);

        second = QString::fromStdString(file.extension);

        file.filename = (first + "." + second).toStdString();
      }
      else
      {
        // There is a repository involved in this connection / file.

        file_list_repos_.push_back(file);

        // The file name must be provided by the user.

        file.filename = "";
      }
    }
  }
}

void
WorkflowLauncher_widget::
build_tasks_file_sets
(vector<WLFile>&               files,
 map<WLTaskKey, set<WLFile*>>& fsets)
{
  {
    for (size_t i = 0; i < files.size(); i++)
    {
      WLFile                                 fil;
      map<WLTaskKey, set<WLFile*>>::iterator finder;
      WLTaskKey                                 key;

      fil = files[i];

      if (fil.start_node_type == TaskEndPoint)
      {
        key.task_id    = fil.start_node_id;
        key.task_seqid = fil.start_node_seqid;

        finder = fsets.find(key);
        if (finder == fsets.end())
        {
          set<WLFile*> new_set;
          new_set.insert(&(files[i]));
          fsets.insert({key, new_set});
        }
        else
        {
          finder->second.insert(&files[i]);
        }
      }

      if (fil.end_node_type == TaskEndPoint)
      {
        key.task_id    = fil.end_node_id;
        key.task_seqid = fil.end_node_seqid;

        finder = fsets.find(key);
        if (finder == fsets.end())
        {
          set<WLFile*> new_set;
          new_set.insert(&(files[i]));
          fsets.insert({key, new_set});
        }
        else
        {
          finder->second.insert(&files[i]);
        }
      }
    }
  }
}

void
WorkflowLauncher_widget::
build_tasks_param_sets
(map<WLTaskKey, set<WLParameter*>>& psets)
{
  {
    for (size_t i = 0; i < launcher_.parameters.size(); i++)
    {

      map<WLTaskKey, set<WLParameter*>>::iterator finder;
      WLTaskKey                                   key;
      bool                                        ok;
      WLParameter*                                par;

      par            = &(launcher_.parameters[i]);
      key.task_id    = par->task_id;
      key.task_seqid = QString::fromStdString(par->task_seqid).toInt(&ok);

      finder = psets.find(key);

      if (finder == psets.end())
      {
        set<WLParameter*> new_set;
        new_set.insert(par);
        psets.insert({key, new_set});
      }
      else
      {
        finder->second.insert(par);
      }
    }
  }
}

void
WorkflowLauncher_widget::
build_ttr_filemap
(map<WLFile_id, WLFile*>& ttr_map)
{
  {
    // Reset the map, just in case it contains some data.

    ttr_map.clear();

    //
    // Iterate over the existing files (connections) to select
    // only those that (1) start at a task and (2) end at a
    // repository.
    //

    for (size_t i = 0; i < file_list_repos_.size(); i++)
    {
      WLFile fil;

      fil = file_list_repos_[i];

      if ((fil.start_node_type == TaskEndPoint) && (fil.end_node_type == RepoEndPoint))
      {
        WLFile_id id;
        id.numid = fil.start_node_nid;
        id.slot  = fil.start_node_slot;

        ttr_map.insert({id, &(file_list_repos_[i])});
      }
    }
  }
}

void
WorkflowLauncher_widget::
build_ttt_filelist
(void)
{
  {
    map<string, string> file_type_to_description;
    map<string, string> file_type_to_extension;
    map<int,    string> repo_nid_to_id;
    map<string, int>    task_id_to_idx;
    map<int,    string> task_nid_to_id;
    map<int,    int>    task_nid_to_seqid;


    //
    // Build the maps that will let us go from numerical to
    // string identifiers.
    //

    repo_nid_to_id.clear();

    for (size_t i = 0; i < workflow_.repos.size(); i++)
    {
      WFNode repo = workflow_.repos[i];
      string id   = repo.id;
      int    nid  = repo.numerical_id;

      repo_nid_to_id.insert({nid, id});
    }

    task_nid_to_id.clear();
    task_nid_to_seqid.clear();

    for (size_t i = 0; i < workflow_.tasks.size(); i++)
    {
      WFNode task  = workflow_.tasks[i];
      string id    = task.id;
      int    nid   = task.numerical_id;
      int    seqid = task.sequential_id;

      task_nid_to_id.insert({nid, id});
      task_nid_to_seqid.insert({nid, seqid});
    }

    //
    // Build the maps that will let us go from task and repository
    // string identifiers to array positions.
    //

    task_id_to_idx.clear();

    for (size_t i = 0; i < toolkit_.tasks.size(); i++)
    {
      task_id_to_idx.insert({toolkit_.tasks[i].id, (int)i});
    }

    //
    // Build the maps that will let us find quickly the
    // description / extension related to some file type.
    //

    file_type_to_description.clear();
    file_type_to_extension.clear();

    for (size_t i = 0; i < toolkit_.file_types.size(); i++)
    {
      toolkit_file_type ft   = toolkit_.file_types[i];
      string            type = ft.id;
      string            desc = ft.description;
      string            ext  = ft.extension;

      file_type_to_description.insert({type, desc});
      file_type_to_extension.insert({type, ext});
    }

    //
    // Now, build the list of files involved in task to task connections.
    //

    file_list_tasks_.clear();

    for (size_t i = 0; i < workflow_.connections.size(); i++)
    {
      WFConnection   conn;
      WLFile      file;
      ConnectionType type_start;
      ConnectionType type_end;

      conn = workflow_.connections[i];

      type_start = conn.from.type;
      type_end   = conn.to.type;

      if ((type_start == TaskEndPoint) && (type_end == TaskEndPoint))
      {
        // We work with task-to-task connections only.

        string                        file_type;
        map<string, string>::iterator ftyp_finder;
        map<int, string>::iterator    id_finder;
        string                        id;
        int                           nid;
        int                           pos;
        int                           pos_file;
        map<string, int>::iterator    pos_finder;
        int                           seqid;
        map<int, int>::iterator       seqid_finder;
        toolkit_task                  task;

        // Set the attributes that we already know.

        file.start_node_type       = type_start;
        file.end_node_type         = type_end;

        //
        // Let's assign most of the file's attributes using
        // the initial endpoint of the connection.
        //

        // Get the numerical identifier of the 1st task.

        nid                        = conn.from.endpoint_id;

        // From numerical id to string id.

        id_finder                  = task_nid_to_id.find(nid);
        id                         = id_finder->second;

        // From id to array position.

        pos_finder                 = task_id_to_idx.find(id);
        pos                        = pos_finder->second;

        // Get the sequential identifier of the 1st task.

        seqid_finder               = task_nid_to_seqid.find(nid);
        seqid                      = seqid_finder->second;

        // Get the definition of the 1st task.

        task = toolkit_.tasks[pos];

        // Retrieve the info related to the file.

        file.start_node_id         = id;
        file.start_node_nid        = nid;
        file.start_node_seqid      = seqid;

        pos_file                   = conn.from.slot;
        file.start_node_slot       = pos_file;

        file.description           = task.output_files[pos_file].description;
        file.start_node_opf_label  = task.output_files[pos_file].options_file_label;
        file_type                  = task.output_files[pos_file].file_type_id;
        file.file_type_id          = file_type;
        ftyp_finder                = file_type_to_description.find(file_type);
        file.file_type_description = ftyp_finder->second;
        ftyp_finder                = file_type_to_extension.find(file_type);
        file.extension             = ftyp_finder->second;

        //
        // Let's complete the data for this file using the
        // final endpoint of the connection.
        //


        // Get the numerical identifier of the 2nd task.

        nid                        = conn.to.endpoint_id;

        // From numerical id to string id.

        id_finder                  = task_nid_to_id.find(nid);
        id                         = id_finder->second;

        // From id to array position.

        pos_finder                 = task_id_to_idx.find(id);
        pos                        = pos_finder->second;

        // Get the sequential identifier of the 2nd task.

        seqid_finder               = task_nid_to_seqid.find(nid);
        seqid                      = seqid_finder->second;

        // Get the definition of the 2nd task.

        task = toolkit_.tasks[pos];

        //
        // Retrieve the info related to the file (just the
        // remaining fields not set thanks to the 1st task).
        //

        file.end_node_id           = id;
        file.end_node_nid          = nid;
        file.end_node_seqid        = seqid;
        file.end_node_slot         = conn.to.slot;

        pos_file                   = conn.to.slot;
        file.end_node_opf_label    = task.input_files[pos_file].options_file_label;

        //
        // We build the name of the file using the string and sequential
        // identifiers and slots involved in each task.
        //

        QString first, second;

        first  = QString::fromStdString(file.start_node_id)
               + "_"
               + QString::number(file.start_node_seqid)
               + "_"
               + QString::number(file.start_node_slot);

        second = QString::fromStdString(file.extension);

        file.filename = (first + "." + second).toStdString();

        // Insert the file definition into the task-to-task file array.

        file_list_tasks_.push_back(file);
      }
    }

    // That's all.

    return;
  }
}

void
WorkflowLauncher_widget::
build_ttt_filemap
(map<WLFile_id, WLFile*>& ttt_map)
{
  {
    // Reset the map, just in case it contains some data.

    ttt_map.clear();

    //
    // Iterate over the existing files (connections) to select
    // only those that (1) start and end at a task.
    //

    for (size_t i = 0; i < file_list_tasks_.size(); i++)
    {
      WLFile fil;
      WLFile_id id;

      fil = file_list_tasks_[i];

      id.numid = fil.start_node_nid;
      id.slot  = fil.start_node_slot;

      ttt_map.insert({id, &(file_list_tasks_[i])});
    }
  }
}

void
WorkflowLauncher_widget::
build_parameter_list
(map<string, WLParameter>& wlp_id_to_wlp)
{
  {
     map<string, int>                   tkt_id_to_pos;
     map<string, int>                   tkp_id_to_pos;
    //
    // First, create a task id -> position map to quickly find the
    // definition of toolkit tasks.
    //


    tkt_id_to_pos.clear();

    for (size_t tskidx = 0; tskidx < toolkit_.tasks.size(); tskidx++)
    {
      tkt_id_to_pos.insert({toolkit_.tasks[tskidx].id, (int)tskidx});
    }

    //
    // Now, do the same for the toolkit parameters.
    //

    tkp_id_to_pos.clear();

    for (size_t paridx = 0; paridx < toolkit_.parameters.size(); paridx++)
    {
      tkp_id_to_pos.insert({toolkit_.parameters[paridx].id, (int)paridx});
    }

    // Iterate over the workflow tasks to find their parameters.

    wlp_id_to_wlp.clear();

    for (size_t tskidx = 0; tskidx < workflow_.tasks.size(); tskidx++)
    {
      string                     full_tsk_id;
      map<string, int>::iterator tsk_finder;
      map<string, int>::iterator par_finder;
      string                     tsk_id;
      int                        tsk_seqid;
      toolkit_task               tkt;
      WFNode                     wft;

      wft        = workflow_.tasks[tskidx];
      tsk_id     = wft.id;
      tsk_seqid  = wft.sequential_id;

      full_tsk_id    = "( " + QString::number(tsk_seqid).toStdString() + ") " + tsk_id;

      tsk_finder = tkt_id_to_pos.find(wft.id);
      tkt        = toolkit_.tasks[tsk_finder->second];

      for (size_t paridx = 0; paridx < tkt.parameters.size(); paridx++)
      {
        string            full_id;
        toolkit_parameter tkp;
        string            tkp_id;
        WLParameter       wlp;

        tkp_id = tkt.parameters[paridx];
        par_finder = tkp_id_to_pos.find(tkp_id);
        tkp        = toolkit_.parameters[par_finder->second];

        wlp.id          = tkp_id;
        wlp.task_id     = tsk_id;
        wlp.task_seqid  = QString::number(tsk_seqid).toStdString();
        wlp.type        = tkp.type;
        wlp.description = tkp.description;
        wlp.value       = "";

        full_id         = full_tsk_id + " " + wlp.id;

        wlp_id_to_wlp.insert({full_id, wlp});
      }
    }
  }
}

bool
WorkflowLauncher_widget::
check_completeness
(void)
{
  bool result;

  {
    bool result1, result2, result3;

    error_list_.clear();

    result1 = tab_parameters_->check_completeness();
    if (!result1)
    {
      QVector<QString> errors = tab_parameters_->error_list();
      error_list_ += errors;
    }

    result2 = tab_repositories_->check_completeness();
    if (!result2)
    {
      QVector<QString> errors = tab_repositories_->error_list();
      error_list_ += errors;
    }

    result3 = tab_files_->check_completeness();
    if (!result3)
    {
      QVector<QString> errors = tab_files_->error_list();
      error_list_ += errors;
    }

    result = result1 & result2 & result3;
  }

  return result;
}

void
WorkflowLauncher_widget::
complete_launcher_data
(void)
{
  {
    //
    // First, let's complete the parameter information ---------------------
    //

    map<string, int> param_id_to_pos;

    // Build a map to help us find a parameter just using its id.

    param_id_to_pos.clear();

    for (size_t i = 0; i < toolkit_.parameters.size(); i++)
    {
      toolkit_parameter par    = toolkit_.parameters[i];
      string            par_id = par.id;

      param_id_to_pos.insert({par_id, (int)i});
    }

    //
    // Complete the information about parameters in our launcher.
    // Note that we'll sort the parameters using a map.  At the
    // end, we'll replace the original parameters with the
    // sorted ones.
    //

    map<string, WLParameter> map_parameters;
    map_parameters.clear();

    for (size_t i = 0; i < launcher_.parameters.size(); i++)
    {
      WLParameter the_param = launcher_.parameters[i];
      string par_id         = the_param.id;

      string full_id = "(" + the_param.task_seqid + ") " + the_param.task_id + " " + par_id;

      map<string,int>::iterator par_finder = param_id_to_pos.find(par_id);
      int pos = par_finder->second;

      the_param.type        = toolkit_.parameters[pos].type;
      the_param.description = toolkit_.parameters[pos].description;

      map_parameters.insert({full_id, the_param});
    }

    launcher_.parameters.clear();
    map<string, WLParameter>::iterator map_it;

    for (map_it = map_parameters.begin(); map_it != map_parameters.end(); map_it++)
    {
      launcher_.parameters.push_back(map_it->second);
    }

    // Destroy garbage related to parameters.

    param_id_to_pos.clear();
    map_parameters.clear();

    //
    // The information about repositories is already complete --------------
    // There is nothing about them that we have to do here.
    //

    //
    // Now, let's complete the information about files ---------------------
    // (also called "connections").
    //

    // Build maps that will help us to complete the information.

    map<int, int>    task_nid_to_pos;
    map<int, int>    repo_nid_to_pos;
    map<string, int> task_id_to_pos;
    map<string, int> file_type_id_to_pos;

    // Map for tasks: from numerical id to position (worflow)

    task_nid_to_pos.clear();

    for (size_t i = 0; i < workflow_.tasks.size(); i++)
    {
      int nid = workflow_.tasks[i].numerical_id;
      task_nid_to_pos.insert({nid, (int)i});
    }

    // Map for repos: from numerical id to position (workflow).

    repo_nid_to_pos.clear();

    for (size_t i = 0; i < workflow_.repos.size(); i++)
    {
      int nid = workflow_.repos[i].numerical_id;
      repo_nid_to_pos.insert({nid, (int)i});
    }

    // Map for tasks: from identifier to position (toolkit).

    task_id_to_pos.clear();

    for (size_t i = 0; i < toolkit_.tasks.size(); i++)
    {
      string id = toolkit_.tasks[i].id;
      task_id_to_pos.insert({id, (int)i});
    }

    // Maps for file types: from identifier to position.

    for (size_t i = 0; i < toolkit_.file_types.size(); i++)
    {
      string id = toolkit_.file_types[i].id;
      file_type_id_to_pos.insert({id, (int)i});
    }

    //
    // Complete the information related to files (connections) that
    // start or end at a repository.
    //

    map<int,int>::iterator     ii_finder;
    map<string, int>::iterator si_finder;

    for (size_t i = 0; i < launcher_.files.size(); i++)
    {
      ConnectionType type_start = launcher_.files[i].start_node_type;
      ConnectionType type_end   = launcher_.files[i].end_node_type;
      int            nid_start  = launcher_.files[i].start_node_nid;
      int            nid_end    = launcher_.files[i].end_node_nid;

      int            pos_node;
      int            pos_task;
      int            pos_ftype;

      int            slot;
      string         file_type_id;

      //
      // Locate the task or repository for both enpoints and
      // extract some of the info needed to complete
      // our data.
      //

      if (type_start == TaskEndPoint)
      {
        ii_finder = task_nid_to_pos.find(nid_start);
        pos_node = ii_finder->second;

        // The task id and sequential id.

        launcher_.files[i].start_node_id    = workflow_.tasks[pos_node].id;
        launcher_.files[i].start_node_seqid = workflow_.tasks[pos_node].sequential_id;

        // The file type and file type description. General description.

        si_finder = task_id_to_pos.find(launcher_.files[i].start_node_id);
        pos_task  = si_finder->second;

        slot = launcher_.files[i].start_node_slot;

        file_type_id = toolkit_.tasks[pos_task].output_files[slot].file_type_id;

        launcher_.files[i].file_type_id = file_type_id;
        launcher_.files[i].description = toolkit_.tasks[pos_task].output_files[slot].description;

        si_finder = file_type_id_to_pos.find(file_type_id);
        pos_ftype = si_finder->second;

        launcher_.files[i].file_type_description = toolkit_.file_types[pos_ftype].description;
        launcher_.files[i].start_node_opf_label  = toolkit_.tasks[pos_task].output_files[slot].options_file_label;
        launcher_.files[i].end_node_opf_label    = "";
      }
      else
      {
        ii_finder = repo_nid_to_pos.find(nid_start);
        pos_node = ii_finder->second;

        // The repo id and sequential id.

        launcher_.files[i].start_node_id    = workflow_.repos[pos_node].id;
        launcher_.files[i].start_node_seqid = workflow_.repos[pos_node].sequential_id;
      }

      if (type_end == TaskEndPoint)
      {
        ii_finder = task_nid_to_pos.find(nid_end);
        pos_node = ii_finder->second;

        // The task id and sequential id.

        launcher_.files[i].end_node_id    = workflow_.tasks[pos_node].id;
        launcher_.files[i].end_node_seqid = workflow_.tasks[pos_node].sequential_id;

        // The file type and file type description. General description.

        si_finder = task_id_to_pos.find(launcher_.files[i].end_node_id);
        pos_task  = si_finder->second;

        slot = launcher_.files[i].end_node_slot;

        file_type_id = toolkit_.tasks[pos_task].input_files[slot].file_type_id;

        launcher_.files[i].file_type_id = file_type_id;
        launcher_.files[i].description = toolkit_.tasks[pos_task].input_files[slot].description;

        si_finder = file_type_id_to_pos.find(file_type_id);
        pos_ftype = si_finder->second;

        launcher_.files[i].file_type_description = toolkit_.file_types[pos_ftype].description;
        launcher_.files[i].end_node_opf_label    = toolkit_.tasks[pos_task].input_files[slot].options_file_label;
        launcher_.files[i].start_node_opf_label  = "";
      }
      else
      {
        ii_finder = repo_nid_to_pos.find(nid_end);
        pos_node = ii_finder->second;

        // The repo id and sequential id.

        launcher_.files[i].end_node_id    = workflow_.repos[pos_node].id;
        launcher_.files[i].end_node_seqid = workflow_.repos[pos_node].sequential_id;
      }
    }

    //
    // Copy the current list of files involving at least one repository
    // to our "official" list.
    //

    file_list_repos_ = launcher_.files;

    // Remove garbage.

    task_nid_to_pos.clear();
    repo_nid_to_pos.clear();
    task_id_to_pos.clear();
    file_type_id_to_pos.clear();

    // That's all.

    return;
  }
}

void
WorkflowLauncher_widget::
dragEnterEvent
(QDragEnterEvent *event)
{
  {
    if (event->mimeData()->hasUrls())
    {
      // Check if the file extension is valid

      const QUrl url = event->mimeData()->urls().first();
      QString    filePath = url.toLocalFile();
      QFileInfo  fileInfo(filePath);
      QString     suffix;

      //
      // Get the extension of the file being dragged.
      // Make it uppercase to make comparisons easier.
      //

      suffix = fileInfo.suffix().toUpper();

      // We ONLY accept the .xml extension

      if (suffix == "XML")
      {
        // We accept these extensions.

        event->acceptProposedAction();
      }
      else
      {
        // The extension was not one of those we accept.

        event->ignore();
      }
    }
    else
    {
      // There are no URLs, so the event does not involve a file.

      event->ignore();
    }
  }
}

void
WorkflowLauncher_widget::
dropEvent
(QDropEvent *event)
{
  {
    // We only react when the dropped item is a file.

    if (event->mimeData()->hasUrls())
    {
      // Get the file path

      const QUrl url = event->mimeData()->urls().first();
      QString path   = url.toLocalFile();

      //
      // Try to load the launcher-workflow-toolkit chain
      // of files. If everything is OK, then the data
      // from these files will be shown on the screeen.
      // Otherwise, message boxes explaining what the
      // problems found were will be shown.
      //

      load_launcher_chain(path);
    }
  }
}

void
WorkflowLauncher_widget::
find_deletion_points
(vector<vector<string>>& files_to_delete)
{
  {
    // Clear the input array.

    files_to_delete.clear();

    //
    // Create a map that will let us find the position of a
    // task in the sequence of execution just knowing its
    // numerical identifier.
    //

    map<int, int> numid_to_run_pos;
    numid_to_run_pos.clear();

    for (size_t i = 0; i < workflow_.tasks.size(); i++)
    {
      numid_to_run_pos.insert({workflow_.tasks[i].numerical_id, (int)i});
    }

    //
    // Create a map that will store the last position in which a file
    // is used by some task.
    //

    map<string, int> last_file_usage;

    //
    // Check all the task to task connections. We'll look for the
    // starting endpoints, since these stand for places where the files
    // are created. Then, we'll see what are the destination  endpoints,
    // since these tell us what is the task reading the files.
    // We'll keep the index of the task that reads
    // such files LATER, since it is only then that the file may be
    // deleted.
    //

    for (size_t i = 0; i < file_list_tasks_.size(); i++)
    {
      WLFile f;

      f = file_list_tasks_[i];

      if (f.erasable)
      {
        //
        // Only files that have been marked as erasable will
        // be deleted.
        //

        map<string, int>::iterator file_finder;
        string                     file_name;
        int                        num_id;
        map<int, int>::iterator    task_finder;
        int                        task_pos;

        //
        // Determine (1) the numerical identifier of the task reading
        // the file and then (2) its position in the workflow execution
        // order.
        //

        num_id      = f.end_node_nid;
        task_finder = numid_to_run_pos.find(num_id);
        task_pos    = task_finder->second;

        //
        // Create (or update) an entry in (the file -> last task using
        // the file) map.
        //

        file_name   = f.filename;
        file_finder = last_file_usage.find(file_name);

        //
        // If we hadn't identified this file before, create a new
        // entry in the map; otherwise, update the existing one
        // keeping the GREATEST usage position.
        //

        if (file_finder == last_file_usage.end())
        {
          // New entry.

          last_file_usage.insert({file_name, task_pos});
        }
        else
        {
          // Existing entry.

          int old_pos;

          old_pos = file_finder->second;

          if (task_pos > old_pos)
          {
            //
            // Update the entry only when the position just found is greater than
            // the old one!
            //

            last_file_usage[file_name] = task_pos;

          }
        }
      }
    }

    //
    // Include now all the files in the unconnected file list, since
    // these must be always deleted just after they are created.
    //

    for (size_t i = 0; i < file_list_unconnected_.size(); i++)
    {
      WLFile f;
      string    file_name;
      int       task_pos;

      f = file_list_unconnected_[i];

      file_name = f.filename;
      task_pos  = f.delete_after_task;

      // New entry.

      last_file_usage.insert({file_name, task_pos});
    }

    //
    // Now, we will visit the map last_file_usage to invert its contents;
    // that is, now we have <file, run position> and want to obtain something
    // like <run position, list of files>. This is the final result!
    //

    // Create the empty arrays of strings for each position in the output array.

    for (size_t i = 0; i < workflow_.tasks.size(); i++)
    {
      vector<string>files;
      files_to_delete.push_back(files);
    }

    // Walk through the map and insert each file in its corresponding entry.

    map<string, int>::iterator file_iterator;

    for (file_iterator  = last_file_usage.begin();
         file_iterator != last_file_usage.end();
         file_iterator ++)
    {
      {
        string file_name;
        int    position;

        file_name = file_iterator->first;
        position  = file_iterator->second;

        files_to_delete[position].push_back(file_name);
      }
    }

    // That's all.

    return;
  }
}

#ifdef __GNUC__
string
WorkflowLauncher_widget::
get_data_path
(void)
{
  {
    // We store the read-only files in the standard Ubuntu folder.
    return "/usr/share/workflowmaker";
  }
}
#else
string
WorkflowLauncher_widget::
get_data_path
(void)
{
  {
    // Get the path to the executable folder

    fs::path exec_path(get_executable_path());

    // Navigate to the parent directory of the executable

    fs::path parent_path = exec_path.parent_path();

    // Construct the path to the target folder

    fs::path target_path = parent_path / "data";

    // Check if the target folder exists

    if (fs::exists(target_path) && fs::is_directory(target_path))
    {
      return target_path.string();
    }

    // Return an empty string if the folder is not found

    return "";
  }
}
#endif

#ifdef __GNUC__

string
WorkflowLauncher_widget::
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
WorkflowLauncher_widget::
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

bool
WorkflowLauncher_widget::
load_launcher
(QString&    lch_path,
 WLLauncher& lch)
{
  {
    string          error_message;
    QString         schema_file;
    int             status;
    launcher_parser lch_parser;

    //
    // Get the path to the schema defining the launcher files.
    // On Windows it resides in the data folder, which is a sibling
    // of the binary folder (the place where our executable file reside).
    // On Linux, it will be stored in the default share folder for
    // the WorkflowMaker package (typically, /usr/share/workflowmaker).
    // The function get_data_path() below will return the appropriate
    // path for each operating system.
    //

    schema_file = QString::fromStdString(get_data_path() + "/launcher.xsd");

    //
    // THE FOLLOWING BLOCK, CHECKING THE VALIDITY OF THE FILE, IS COMMENTED
    // BECAUSE THE RAPIDXML LIBRARY REPLACING OBSOLETE QT'S ONE HAS NO VALIDATING
    // MECHANISMS. THE CODE IS PRESERVED JUST IN CASE MIGRATING TO ANOTHER XML
    // LIBRARY ALLOW FOR SCHEMA VALIDATION.
    //

    // Check the validity of the input file using the schema.

    /*

    status = lch_parser.set_schema(schema_file);

    if (!status)
    {
      vector<string> errors = lch_parser.error_list();

      error_message  = "Unable to load the schema definition for launcher files.\n";
      error_message += "Maybe reinstalling the application would solve the problem.\n";
      error_message += "The precise error messages detected are listed below:\n";

      for (size_t i = 0; i < errors.size(); i++)
      {
        error_message = error_message + "  " + errors[i] + "\n";
      }

      QMessageBox msgBox;
      msgBox.setText(QString::fromStdString(error_message));
      msgBox.setWindowTitle("Error(s) loading the launcher schema");
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();
      return false;
    }

    */

    // Try to parse the launcher file, since it could be erroneous.

    status = lch_parser.parse(lch_path, lch);
    if (!status)
    {
      vector<string> error_list;

      //
      // We've got problems when parsing the launcher file.
      // Notify about the issue and go away!
      //

      error_list = lch_parser.error_list();

      error_message  = "Errors detected when loading the launcher file.\n";
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

void
WorkflowLauncher_widget::
load_launcher_chain
(QString& lch_path)
{
  {
    QString     banner;
    WLLauncher  lch;
    QString     loaded_tk_id;
    QString     loaded_wf_id;
    QString     message;
    QMessageBox msgBox;
    bool        status;
    toolkit     tk;
    QString     tk_id;
    WFWorkflow  wf;
    QString     wf_id;

    // Try to load the launcher.

    if (!load_launcher(lch_path, lch)) return;

    //
    // We've got a launcher. Try to load the workflow it relies on.
    //

    // First, get its name.

    wf_id = QString::fromStdString(lch.workflow_id);

    //
    // Tell the users that they must load the workflow on which
    // the workflow relies.
    //

    message  = "The launcher you are just loading relies\n";
    message += "on a workflow whose identifier is '" + wf_id + "'.\n\n";
    message += "Please, locate the file defining the workflow above\n";
    message += "to continue the loading of the launcher you selected.";

    msgBox.setText(message);
    msgBox.setWindowTitle("Please, locate the workflow used by your launcher");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();

    // Load the related workflow.

    banner = "Select the file with the workflow with id '" + wf_id + "'";
    status = select_and_open_workflow(banner, wf);

    if (!status) return;

    // Check that the workflow loaded is the right one.

    loaded_wf_id = QString::fromStdString(wf.id);
    if (wf_id != loaded_wf_id)
    {
      message  = "The workflow just loaded is not the one needed by the launcher,\n";
      message += "since its identifier is '" + loaded_wf_id;
      message += "' instead of '" + wf_id + "'.\n\n";
      message += "Please, try again.";

      msgBox.setText(message);
      msgBox.setWindowTitle("Invalid workflow selected");
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();

      return;
    }

    //
    // The workflow is OK.
    // Get the identifier of the underlying toolkit.
    //

    tk_id = QString::fromStdString(wf.toolkit_id);

    //
    // Tell the user that (s)he must load the toolkit on which
    // the workflow relies.
    //

    message  = "The workflow (' " + wf_id + "') for this launcher relies\n";
    message += "also on a toolkit whose identifier is '" + tk_id + "'.\n\n";
    message += "Please, locate the file defining the toolkit above\n";
    message += "to finalize the loading of the launcher you selected.";

    msgBox.setText(message);
    msgBox.setWindowTitle("Please, locate the toolkit used by your launcher + workflow");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();

    // Load the related toolkit.

    banner = "Select the file with the toolkit with id '" + tk_id + "'";
    status = select_and_open_toolkit(banner, tk);

    if (!status) return;

    //
    // Check that the toolkit just loaded is the one we need.
    // The user may have loaded a toolkit that is not the
    // one used by our workflow!!!
    //

    loaded_tk_id = QString::fromStdString(tk.id);
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

    // We've got a launcher, and the related workflow and tookit.

    launcher_ = lch;
    workflow_ = wf;
    toolkit_  = tk;

    //
    // Since the launcher file does not include the structural
    // information about the workflow & toolkit, we must fill
    // the holes using, precisely, our workflow and toolkit...
    //

    complete_launcher_data();

    // Build the list of task-to-task files.

    build_ttt_filelist();

    // Let our widget show the parameters, repositories and files.

    tab_parameters_->set_values(launcher_.parameters);
    tab_repositories_->set_values(launcher_.repositories);
    tab_files_->set_values(launcher_.files);

    // That's all.

    return;
  }
}

bool
WorkflowLauncher_widget::
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
    // On Windows it resides in the data folder, which is a sibling
    // of the binary folder (the place where our executable file reside).
    // On Linux, it will be stored in the default share folder for
    // the WorkflowMaker package (typically, /usr/share/workflowmaker).
    // The function get_data_path() below will return the appropriate
    // path for each operating system.
    //

    schema_file = QString::fromStdString(get_data_path() + "/toolkit.xsd");

    //
    // THE FOLLOWING BLOCK, CHECKING THE VALIDITY OF THE FILE, IS COMMENTED
    // BECAUSE THE RAPIDXML LIBRARY REPLACING OBSOLETE QT'S ONE HAS NO VALIDATING
    // MECHANISMS. THE CODE IS PRESERVED JUST IN CASE MIGRATING TO ANOTHER XML
    // LIBRARY ALLOW FOR SCHEMA VALIDATION.
    //

    // Check the validity of the input file using the schema.

    /*

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

    */

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
WorkflowLauncher_widget::
load_workflow
(QString&    wf_path,
 WFWorkflow& wf)
{
  {
    string          error_message;
    QString         schema_file;
    int             status;
    workflow_parser wf_parser;

    //
    // Get the path to the schema defining the workflow files.
    // On Windows it resides in the data folder, which is a sibling
    // of the binary folder (the place where our executable file reside).
    // On Linux, it will be stored in the default share folder for
    // the WorkflowMaker package (typically, /usr/share/workflowmaker).
    // The function get_data_path() below will return the appropriate
    // path for each operating system.
    //

    schema_file = QString::fromStdString(get_data_path() + "/workflow.xsd");

    //
    // THE FOLLOWING BLOCK, CHECKING THE VALIDITY OF THE FILE, IS COMMENTED
    // BECAUSE THE RAPIDXML LIBRARY REPLACING OBSOLETE QT'S ONE HAS NO VALIDATING
    // MECHANISMS. THE CODE IS PRESERVED JUST IN CASE MIGRATING TO ANOTHER XML
    // LIBRARY ALLOW FOR SCHEMA VALIDATION.
    //

    // Check the validity of the input file using the schema.

    /*

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

    */

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

void
WorkflowLauncher_widget::
new_launcher_file
(void)
{
  {
    QString      message;
    QMessageBox  msgBox;
    bool         status;
    toolkit      tk;
    QString      tk_banner;
    QString      tk_id;
    WFWorkflow   wf;
    QString      wf_path;

    // Hide the launcher data dialog.

    launcher_data_.hide();

    // Get the launcher data from our launcher dialog.

    launcher_data_.get_dialog_data(launcher_id_, launcher_description_, wf_path);

    // Then, load the selected workflow.

    if (!load_workflow(wf_path, wf)) return;

    // Get the identifier of the underlying toolkit.

    tk_id = QString::fromStdString(wf.toolkit_id);

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

    tk_banner = "Select the file with the toolkit with id '" + tk_id + "'";
    status = select_and_open_toolkit(tk_banner, tk);

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

    // We've got a workflow and its tookit.

    workflow_ = wf;
    toolkit_  = tk;

    //
    // Well, at this point all files have been correctly loaded.
    // Set our structures using those coming from the said files
    // and initialize the several tabs in our interface to show
    // the values just loaded.
    //

    // The parameters.

    map<string, WLParameter>           map_params;
    map<string, WLParameter>::iterator map_it;
    vector<WLParameter>                wlparams;

    build_parameter_list(map_params);

    for (map_it = map_params.begin(); map_it != map_params.end(); map_it++)
    {
      wlparams.push_back(map_it->second);
    }

    tab_parameters_->set_values(wlparams);

    // The repositories

    vector<WLRepository> wlrepos;

    for (size_t i = 0; i < workflow_.repos.size(); i++)
    {
      WLRepository wlr;
      WFNode       wfrepo;

      wfrepo = workflow_.repos[i];

      wlr.id   = wfrepo.id;
      wlr.nid  = wfrepo.numerical_id;
      wlr.path = "";

      wlrepos.push_back(wlr);
    }

    tab_repositories_->set_values(wlrepos);

    // The files.

    build_full_filelist();
    tab_files_->set_values(file_list_repos_);

    // We have an open document.

    open_document_ = true;

    // That's all.

    return;
  }
}

void
WorkflowLauncher_widget::
on_export_launcher_file
(void)
{
  {
    bool              correct;
    QString           destFile;
    QString           errors;
    QStringList       file_names;
    QString           message;
    QMessageBox       msgBox;

    //
    // First of all, check that all the requested values have been
    // input.
    //

    correct = check_completeness();

    if (!correct)
    {
      errors = "";
      for (int i = 0; i < error_list_.size(); i++)
        errors = errors + error_list_[i] + "\n";

      reset_error_messages();

      msgBox.setWindowTitle(tr("Can't export the options and shell files: missing data"));
      msgBox.setText(errors);
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);

      msgBox.exec();

      tab_parameters_->reset_error_list();
      tab_repositories_->reset_error_list();
      tab_files_->reset_error_list();

      return;
    }

    //
    // Get the values of all our data from the screen.
    //

    launcher_.id          = launcher_id_.toStdString();
    launcher_.description = launcher_description_.toStdString();
    launcher_.workflow_id = workflow_.id;

    launcher_.parameters.clear();
    launcher_.parameters = tab_parameters_->get_current_parameter_list();

    launcher_.repositories.clear();
    launcher_.repositories = tab_repositories_->get_current_repo_list();

    launcher_.files.clear();
    launcher_.files = tab_files_->get_current_file_list();

    //
    // Build the list of task-to-task files, just in case we didn't do it
    // before.
    //

    build_ttt_filelist();

    //
    // Ask whether the user would like to save the launcher file first.
    //

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Save launcher file?", "Would you like to save the launcher file first?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
      //
      // Get the name of the output launcher file name.
      //

      QFileDialog dialog;
      dialog.setWindowTitle(tr("Name of the output launcher file"));
      dialog.setFileMode(QFileDialog::AnyFile);
      dialog.setAcceptMode(QFileDialog::AcceptSave);
      dialog.setNameFilter("Launcher files (*.xml)");

      if (!dialog.exec()) return;

      file_names = dialog.selectedFiles();

      destFile = file_names[0];

      QFileInfo finfo2(destFile);
      QString suffix = finfo2.suffix();

      if (suffix.compare("xml", Qt::CaseInsensitive) != 0)
      {
        destFile += ".xml";
      }

      //
      // OK, let's write the launcher file.
      //

      launcher_writer lch_writer;
      if (!lch_writer.write(launcher_, destFile))
      {
        string         error_message;
        vector<string> error_list;

        //
        // We've got problems when trying to write the toolkit file.
        // Notify about the issue and go away!
        //

        error_list = lch_writer.error_list();

        error_message  = "Errors detected when writing the launcher file.\n";
        error_message += "See the list of detected errors below:\n";

        for (size_t i = 0; i < error_list.size(); i++)
        {
          error_message = error_message + "  " + error_list[i] + "\n";
        }

        msgBox.setWindowTitle(tr("Error writing a launcher file"));
        msgBox.setText(errors);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);

        msgBox.exec();

        return;
      }

      // Notify the user that (s)he's got a brand new launcher file.

      message  = "Launcher file successfully saved.";

      msgBox.setWindowTitle(tr("File operation finished"));
      msgBox.setText(message);
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);

      msgBox.exec();
    }

    //
    // Saved or not, we must now export the option and shell files.
    //
    // Get the operating system and folder where the files will be
    // exported.
    //

    export_options_widget eow;
    eow.exec();

    if (!eow.get_status()) return; // The user cancelled the operation.

    os_for_export_ = eow.get_os().toStdString();
    work_dir_path_ = (eow.get_path()).toStdString() + "/";

    //
    // Update the list of repo-task-repo files.
    // Note that the task-task files need no such thing.
    //

    file_list_repos_ = launcher_.files;

    //
    // Prepend the path of the work directory or input / output repositories
    // to the different files intervening in the workflow.
    //

    prepend_folder_paths();

    //
    // Make sure that that the files that intervene NOT ONLY in task-to-task
    // connections but ALSO in task-repo ones get the name of the file involved
    // in the task-repo connection.
    //
    // To do this, we first build a map with the tt connections, then another
    // one with the tr ones to make easier the said task.
    //

    map<WLFile_id, WLFile*> ttt_map;
    map<WLFile_id, WLFile*> ttr_map;

    build_ttt_filemap(ttt_map);
    build_ttr_filemap(ttr_map);

    //
    // Now, rename the ttt connections to adopt the corresponding ttr file name.
    // This is necessary when some task's output file is used (1) as the input for
    // other other task but (2) it is also saved to some repository. If this situation
    // arises, we keep the name assigned to the ttr connection everywhere.
    //

    rename_ttt_repeated_files(ttr_map, ttt_map);

    //
    // Now, find out what are the set of parameters required by each task.
    //

    map<WLTaskKey, set<WLParameter*>> params_per_task_map;

    build_tasks_param_sets(params_per_task_map);

    //
    // Create the connections for those output slots that the user didn't
    // assign. These connections (files) must have a name since the
    // corresponding task will create them. These connections are
    // stored in a new files array (file_list_unconnected_).
    //

    add_disconnected_files(file_list_unconnected_);

    //
    // It is the time to determine what are the files that intervene in each
    // step of the workflow. We need this information to generate the
    // several options files, one of them for each task.
    //
    // We'll generate the map calling method build_tasks_file_sets several
    // times.
    //
    // Note that this method may generate duplicates, since it is possible
    // to connect an output file slot to multiple input ones. This will
    // be solved when writing the option files.
    //

    map<WLTaskKey, set<WLFile*>> files_per_task_map;

    build_tasks_file_sets(file_list_repos_,       files_per_task_map);
    build_tasks_file_sets(file_list_tasks_,       files_per_task_map);
    build_tasks_file_sets(file_list_unconnected_, files_per_task_map);

    // We may write the options files right now!

    vector<string> write_errors;

    if (!write_option_files(files_per_task_map, params_per_task_map, write_errors))
    {
      //
      // We've found some problems when trying to write the output
      // option files... Report these to the user.
      //

      string         error_message;

      error_message  = "Errors detected when writing the option files:\n\n";

      for (size_t i = 0; i < write_errors.size(); i++)
      {
        error_message = error_message + "  " + write_errors[i] + "\n";
      }

      msgBox.setWindowTitle(tr("Errors while writing one or more option files"));
      msgBox.setText(errors);
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);

      msgBox.exec();

      return;
    }

    // Now find the points where the task-to-task files may be deleted.

    vector<vector<string>> files_to_delete;
    find_deletion_points (files_to_delete);

    //
    // And to finish: write the shell file in the selected folder for
    // the requested operating system.
    //

    if (!write_shell_file(files_to_delete))
    {
      //
      // We've found some problems when trying to write the output
      // shell file... Report these to the user.
      //

      string error_message;

      error_message  = "Errors detected when writing the shell file:\n\n";

      for (size_t i = 0; i < write_errors.size(); i++)
      {
        error_message = error_message + "  " + write_errors[i] + "\n";
      }

      msgBox.setWindowTitle(tr("Errors while writing one or more option files"));
      msgBox.setText(errors);
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);

      msgBox.exec();

      return;
    }
    else
    {
      // Notify the user that the files have been exported.

      message  = "Shell files successfully exported.\n";
      message += "These have been saved in this folder:\n\n";
      message += QString::fromStdString(work_dir_path_);

      msgBox.setWindowTitle(tr("File operation finished"));
      msgBox.setText(message);
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);

      msgBox.exec();
    }

  }
}

void
WorkflowLauncher_widget::
on_load_launcher_file
(void)
{
  {
    QStringList file_names;
    QString     path;

    // First, get the name of the toolkit file to load.

    QFileDialog dialog(this);
    dialog.setWindowTitle("Please, select the launcher file to load");
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("Launcher file (*.xml)");

    if (!dialog.exec()) return;

    file_names = dialog.selectedFiles();

    path = file_names[0];

    //
    // Try to load the launcher-workflow-toolkit chain
    // of files. If everything is OK, then the data
    // from these files will be shown on the screeen.
    // Otherwise, message boxes explaining what the
    // problems found were will be shown.
    //

    load_launcher_chain(path);

    // That's all.

    return;

  }
}

void
WorkflowLauncher_widget::
on_new_launcher_file
(void)
{
  {

    // We'll refuse to work if a document is already open.

    if (open_document_)
    {
      QMessageBox msgBox;
      QString     message;

      message  = "There is another launcher already opened\n";
      message += "by this application.\n\n";
      message += "To open or create a new launcher, please save your work\n";
      message += "and restart the application.";

      msgBox.setText(message);
      msgBox.setWindowTitle("Can not create a new launcher");
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();

      return;
    }

    // Show the new launcher data dialog to get its lineage.

    launcher_data_.show();

    // That's all.

    return;
  }
}

void
WorkflowLauncher_widget::
on_save_launcher_file
(void)
{
  {
    bool              correct;
    QString           destFile;
    QString           errors;
    QStringList       file_names;
    QString           message;
    QMessageBox       msgBox;

    //
    // First of all, check that all the requested values have been
    // input.
    //

    correct = check_completeness();

    if (!correct)
    {
      errors = "";
      for (int i = 0; i < error_list_.size(); i++)
        errors = errors + error_list_[i] + "\n";

      reset_error_messages();

      msgBox.setWindowTitle(tr("Can't save the launcher file: missing data"));
      msgBox.setText(errors);
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);

      msgBox.exec();

      tab_parameters_->reset_error_list();
      tab_repositories_->reset_error_list();
      tab_files_->reset_error_list();

      return;
    }

    //
    // Get the name of the output launcher file name.
    //

    QFileDialog dialog;
    dialog.setWindowTitle(tr("Name of the output launcher file"));
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter("Launcher files (*.xml)");

    if (!dialog.exec()) return;

    file_names = dialog.selectedFiles();

    destFile = file_names[0];

    QFileInfo finfo2(destFile);
    QString suffix = finfo2.suffix();

    if (suffix.compare("xml", Qt::CaseInsensitive) != 0)
    {
      destFile += ".xml";
    }

    //
    // Get the values of all our data from the screen.
    //

    WLLauncher launcher;

    launcher.id          = launcher_id_.toStdString();
    launcher.description = launcher_description_.toStdString();
    launcher.workflow_id = workflow_.id;

    launcher.parameters.clear();
    launcher.parameters = tab_parameters_->get_current_parameter_list();

    launcher.repositories.clear();
    launcher.repositories = tab_repositories_->get_current_repo_list();

    launcher.files.clear();
    launcher.files = tab_files_->get_current_file_list();

    //
    // OK, let's write the launcher file.
    //

    launcher_writer lch_writer;
    if (!lch_writer.write(launcher, destFile))
    {
      string         error_message;
      vector<string> error_list;

      //
      // We've got problems when trying to write the toolkit file.
      // Notify about the issue and go away!
      //


      error_list = lch_writer.error_list();

      error_message  = "Errors detected when writing the launcher file.\n";
      error_message += "See the list of detected errors below:\n";

      for (size_t i = 0; i < error_list.size(); i++)
      {
        error_message = error_message + "  " + error_list[i] + "\n";
      }

      msgBox.setWindowTitle(tr("Error writing a launcher file"));
      msgBox.setText(errors);
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);

      msgBox.exec();

      return;
    }

    // Notify the user that (s)he's got a brand new launcher file.

    message  = "Launcher file successfully saved.";

    msgBox.setWindowTitle(tr("File operation finished"));
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    msgBox.exec();

  }
}

void
WorkflowLauncher_widget::
prepend_folder_paths
(void)
{
  {
    map<int, int>           repo_numid_to_pos;
    map<int, int>::iterator repo_finder;

    //
    // First, prepend the work directory path to all the
    // task-to-task files.
    //

    for (size_t i = 0; i < file_list_tasks_.size(); i++)
    {
      string cur_path;
      string final_path;

      cur_path = file_list_tasks_[i].filename;
      final_path = work_dir_path_ + cur_path;
      file_list_tasks_[i].filename = final_path;
    }

    //
    // Now, build a map that will let us find a repository
    // just knowing its numerical identifier.
    //

    repo_numid_to_pos.clear();

    for (size_t i = 0; i < launcher_.repositories.size(); i++)
    {
      int numid = launcher_.repositories[i].nid;
      repo_numid_to_pos.insert({numid, (int)i});
    }

    //
    // Finally, prepend the path to the corresponding repository
    // to all these files that either reside or will be stored
    // in a repository.
    //

    for (size_t i = 0; i < file_list_repos_.size(); i++)
    {
      string cur_path;
      string final_path;
      int    numid;
      size_t repo_len;
      string repo_path;
      int    repo_pos;

      // Decide what end of the connection is the repository one.

      if (file_list_repos_[i].start_node_type == RepoEndPoint)
      {
        // Repo to task.

        numid = file_list_repos_[i].start_node_nid;
      }
      else
      {
        // Task to repo.

        numid = file_list_repos_[i].end_node_nid;
      }

      //
      // Find the path to the repository involved in the
      // repo endpoint.
      //

      repo_finder = repo_numid_to_pos.find(numid);
      repo_pos    = repo_finder->second;

      //
      // Build paths. Be careful about the presence / absence of
      // a slash at the end of the repository path.
      //

      cur_path = file_list_repos_[i].filename;
      repo_path = launcher_.repositories[repo_pos].path;
      repo_len  = repo_path.length() - 1;

      if ((repo_path[repo_len] != '/') && (repo_path[repo_len] != '\\'))
        repo_path += "/";

      final_path = repo_path + cur_path;
      file_list_repos_[i].filename = final_path;
    }

    // That's all.

    return;

  }
}

void
WorkflowLauncher_widget::
reset_error_messages
(void)
{
  {
    // Reset all the error messages for all tabs.

    tab_parameters_->reset_error_list();
  }
}

void
WorkflowLauncher_widget::
rename_ttt_repeated_files
(map<WLFile_id, WLFile*>& ttr_map,
 map<WLFile_id, WLFile*>& ttt_map)
{
  {
    map<WLFile_id, WLFile*>::iterator ttt_it;
    map<WLFile_id, WLFile*>::iterator ttr_finder;

    for (ttt_it = ttt_map.begin(); ttt_it != ttt_map.end(); ttt_it++)
    {
      //
      // Get the extended (num. id + slot) identifier of next file
      // from the map of task-to-task ones.
      //

      WLFile_id id = ttt_it->first;

      // Let's check if this file is also in the list of task-to-repo ones.

      ttr_finder          = ttr_map.find(id);
      WLFile* ttt_file = ttt_it->second;

      if (ttr_finder != ttr_map.end())
      {
        //
        // The task-to-task file is also copied to an output
        // repository. Therefore, we'll use the name chosen by
        // the user instead of the automatically generated one.
        //

        WLFile* ttr_file = ttr_finder->second;
        ttt_file->filename  = ttr_file->filename;

        //
        // Finally, and since this file must be copied to a repository,
        // we'll mark it as non erasable automatically.
        //

        ttt_file->erasable = false;
      }
      else
      {
        //
        // This file is never copied to an output repository. Therefore
        // it may be used as soon as it is no longer needed.
        //

        ttt_file->erasable = true;
      }
    }
  }
}

void
WorkflowLauncher_widget::
replace_all_occurrences
(string& the_string,
 string& find_this,
 string& replace_with_this)
{
  {
    size_t pos = 0;

    while (pos += replace_with_this.length())
    {
      pos = the_string.find(find_this, pos);
      if (pos == std::string::npos)
      {
        break;
      }

      the_string.replace(pos, find_this.length(), replace_with_this);
    }
  }
}

bool
WorkflowLauncher_widget::
select_and_open_toolkit
(QString& banner,
 toolkit& tk)
{
  {
    string         error_message;
    QStringList    file_names;
    QString        path;

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
WorkflowLauncher_widget::
select_and_open_workflow
(QString&    banner,
 WFWorkflow& wf)
{
  {
    string         error_message;
    QStringList    file_names;
    QString        path;

    // First, get the name of the workflow file to load.

    QFileDialog dialog(this);
    dialog.setWindowTitle(banner);
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

string
WorkflowLauncher_widget::
version_string
(void)
{
  string version;

  {
    ifstream version_file;
    string   version_file_name;

    //
    // Get the path of the file containing the version string.
    // It is (should be) located in the application's data directory.
    //

    version_file_name = get_data_path()
                        + "/workflowmaker_version.txt";

    //
    // Open and read the version file. If this file does not
    // exist, return 'Unknown WorkflowMaker version' as the result.
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

WorkflowLauncher_widget::
WorkflowLauncher_widget
(QWidget* parent)
    : QWidget(parent)
{
  {
    string               sversion;
    QString              title;
    QString              version;

    // Get the WorkflowMaker package's current version.

    sversion = version_string();
    version  = QString::fromStdString(sversion);

    //
    // Initialize.
    //

    open_document_ = false;
    title          = "WorkflowLauncher " + version;

    //
    // Create the different graphic elements.
    //

    //
    // The tabs with the tookit's description, parameters,
    // file types and tasks.
    //

    tabs_             = new QTabWidget;
    tab_parameters_   = new WorkflowLauncher_parameters_widget();
    tab_repositories_ = new WorkflowLauncher_repos_widget();
    tab_files_        = new WorkflowLauncher_files_widget();

    tabs_->addTab(tab_parameters_,   tr("Parameters"));
    tabs_->addTab(tab_repositories_, tr("Repositories"));
    tabs_->addTab(tab_files_,        tr("Files"));

    tab_parameters_->setMinimumWidth(800);
    tab_repositories_->setMinimumWidth(800);
    tab_files_->setMinimumWidth(800);

    setMinimumHeight(500);

    // Control buttons.

    QHBoxLayout* control_button_layout = new QHBoxLayout;
    control_button_cancel_  = new QPushButton(tr("Quit"));
    control_button_load_    = new QPushButton(tr("Load launcher"));
    control_button_new_     = new QPushButton(tr("New launcher"));
    control_button_save_    = new QPushButton(tr("Save launcher"));
    control_button_export_  = new QPushButton(tr("Export shell files"));

    control_button_layout->addWidget(control_button_cancel_);
    control_button_layout->addStretch();
    control_button_layout->addWidget(control_button_load_);
    control_button_layout->addWidget(control_button_new_);
    control_button_layout->addWidget(control_button_save_);
    control_button_layout->addStretch();
    control_button_layout->addWidget(control_button_export_);

    // Tabs.

    QHBoxLayout* tabs_layout = new QHBoxLayout;
    tabs_layout->addWidget(tabs_);

    // Overall layout.

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(tabs_layout);
    mainLayout->addLayout(control_button_layout);
    setLayout(mainLayout);

    // Window title.

    setWindowTitle(title);

    //
    // Connect signals and slots.
    //

    // Close the app.

    connect(control_button_cancel_, SIGNAL (clicked()),
            this,                   SLOT   (close()));

    // Load launcher file.

    connect(control_button_load_,   SIGNAL (clicked()),
            this,                   SLOT   (on_load_launcher_file()));

    // Create a launcher file.

    connect(control_button_new_,    SIGNAL (clicked()),
            this,                   SLOT   (on_new_launcher_file()));

    // Save toolkit file.

    connect(control_button_save_,   SIGNAL (clicked()),
            this,                   SLOT   (on_save_launcher_file()));

    // New launcher data.

    connect (&launcher_data_,       SIGNAL (data_is_available()),
             this,                  SLOT   (new_launcher_file()));

    // Export launcher

    connect(control_button_export_, SIGNAL (clicked()),
            this,                   SLOT   (on_export_launcher_file()));

    // Window icon

    setWindowIcon(QIcon(":/resources/WorkflowLauncher_128.png"));

    // Set window flags to include all except the close button

    setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

    // Enable drag-and-drop.

    setAcceptDrops(true);

  }
}

WorkflowLauncher_widget::
~WorkflowLauncher_widget
(void)
{
  {
    // Intentionally left blank.
  }
}

bool
WorkflowLauncher_widget::
write_option_files
(map<WLTaskKey, set<WLFile*>>&   fsets,
 map<WLTaskKey, set<WLParameter*>>& psets,
 vector<string>                     errors)
{
  bool result;

  {

    map<WLTaskKey, set<WLFile*>>::iterator   ffinder;
    map<int,int>::iterator                      nid_finder;
    map<WLTaskKey, set<WLParameter*>>::iterator pfinder;
    map<int,int>                                task_seqid_to_nid;
    set<WLTaskKey>                              visited;

    result = true;

    errors.clear();
    visited.clear();

    //
    // Build a map that will let us find the numerical identifier of a workflow
    // task using its sequential identifier.
    //

    task_seqid_to_nid.clear();

    for (size_t i = 0; i < workflow_.tasks.size(); i++)
    {
      WFNode task  = workflow_.tasks[i];
      int    nid   = task.numerical_id;
      int    seqid = task.sequential_id;

      task_seqid_to_nid.insert({seqid, nid});
    }

    //
    // First, iterate over the tasks that have files and try to locate
    // their parameters (there may be none!!!)
    //

    for (ffinder = fsets.begin(); ffinder != fsets.end(); ffinder++)
    {
      WLTaskKey key;
      ofstream  op_file;
      string    op_filename;
      string    sseqid;
      int       task_nid;

      // Get the key for the current file set.

      key = ffinder->first;

      // Add it to the set of already visited keys.

      visited.insert(key);

      // Get the task's numerical identifier using its sequential identifier.

      nid_finder = task_seqid_to_nid.find(key.task_seqid);
      task_nid   = nid_finder->second;

      // Try to find the parameters for this key (task).

      pfinder = psets.find(key);

      // Build the name of the output options file.

      sseqid = QString::number(key.task_seqid).toStdString();

      op_filename = work_dir_path_ + key.task_id + "_" + sseqid + ".op";

      // Open the output file.

      op_file.open(op_filename);

      if (!op_file.is_open())
      {
        result = false;
        string message = "Error opening the options file '" + op_filename + "' for writing.";
        errors.push_back(message);
      }
      else
      {
        //
        // Build a unique map with labels and values. In this way, we'll avoid
        // repeated connections!
        //

        set<WLFile*>           files;
        set<WLFile*>::iterator fit;
        map<string, string>       labels_and_values;

        files = ffinder->second;

        for (fit = files.begin(); fit != files.end(); fit++)
        {
          WLFile* the_file = *fit;
          string     label;

          // Get the options file label from the appropriate endpoint.

          if (the_file->start_node_type == TaskEndPoint)
          {
            // The start endpoint is a task.

            if (the_file->start_node_nid == task_nid)
            {
              //
              // The task is the one we're dealing with. This means that
              // from this tasks' standpoint, the file is one of its
              // outputs. Therefore, we take the label from the start endpoint.
              //

              label = the_file->start_node_opf_label;
            }
            else
            {
              //
              // This is the opposite situation. If we are involved in this
              // connection (file) but we are at the END of the connection,
              // so it means that this is an input file. We take the label from
              // the ending endpoint.
              //

              label = the_file->end_node_opf_label;
            }
          }
          else
          {
            //
            // If the starting endpoint is not a task, it means that it is
            // a repository. Therefore, it means that the task we're working
            // with at this moment is at the ending endpoint. Again, this
            // means that this file is an input for this task, and we'll
            // take the label from the ending endpoint.
            //

            label = the_file->end_node_opf_label;
          }

          labels_and_values.insert({label, the_file->filename});
        }

        // Write the whole set of (purged, unique) files (label = value).

        map<string, string>::iterator flit;

        for (flit = labels_and_values.begin(); flit != labels_and_values.end(); flit++)
        {
          string filename;
          string label;

          label    = flit->first;
          filename = flit->second;

          op_file << label << " = " << filename << endl;
        }

        //
        // Now, we'll check if there are also parameters for this task. If so,
        // we'll write it too to the options file.
        //

        if (pfinder != psets.end())
        {
          set<WLParameter*>           params;
          set<WLParameter*>::iterator pit;

          params = pfinder->second;

          for (pit = params.begin(); pit != params.end(); pit++)
          {
            WLParameter* the_param = *pit;
            op_file << the_param->id << " = " << the_param->value << endl;
          }
        }

        // Close the options file.

        op_file.close();
      }
    }

    //
    // Now, iterate over the map for the parameters. If any of the sets
    // there has not been processed yet (their keys are not present in
    // the visited set) then we must create another options file that
    // will include only parameters and no files. This is a weird situation,
    // but we must take care of it.
    //

    for (pfinder = psets.begin(); pfinder != psets.end(); pfinder++)
    {
      WLTaskKey                key;
      set<WLTaskKey>::iterator vfinder;

      // Get the key for the current parameter set.

      key = pfinder->first;

      // Check if this key has already been processed.

      vfinder = visited.find(key);

      //
      // If and only if the key is not found we must write these
      // parameters to an options file.
      //

      if (vfinder == visited.end())
      {
        ofstream op_file;
        string   op_filename;
        string   sseqid;

        // Build the name of the output options file.

        sseqid = QString::number(key.task_seqid).toStdString();

        op_filename = work_dir_path_ + key.task_id + "_" + sseqid + ".op";

        // Open the output file.

        op_file.open(op_filename);
        if (!op_file.is_open())
        {
          result = false;
          string message = "Error opening the options file '" + op_filename + "' for writing.";
          errors.push_back(message);
        }
        else
        {
          // Write the whole set of parameters.

          set<WLParameter*>           params;
          set<WLParameter*>::iterator pit;

          params = pfinder->second;

          for (pit = params.begin(); pit != params.end(); pit++)
          {
            WLParameter* the_param = *pit;
            op_file << the_param->id << " = " << the_param->value << endl;
          }

          // Close the options file.

          op_file.close();
        }
      }
    }

    // That's all.

    return result;
  }
}

bool
WorkflowLauncher_widget::
write_shell_file
(vector<vector<string>>& files_to_delete)
{
  {
    bool     is_windows;
    ofstream shell_file;
    string   shell_file_name;

    // Initialize.

    is_windows = (os_for_export_ == "Windows");

    if (is_windows)
      shell_file_name = work_dir_path_ + "go.bat";
    else
      shell_file_name = work_dir_path_ + "go.sh";

    //
    // If the operating system is Windows, then change the forward
    // slashes ("/") to backwards slashes ("\") in the list of
    // files to delete.
    //

    if (is_windows)
    {
      string fslash = "/";
      string bslash = "\\";
      for (size_t i = 0; i < files_to_delete.size(); i++)
      {
        for (size_t j = 0; j < files_to_delete[i].size(); j++)
        {
          replace_all_occurrences(files_to_delete[i][j], fslash, bslash);
        }
      }
    }

    //
    // Build a map that will let us go from a task string id
    // to the position of the same task in the toolkit.
    //

    map<string, int> task_id_to_pos;

    for (size_t i = 0; i < toolkit_.tasks.size(); i++)

    {
      task_id_to_pos.insert({toolkit_.tasks[i].id, (int)i});
    }

    // Open the output shell file.

    shell_file.open(shell_file_name);
    if (!shell_file.is_open()) return false;

    //
    // Linux shell files must be preceded by a header. Moreover, the
    // goto command does not exist, so we must write a function to
    // emulate it (jumpto).
    //
    // The function has been copied from the third answer ("Bob Copeland")
    // here:
    //
    // https://stackoverflow.com/questions/9639103/is-there-a-goto-statement-in-bash
    //

    if (!is_windows)
    {
      shell_file << "#!/bin/bash"                                                 << endl << endl;

      shell_file << "function jumpto"                                             << endl;
      shell_file << "{"                                                           << endl;
      shell_file << "  label=$1"                                                  << endl;
      shell_file << "  cmd=$(sed -n \"/$label:/{:a;n;p;ba};\" $0 | grep -v ':$')" << endl;
      shell_file << "  eval \"$cmd\""                                             << endl;
      shell_file << "  exit"                                                      << endl;
      shell_file << "}"                                                           << endl << endl;
    }
    else
    {
      // This is windows. We'll simply deactivate the ECHO.

      shell_file << "ECHO OFF" << endl << endl;
    }

    // Change the default directory.

    shell_file << "cd " << work_dir_path_ << endl;

    //
    // Execute all the tasks in the workflow. Delete the
    // files not needed anymore after each step. Check, for
    // each step, its return status, to decide whether to
    // abort or continue.
    //

    for (size_t i = 0; i < workflow_.tasks.size(); i++)
    {
      string                     seq_id;
      string                     task_id;
      string                     task_exe;
      toolkit_task               tkt;
      map<string, int>::iterator tkt_finder;
      string                     task_opt_file;
      int                        tkt_pos;
      WFNode                     wtask;

      // Get the current workflow task and its string identifier.

      wtask   = workflow_.tasks[i];
      task_id = wtask.id;

      //
      // Find the definition of the task in the toolkit list of
      // tasks. Then, get the name of the executable.
      //

      tkt_finder = task_id_to_pos.find(task_id);
      tkt_pos    = tkt_finder->second;
      tkt        = toolkit_.tasks[tkt_pos];
      task_exe   = tkt.name_of_executable;

      // Sequential id.

      seq_id = (QString::number(wtask.sequential_id)).toStdString();

      // Name of the options file.

      task_opt_file = task_id + "_" + seq_id + ".op";

      // "Run" the task.

      if (!is_windows)
      {
        shell_file << "WORKER=\"(" << seq_id << ") " << task_id << "\"" << endl;
        shell_file << "echo \"Running $WORKER with options file " << task_opt_file << "\"" << endl;
      }
      else
      {
        shell_file << "set WORKER=(" << seq_id << ") " << task_id << endl;
        shell_file << "@ECHO Running %WORKER% with options file " << task_opt_file << endl;
      }

      shell_file << task_exe << " " << task_opt_file << endl;

      // Check the task exit code.

      if (is_windows)
      {
        shell_file << "IF %ERRORLEVEL% NEQ 0 GOTO problems" << endl;
      }
      else
      {
        shell_file << "retVal=$?"                  << endl;
        shell_file << "if [ $retVal -ne 0 ]; then" << endl;
        shell_file << "  jumpto problems"          << endl;
        shell_file << "fi"                         << endl;
      }

      // Delete all the files that must "die" at this point...

      vector<string> to_delete = files_to_delete[i];

      for (size_t j = 0; j < to_delete.size(); j++)
      {
        if (is_windows) shell_file << "DEL " << to_delete[j] << endl;
        else            shell_file << "rm "  << to_delete[j] << endl;
      }
    }

    //
    // At this point we'll write the queue of the file, where a normal
    // process must leave the script right now, but we must also
    // add the "emergency" exit when problems have been detected.
    //

    if (is_windows)
    {
      shell_file << "@ECHO Workflow completed."                                        << endl;
      shell_file << "EXIT /B"                                                          << endl;
      shell_file << ":problems"                                                        << endl;
      shell_file << "@ECHO *** ERROR: %WORKER% reported the error code %ERRORLEVEL%."  << endl;
      shell_file << "@ECHO            Check the documentation of said task for more"   << endl;
      shell_file << "@ECHO            information about the reported error code." << endl;
      shell_file << "@ECHO *** Wokflow finished with errors."                          << endl;
      shell_file << "EXIT /B"                                                          << endl;
    }
    else
    {
      shell_file << "echo \"Workflow completed.\"" << endl;
      shell_file << "exit" << endl;
      shell_file << "problems:" << endl;
      shell_file << "echo \"*** ERROR: $WORKER reported the error code $retVal.\""      << endl;
      shell_file << "echo \"           Check the documentation of said task for more\"" << endl;
      shell_file << "echo \"           information bout the reported error code.\""     << endl;
      shell_file << "echo \"*** Workflow finished with errors.\""                       << endl;
      shell_file << "exit"                                                              << endl;
    }

    // Close the shell file.

    shell_file.close();

    //
    // The following code will be compiled only on LINUX.
    //
    // This is so because it only applies when this application
    // is running on said operating system.
    //
    // The idea is to set the execution flag of the output
    // file (the shell script), so users do not have to worry
    // about changing its properties.
    //
    // Since said "execution flag" does not exist on Windows,
    // then the code is valid only for Linux platforms.
    //
    // Moreover, in the case that although the app is being
    // executed on a Linux box, the script is generated for
    // Windows, then the code below will simply do nothing.
    //
    // In short: if we are on Linux, generating a script for
    // Linux, the execution flag will be set. In any
    // other case, we'll do nothing.
    //

    #ifdef Q_OS_LINUX

      // Work only when the output script is written for Linux.

      if (!is_windows)
      {
        // Check if the file exists

        QFile scriptFile(QString::fromStdString(shell_file_name));

        if (scriptFile.exists())
        {
          // Set the permissions to read, write, and execute for the owner, group, and others.

          scriptFile.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner |
                                    QFile::ReadGroup | QFile::WriteGroup | QFile::ExeGroup |
                                    QFile::ReadOther | QFile::WriteOther | QFile::ExeOther);
        }
      }

    #endif

    // That's all.

    return true;
  }
}
