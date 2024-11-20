# WorkFlowMaker

<p align="center">
  <img src="readme_images/cover.png" alt="WorkflowMaker" width="600">
</p>

## What is it for?

WorkFlowMaker allows you to design and execute workflows created from the
combination of so-called console applications.

A *console application* is one that does not have a Graphical User Interface (GUI)
and runs in a command window. Typically, these applications interact with the user
through parameters passed on the command line, the keyboard (for entering data),
and data files (input/output).

It is common for applications of this type to be generated in research and even
production environments, since they are not affected by the complexity of
developing a graphical user interface.

On certain occasions, it will be necessary to combine several of these
applications to achieve more elaborate results. Normally this is done through a
manual process. The user executes, one by one, the required applications until
obtaining the final result.

This can be a tedious and error-prone process. In cases where it is necessary to
combine a specific set of console applications, it would be interesting to have
scripts that automate this process. However, not everyone is capable of writing
such scripts, so they will depend on other people who are capable of doing so.

WorkFlowMaker tries to solve this problem by allowing users *without programming
knowledge* to combine these console applications using a graphical tool that
allows them to add/remove applications to a workflow and connect the inputs and
outputs of these applications using connections. With such a tool, a user without
programming knowledge but with knowledge of the domain in which they are working
will be able to generate scripts that execute their workflows without the need to
write a line of code.

To achieve this, WorkFlowMaker imposes a few restrictions that explain how console
applications should be programmed. From these restrictions, it is possible to
formalize the definition of these applications and, from here, draw these
workflows and generate scripts to execute them. Obviously, developers who want
to be able to integrate their console applications with WorkFlowMaker will have to
respect these restrictions.

## I am a developer. Can any console application be integrated in a WorkflowMaker workflow?

The short answer is **no**. There are many console applications that cannot be
integrated, as they do not meet a series of restrictions that WorkflowMaker
imposes for such integration to be carried out.

Nevertheless, for developers interested in WorkflowMaker, it is very simple to
create new applications that can be integrated, as the restrictions to be met are
very few and easy to comply with. In fact, it is so simple that it is possible to
adapt non-compatible applications to be integrated with WorkflowMaker, as long as
the source code is available, of course.

### What are the restrictions that a console application must meet to be integrable with WorkflowMaker? ###

In short, WorkflowMaker-compatible console applications must comply with the
following restrictions:

* They must be console applications, that is, having no GUI, implemented as
  executable files.
* All their inputs and outputs must be files.
* They must accept a single command line parameter, the name of an options file
  stating what are the values of the typical parameters that would be input using the keyboard as well as the names of the input and output files.
* They must return a status code stating whether they ended successfully (return
  code 0) or not (any other value).

The options file must be a plain text file consisting of lines of labels and
values pairs, such as

<p align="center">
MAX_NUMBER_OF_ITERATIONS = 5
</p>

<p align="center">
INPUT_FILE = /home/some_user/my_data/the_input_file.dat
</p>

Thanks to this restrictions the interface of the applications is *standardized*,
which makes possible that a generic workflow design tool as WorkflowMaker capable
of integrating any type of console application.

### Then, may I use any programming language to create these compatible console applications? ###

Yes, providing that:

* it produces executable files (not files requiring an interpreter),
* that it is possible to read the command line to retrieve the name of the options
  file and,
* that a status code may be returned.

A **non-exhaustive** list of these languages could be:

<p style="text-align: center;">
Ada / C / C# / D / Haskell / Fortran / Go / Pascal / Rust / Swift /
Visual Basic.NET.
</p>

Interpreted languages, such as java or phyton are not **yet** supported. It is
planned that future releases of WorkflowMaker will be able to handle applications developed with these kind of programming languages.

## I am interested but I am **not** a developer ##

If you are working in a workplace where someone else develops applications that
may be made compatible with WorkflowMaker, then you are also a potential user of
this (set of) tools.

WorkflowMaker consists of three tools:

1. **ToolkitEditor**. This is the application that **developers** will use to
   formally define what their console applications do. Non-developers need not to
   know about ToolkitEditor.

2. **WorkflowEditor**. For **all users**. This is the keystone on which
   WorkflowMaker relies. It is a graphical editor where the applications previously
   defined with ToolkitEditor may be added and combined with others. To *draw*
   these *workflows* users only need to know about the *goal* of each task in the
   design, *not* about how these have been implemented. That is, if you are an
   expert in, let's say, geodesy, but have no programming skills, you will be able
   to create workflows dealing with geodesy just by drawing your
   processes made of several tasks (console applications) and connecting their
   inputs and outputs.

3. **WorkflowLauncher**. For **all users**. The workflows designed with
   WorkflowEditor above are just *templates*. With WorkflowLauncher any user may
   use these templates to process their own datasets. 

In short, if you are not a developer you just need the domain knowledge for which
some developer(s) created a set of applications that have been made compatible
with WorkflowMaker.

## It seems nice, but I usually work on Linux ##

If your Linux box is an modern Ubuntu-based computer (running, for instance,
Ubuntu or Linux Mint) you may use WorkflowMaker too. You do not need to use a
Windows computer (but you can too if you want!).

## Are there any installers?

**Yes**.

In the [Releases](https://github.com/jose-navarro/workflowmaker/releases) section you can download the WorkFlowMaker installer for Windows, the .deb package for Ubuntu based distributions.

WorkflowMaker works on Windows 10 & 11 computers. Regarding Linux, Linux distributions based on Ubuntu 24.10 (such as Linux Mint 22 Wilma) are the only ones supported - due to limited resources availability.

There are also installers (Windows, Ubuntu-based) for a pre-built *example image processing toolkit* that may be used to start practising without having to create a toolkit on your own.

## Where is the documentation?

The user guides for the *WorkFlowMaker suite* may be found at the following path of the repository:

/docs/user guide/*.pdf

The available documents are:

* *WorkflowMaker user guide* (file: WorkflowMaker user guide.pdf). **For all users.**
  This is the guide to understand WorkflowMaker. It explains how to create WorkflowMaker-compatible
  console applications, how to define these and group them in so-called toolkits using ToolkitEditor,
  how to define workflows visually that rely on these toolkits and how to, finally, use them to
  process your own data thanks to WorkflowLauncher.

* *Building WorkflowMaker* (file:Building WorkFlowMaker.pdf ). **For developers only:**
  this guide is targeted at describing how to build WorkflowMaker from source code, both for Windows
  and Linux platforms. It describes the tools required, the steps to build automatically the whole
  suite of applications and, also, how to build single applications when required.

* *Building a WorkflowMaker-compliant console application* (file: Building a compliant console application.pdf).
   **For developers only:** in this case, a step by step example is provided, showing how to build
  a WorkflowMaker-compatible console application. The example is written in C++, but the ideas there
  explained are valid for any programming language. The example application is one of the tasks included
  in the sample image processing toolkit.

With regard to the *sample image processing toolkit*, there are two folder to check:

* /samples/image_processing/docs/.pdf

  * *The image processing sample toolkit* (file: The image processing sample toolkit.pdf).
    **For all users, but mainly for developers**. 
    This document describes a very simple set of image processing console applications that together
    make a so-called toolkit. The description is made from the WorkflowMaker standpoint, explaining
    the steps that must be taken to build said toolkit. Therefore, it is highly advisable to read
    the "WorkflowMaker user guide" first.
    In the case of **regular users**, this guide explains what the different tasks in the toolkit do,
    so it is a good starting point to understand how to combine these in useful workflows.
    **Developers** should also read this guide since it explains, step by step, how to define an operational
    toolkit.

  * *Building the samples* (file: Building the samples.pdf). **For developers only.**
    If you are interested in building the sample toolkit, this guide explains how.

* /samples/libraries/simple_options_file_parser/docs. **For developers only.** There is a single document in this
    folder, *Simple options file format description* (file: Simple options file
    format description.pdf). All the tasks in the sample image processing toolkit
    rely on a basic library that simplifies the task of parsing label / value
    options file. Therefore, to help understanding the tasks in said sample
    toolkit, this document is included, which describes the format supported by
    this library.

## Are there any documents describing the API? ##

Yes. But these are not included in the source code repository. The API HTML documentation is
generated whenever WorkflowMaker or the sample image processing toolkit are built.
The documents explaining the process and where to find the API documents are:

* *Building WorkflowMaker* and
* *Building the samples*.

The previous section describe where to find these documents.

## How may I build from source code?

Read the guides related to building from source code. Again, the documents
to read are:

* *Building WorkflowMaker* and
* *Building the samples*.

## Tell me how to get started

The following subsections will tell you how to proceed to start using
WorkflowMaker.

### Do this no matter what you will do later ###

1. Download the WorkFlowMaker installer / deb package from the Releases section. 

2. Install WorkFlowMaker on your computer (for Windows, the installer is a
   classic next->next process; for Linux, the deb package may be installed using
   GDebi (double clicking on the deb file) or using the console).

3. Take a look at the WorkflowMaker user guide to learn where to find the
   relevant information about the tools. All user guides are part of the
   installation kit (either included as a subfolder in the Linux installer
   or as a specific folder created by the installer on Windows, which may
   be accessed via the Start menu).

### I want a quick start to learn how to use WorkflowMaker ###

If you want to start testing how to draw workflows and execute these on your own
computer, proceed as follows:

1. Download the sample image toolkit, either the installer for Windows or the
   deb package for Linux.

2. Install the toolkit using the appropriate method for Windows or Linux.

3. Copy the WorkflowMaker sample files listed below to some writable folder on
   your computer. On Windows, these are available via the Start menu; look
   for the WorkflowMaker menu entry and then for a sub-entry pointing to the
   samples folder. On Linux, these files are packaged in the samples and docs
   subfolder of the WorkflowMaker installer. The files to copy are:
   
   * *image_processing_toolkit.xml*. This file includes the formal definition
      of all the tasks included in the example image processing toolkit. Such
      definition was made using the ToolkitEditor tool. Note that this
      file **must not be modified**; otherwise, the workflow below will
      become unusable.

   * *image_processing_workflow.xml*. This is an example workflow, showing
      how the different tools included in the toolkit may be combined to
      produce a more complex result. To keep the integrity of the sample, this
      file should not be modified; if changes are made, these should not be
      saved. Otherwise, the launcher below will stop working.

   * *image_processing_launcher.xml*. As stated above, workflows are just
      templates defining how to process some date. With a launcher, it is
      possible to give values to parameters, names to files and paths to
      the folders where these reside. This launcher corresponds to the
      workflow and toolkit above; it includes values for all the said items.
      If you want to use it, make sure that you change the paths where files
      are stored (that is, the paths to *repositories* for both input and
      output files) and that the input files stated by the laucher actually reside
      in the folders indicated. In other words, this file must be modified
      using WorkflowLauncher to make the appropriate (folder paths) changes.

4. Use WorkflowEditor to play with image_processing_workflow.xml; then, switch
   to WorkflowLauncher to provide the appropriate values for the paths where
   your input and output folders stay and create the script to run the
   workflow. Open a command line and run said script.

Of course, it is possible to design other workflows than the one included
with the sample; and then, it is possible to create the corresponding launcher
to provide data values and execute the workflow. The only thing that
should never be done when playing with this sample toolkit is modifying
the toolkit file itself (image_processing_toolkit.xml) since this would
corrupt the formal definition of the tasks it contains.

Note that to fully understand the directions above, reading the WorkflowMaker
user guide before proceeding is rather convenient.

### I want to develop my own toolkit ###

Developing a self-made WorkflowMaker toolkit implies a sound knowledge of the
concepts on which this software suite relies. Therefore, it is recommended to
read the WorkflowMaker user guide as well as those explaining how the
sample image toolkit is implemented. The documents to read are:

* WorkflowMaker user guide

* Building a WorkflowMaker-compliant console application

* The image processing sample toolkit

All these documents are included with the installers. If you do not plan to
install WorkflowMaker yet, the section above describing the available
documentation explains where to find these documents in the repository tree.

Once that your toolkit is implemented (including its formal definition using
ToolkitEditor) proceed as stated in the previous section; use your own toolkit
instead of the sample one.

## WorkflowMaker at a glance ##

### Target ###

WorkflowMaker is a set of visual tools whose gool is making possible the
visual design of workflows integrating self-developed console applications as
well as the generation of the necessary scripts to run these workflows.

Thanks to WorkflowMaker, there is o need to write a single line of code to
design and execute your own workflows.

### Features ###

* Design of workflows by visually combining tasks implemented as 
  console applications.
* Visual connection of the inputs and outputs of said tasks. 
* Automatic generation of scripts to execute the workflows thus designed.
* No programming knowledge required.
* Easy to use graphical interface.

### Benefits ###

* Reduce errors when manually combining tasks, either manually or by means
  of self-written scripts.
* Save time.
* Users _with no programming knowledge_ may create their own workflows.
* Workflows are self-documented.

### Use cases ###

* Research.
* Production. 
* Education.
* Training.
* Any activity where self-developed console applications are implemented.

### Availability ###

WorkflowMaker and the sample image processing toolkit are available for Windows
10 / 11 as well as Ubuntu 24.10-based Linux distributions. There are installers
available for these platforms.

## Image gallery ##

<p align="center">
  <img src="readme_images/ToolkitEditor.png" alt="ToolkitEditor" width="700">
</p>
<p align="center"><em>Figure 1: The ToolkitEditor - Defining a task.</em></p>

<p align="center">
  <img src="readme_images/WorkflowEditor.png" alt="WorkflowEditor" width="700">
</p>
<p align="center"><em>Figure 2: Drawing a workflow with WorkflowEditor.</em></p>

<p align="center">
  <img src="readme_images/WorkflowLauncher.png" alt="WorkflowLauncher" width="700">
</p>
<p align="center"><em>Figure 3: Providing data to launch a workflow with WorkflowLauncher.</em></p>

<p align="center">
  <img src="readme_images/cover_icons.png" alt="WorkflowMaker's icons" width="200">
</p>
<p align="center"><em>Figure 4: The icons of the three WorkflowMaker's applications.</em></p>

## The licenses ##

WorkflowMaker relies on several open source several components. Below, its own
licence as well as those of the auxiliary packages are reproduced.

### WorkflowMaker ###

*A copy of the MIT license may be found in /docs/licenses/LICENSE as well
as in the root folder of the repository.*

The MIT License

SPDX short identifier: MIT

Copyright 2024 Centre Tecnològic de Telecomunicacions de Catalunya-CERCA

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

### The rapidxml library ###

WorkflowMaker uses the rapidxml library, which is available under the Boost 
Software License. For more information, visit https://www.boost.org/LICENSE_1_0.txt. A copy of this license may be found in */doc/licenses/Boost_Software_License_1_0.txt*.

### The Qt 6 framework ###

This application uses the Qt 6 framework, which is available under the LGPL v3
license. For more information, visit https://www.qt.io/licensing and https://www.gnu.org/licenses/lgpl-3.0.html. A copy of the LGPL v3 license may be found in  */doc/licenses/LGPL_v3_License.txt*.

### The ImageMagick library ###

All the tasks included in the sample image processing toolkit rely on the libraries above. In their case they also make use of on the ImageMagick library, which is available under ImageMagick license. For more information, visit https://imagemagick.org/script/license.php. A copy of the ImageMagick license may be found in */docs/licenses/ImageMagick_license.txt*.