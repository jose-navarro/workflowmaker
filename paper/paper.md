---
title: 'WorkflowMaker, a visual workflow software tool for the easy integration of self-developed applications'
tags:
  - Cross-platform application
  - Visual tools
  - Visual Programming
authors:
  - name: José A. Navarro
    orcid: 0000-0001-7877-1516
    affiliation: "1" 
affiliations:
 - name: Centre Tecnològic de Telecomunicacions de Catalunya-CERCA, Av. Carl Friedrich Gauss 7, Castelldefels, 08860, Spain
   index: 1
date: 20 December 2024
bibliography: paper.bib
---

# Summary

WorkflowMaker is a set of tools aimed at the *visual design* and subsequent *execution* of
workflows that integrate *console applications* developed by users themselves, whether in
research or production environments involving in-house software development.

In this context, *visual design* means *drawing* the workflow by including tasks (the
applications) and connecting their inputs and outputs, thereby creating a *template* that
specifies who does what (applications) and how information flows between them
(data connections). *Execution* means automatically generating the necessary scripts
(for Windows or Linux) to process the data according to the designed workflow.

The goal of WorkflowMaker is to reduce the complexity of combining different console
applications to create said workflows. It seeks to eliminate the need for manual
execution of these applications or the writing of automation scripts, which require skills
that not everyone possesses. This approach makes it possible for these applications to be used
not only by their developers but also by any expert in the application field, even if they
lack programming skills.

# Statement of need

It is not surprising to find research or production groups that create their own software.
Often, however, despite being experts in their field of knowledge, only some members of
these groups have programming skills.

Developers in these groups are able to create console applications to solve specific problems
with relative ease, as these do not include graphical interfaces. By combining them in
different ways, it is possible to achieve more complex results. However, doing so either
requires manually running the applications one by one or writing scripts to automate the
process. This, which may be easy for developers, can be a stumbling block for those without
programming skills, who need to rely on their more qualified colleagues to do it.  This limits
the flexibility of the group, creating dependencie among its memberss&mdash;and bottlenecks.

The author believes it would be highly beneficial if those without programming skills
could design workflows by combining the applications created by their colleagues using a tool
that (1) would literally allow them to draw such workflows and (2) automatically generate the
necessary scripts to execute them.

However, and although it is very difficult to prove the absolute non-existence of something, the 
author has not been able to find a visual tool to draw and execute workflows made of console
applications.

@ref-journal-kuhail carried out an exhaustive review of articles related to visual programming.
The conclusions of this study on target fields of application are: 23.3 % of the tools are
focused on the Internet Of Things (IOT) 
[@ref-inproc-valtolina; @ref-journal-danado; @ref-journal-francese; @ref-journal-johnsson];
another 23.3 % is oriented towards education
[@ref-proceeding-kunimune; @ref-proceeding-abe; @ref-journal-broll; @ref-journal-deluca; @ref-journal-feng; @ref-proceeding-rao];
the remainder are spread across the fields of Business Process Management (BPM)
[@ref-journal-weber], museum exhibits [@ref-inproc-stratton], reporting [@ref-journal-chen],
records management [@ref-proceeding-cabitzaiade], geographic information [@ref-proceeding-luong],
robotics [@ref-proceeding-alexandrova], mechanical engineering [@ref-inproc-sorce], information
visualization [@ref-journal-mei], and psychiatry [@ref-proceeding-rough]. 

A less academic search for tools, whether open-source or proprietary, 
produced a long list. The best ones, according to 
[@ref-web-pathak; @ref-web-boitnott; @ref-web-towns] are listed here in alphabetical order:
Airtable [@ref-web-airtable], Appy Pie [@ref-web-appypie], Betty Blocks [@ref-web-bettyblocks],
Bubble [@ref-web-bubble], Carrd [@ref-web-carrd], Caspio [@ref-web-caspio], Jotform
[@ref-web-jotform], Knack [@ref-web-knack], m-Power [@ref-web-mpower], Mailchimp
[@ref-web-mailchimp], Mendix [@ref-web-mendix], Microsoft Power Apps [@ref-web-powerapps],
Notion [@ref-web-notion], OutSystems [@ref-web-outsystems], Quickbase [@ref-web-quickbase],
Retool [@ref-web-retool], Shopify [@ref-web-shopify], SNAP [@ref-web-esa], Visual LANSA
[@ref-web-vlansa], Webflow [@ref-web-webflow], Zapier [@ref-web-zapier] and  Zoho Creator
[@ref-web-zohocreator]. For a very extensive list of visual programming / workflow / no-code /
low-code tools please refer to [@ref-web-g2bsr].

Most of the tools above are targeted at BPM; they let users build workflows visually, connecting 
data and processes. Usually, they offer standard data stores (such as commercial or open-source 
databases or spreadsheets) and applications available in the market to be used as the nodes in 
the workflow. Other applications are aimed at marketing / email distribution or website / e-shop 
builders. Unfortunatelly, there is not a single tool to build workflows integrating console
applications.

# Description

WorkflowMaker consists of three applications.

*ToolkitEditor* – This tool allows developers to *formally characterize* their console
applications by specifying (1) their keyboard parameters and (2) input and output files.
In this context, *keyboard parameter* stands for these values that users provide using the
keyboard when the console application is run. To be integrable with WorkflowMaker applications
must comply with certain constraints:

  * They must accept a single command-line parameter: the name of a plain text file
    containing the values of the keyboard parameters as well as the names of the input /
    output files. The format of this file is standardized.

  * They must return a value indicating whether the execution completed successfully (zero)
    or not (non-zero) so that the scripts automatically generated by WorkflowLauncher (see
    below) can manage the execution flow in case of errors.

![ToolkitEditor: the list of tasks already defined.\label{fig_toolkit1}](figure_toolkit1.png){height="300pt"}

![ToolkitEditor: the OILPAINT task in detail.\label{fig_toolkit2}](figure_toolkit2.png){height="300pt"}

*WorkflowEditor* – This is a graphical editor that allows users to design workflows by combining
the tasks defined with ToolkitEditor. It relies on three pillars: (1) repositories, which are
locations where input and output files reside, (2) tasks, the formally defined applications, and
(3) connections, which indicate how data flows between repositories and tasks.

![WorkflowEditor: visually designing a workflow.\label{fig_wfeditor}](figure_wfeditor.png){height="300pt"}

*WorkflowLauncher* – The workflows created with WorkflowEditor are simply templates that
outline how to perform some process, but do not set the values of the datato be
processed&mdash;so they can be reused for may data sets. WorkflowLauncher is used to specify the
names of the input files for those workflows, as well as the values of the required keyboard
parameters. From this, an executable script (Windows or Linux) is generated.

![WorkflowLauncher: Setting the values of keyboard parameters.\label{fig_wflauncher1}](figure_wflauncher1.png){height="300pt"}

![WorkflowLauncher: Generating the scripts.\label{fig_wflauncher2}](figure_wflauncher2.png){height="70pt"}

# A very simple example

Figure \ref{fig_workflow}(a) show a very simple image processing workflow drawn with WorkflowEditor; there, two images stored in some repository (REPO_1) are rotated (tasks (1) ROTATE and (2) ROTATE). Then, their outputs are used by a third task ((3) MOSAIC) to mosaic these images producing a new one, which is used by the last task ((4) OILPAINT) to apply an
artistic image effect known as "oil painting". The result of this process is stored in
a new repository, (REPO_2). In Figure \ref{fig_workflow}(b) the two input images used to
run this workflow are shown; \ref{fig_workflow}(c) depicts the result.

The workflow in the example has been created using the *sample image processing toolkit*
available in WorkflowMaker's GitHub repository.

![A simple image processing workflow (a), the two input images (b), and the result (c).\label{fig_workflow}](figure_workflow.png)

# References
