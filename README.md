# WorkFlowMaker

## What is it for?

WorkFlowMaker allows you to design and execute workflows created from the combination of so-called console applications.

A "console application" is one that does not have a graphical interface and runs in a command window. Typically, these applications interact with the user through parameters passed on the command line, the keyboard (for entering data), and data files (input/output).

It is common for applications of this type to be generated in research and even production environments, since they are not affected by the complexity of developing a graphical user interface.

On certain occasions, it will be necessary to combine several of these applications to achieve more elaborate results. Normally this is done through a manual process. The user executes, one by one, the required applications until obtaining the final result.

This can be a tedious and error-prone process. In cases where it is necessary to combine a specific set of console applications, it would be interesting to have scripts that automate this process. However, not everyone is capable of writing such scripts, so they will depend on other people who are capable of doing so.

WorkFlowMaker tries to solve this problem by allowing users without programming knowledge to combine these console applications using a graphical tool that allows them to add/remove applications to a workflow and connect the inputs and outputs of these applications using connections. With such a tool, a user without programming knowledge but with knowledge of the domain in which they are working will be able to generate scripts that execute their workflows without the need to write a line of code.

To achieve this, WorkFlowMaker imposes a few restrictions that explain how console applications should be programmed. From these restrictions, it is possible to formalize the definition of these applications and, from here, draw these workflows and generate scripts to execute them. Obviously, developers who want to be able to integrate their console applications with WorkFlowMaker will have to respect these restrictions.


## Are there any installers?

Yes.

In the Releases section you can download the WorkFlowMaker installer for Windows, the .deb package for Ubuntu based distributions. In addition, there are two more installers (exclusively for Windows) that include two sets of test applications (image processing and text file processing). By installing these two test sets, it is possible to start practicing with WorkFlowMaker without having to go through the process of formalizing your own tools.

Furthermore, the test applications include the source code, so developers may see how WorkFlowMaker-compliant applications must be implemented.

## Are there any user guides?

The user guides for the WorkFlowMaker suite may be found at the following path of the repository:

/docs/user guide/*.pdf


The information related to the two test sets is also available in the repository;

* Image processing: /samples/image_processing/docs/.pdf
* Text file processing": /samples/text_file_processing/docs/.pdf

## How may I build from source code?

Read the document /docs/user guide/Building WorkFlowMaker.pdf

Note that building from source may be a painful process, since no automation is provided yet. Libraries and tools must be built one by one. Take special care reading the section about post build files to understand how these work and how to adapt it to your needs.

## Features

* Create workflows by combining, visually, console applications 
* Connect the inputs and outputs of applications 
* Generate scripts to execute workflows 
* No programming knowledge required 
* Easy to use graphical interface 
* Available for Windows and Ubuntu-based distributions

## Benefits

* Reduce errors 
* Save time
* Users _with no programming knowledge_ may create their own workflows.
* Workflows are self-documented.

## Use cases

* Research 
* Production 
* Education 
* Training 
* Personal use

## Get started

1. Download the WorkFlowMaker installer / deb package from the Releases section. 
2. Install WorkFlowMaker on your computer. 
3. Define your tasks (console applications) using ToolKitEditor. This is, clearly, a task for the developer(s) of the said applications. For *Windows* platforms, there are two installers for sample toolkits including (1) the definition of the toolkit and a single workflow plus (2) the binaries of the tools in the toolkit, so these are ready to use and (3) guides to understand the toolkit. See the Releases section to download these. For *Linux* platforms these sample toolkits are also available, but the tools are not prebuilt.
4. Create new workflows running WorkflowEditor. Add tasks and connect their input and outputs by means of connections. This may be done by anyone with enough knowledge about _what the applications do_; no programming skills are thus required.
5. Run WorkflowLauncher to assign data values to input and output file names and keyboard parameters to the workflow of your choice. Generate the script. Again, no programming skills are needed.
6. Run your workflow by executing the script.

Note that step 3 above require the adaptation of the console applications so these comply with the requisites set by WorkFlowMaker.



## The licence

The MIT License

SPDX short identifier: MIT

Copyright 2024 Centre Tecnològic de Telecomunicacions de Catalunya-CERCA

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
