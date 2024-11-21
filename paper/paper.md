---
title: "WorkflowMaker, a visual workflow software tool for the easy integration of self-developed applications"
authors:
  - name: Jose A. Navarro
    orcid: 0000-0000-0000-0000
  - affiliation: '1'
affiliations:
  - index: 1
    name: Centre Tecnologic de Telecomunicacions de Catalunya-CERCA, Av. Carl Friedrich Gauss 7, Castelldefels, 08860, Spain
date: %e %B %Y
bibliography: paper.bib
---

# Summnary

WorkflowMaker is a set of tools aimed at the *visual design* and subsequent *execution* of
workflows that integrate *console applications* developed by users themselves, whether in
research or production environments involving in-house software development.

In this context, *visual design* means *drawing* the workflow by including tasks (the
applications) and connecting their inputs and outputs, thereby creating a workflow that
specifies who does what (the console applications) and how information flows between them
(the data connections). *Execution* means automatically generating the necessary scripts
(for Windows or Linux) to process the data according to the designed workflow.

The goal of WorkflowMaker is to reduce the complexity of combining different console
applications to achieve a more refined result. It seeks to eliminate the need for manual
execution of these applications or the writing of automation scripts, which require skills
that not everyone possesses. This approach makes it possible for these applications to be used
not only by their developers but also by any expert in the application field, even if they
lack programming skills.

# Statement of need

It is not surprising to find research / production groups that create their own software.
For this to be possible, it is necessary their members to have, in addition to
the skills specific to their domain, a certain level of knowledge in the field of computer
programming and software engineering. This, according to the author's experience, is no
so common, being the amount of people capable of generating applications relatively small.

Usually, programming-skilled developers create a repertoire of tools solving different problems 
related to their field of knowledge. By combining these tools in various ways, it is then 
possible to tackle more complicated problems. In the context of this work, the combination of 
some of these tools to achieve more complex goals is called a *workflow*. However, implementing
these workflows can be a difficult task and prone to human errors if the appropriate tools are 
not used. Usually, workflows are created by writing some kind of script combining the desired
set of tasks. This, which is relatively easy for experts with programming skills, can be a 
stumbling block for those without, who need to rely on their more qualified colleagues to do it. 
This limits the flexibility of the research group, creating dependencies&mdash;and 
bottlenecks&mdash;among its members. 

According to the author's experience, a typical tool built by researchers-developers is the 
so-called "console" (command line) application. Console applications accept some input 
parameters through the keyboard and process some input files to create some others as outputs. 
Being so common, the use of a *visual design tool* able to combine console applications to 
create workflows would be very useful for those experts without programming knowledge. So now, 
more specifically, and in the context of this work, a workflow is a combination of console 
applications connected by some edges representing the flow of data between these&mdash;that is, 
how the outputs of some tasks are used as inputs by some others.

This visual designer should aim for simplicity for two reasons: firstly, users (either having 
programming skills or not) should be able to use it without having to overcome a costly learning 
curve; secondly, developers building apps that support this designer should not have to adhere 
to a complex set of restrictions diverting their attention from the main goal: their own 
research.

However, and although it is very difficult to prove the absolute non-existence of something, the 
author has not been able to find a tool meeting these requirements even though there is a wide 
range of applications for visual programming, workflow management, business process modeling or 
no-code/low-code development platforms, which are the different incarnations or labels given to 
visual tools.

In @ref-journal-kuhail an exhaustive review of articles related to visual programming was 
carried out. The conclusions of this study on target fields of application are: 23.3 % of the 
tools are focused on the Internet Of Things (IOT) [@ref-inproc-valtolina; @ref-journal-danado; @ref-journal-francese; @ref-journal-johnsson]; another 23.3 % is oriented 
towards education [@ref-proceeding-kunimune; @ref-proceeding-abe; @ref-journal-broll; @ref-journal-deluca; @ref-journal-feng; @ref-proceeding-rao];
the remainder are spread across the fields of Business Process Management (BPM)
@ref-journal-weber, museum exhibits @ref-inproc-stratton, reporting @ref-journal-chen, records 
management @ref-proceeding-cabitzaiade, geographic information @ref-proceeding-luong, robotics 
@ref-proceeding-alexandrova, mechanical engineering @ref-inproc-sorce, information visualization 
@ref-journal-mei, and psychiatry @ref-proceeding-rough. 

Despite the exhaustiveness of this study, there are no tools aimed at building workflows made up 
of console applications. A less academic search for tools, whether open-source or proprietary, 
produced a long list. Some of the tools that are considered the best ones at the time of writing 
this article [@ref-web-pathak; @ref-web-boitnott; @ref-web-towns] are listed 
here in alphabetical order&mdash;so no assumptions about their quality or appropriateness are made: 
Airtable @ref-web-airtable, Appy Pie @ref-web-appypie, Betty Blocks @ref-web-bettyblocks, Bubble 
@ref-web-bubble, Carrd @ref-web-carrd, Caspio @ref-web-caspio, Jotform @ref-web-jotform, Knack 
@ref-web-knack, m-Power @ref-web-mpower, Mailchimp @ref-web-mailchimp, Mendix @ref-web-mendix, 
Microsoft Power Apps @ref-web-powerapps, Notion @ref-web-notion, OutSystems @ref-web-outsystems, 
Quickbase @ref-web-quickbase, Retool @ref-web-retool, Shopify @ref-web-shopify, SNAP
@ref-web-esa, Visual LANSA @ref-web-vlansa, Webflow @ref-web-webflow, Zapier @ref-web-zapier and 
Zoho Creator @ref-web-zohocreator.

Most of the tools above are targeted at BPM; they let users build workflows visually, connecting 
data and processes. Usually, they offer standard data stores (such as commercial or open-source 
databases or spreadsheets) and applications available in the market to be used as the nodes in 
the workflow. The targeted users are chief executive officers, administrative or salesman 
executives. Other applications are aimed at marketing / email distribution or website / e-shop 
builders. 

For a very extensive list of visual programming / workflow / no-code / low-code tools please 
refer to @ref-web-g2bsr.

Other tools were found that were not present in the usual comparison reports. The most relevant 
examples are Node-RED @ref-web-nodered, Apache NiFi @ref-web-apachenifi, and WexFlow 
[@ref-web-wexflow1; @ref-web-wexflow2]. The problems with these tools are the limitations in 
terms of the programming language to use, the restrictions that user applications must comply 
with, and the steep learning curves to overcome. Moreover, neither of them works with console applications.

In fact, the author is only aware of a single tool covering the targets defined above: it is the 
Visual Factory Suite (VFS) @ref-proceeding-navarro2000, a software suite developed by the author 
of this work and aimed precisely at the definition of workflows integrating console 
applications.  Even though it worked satisfactorily, the VFS was very complex from several 
standpoints: the number of applications to maintain was high and the technologies involved
(distributed execution, communications using TCP/IP sockets, access to databases) were 
complicated for the small group of developers involved in the project; developers had to comply 
with a rather high number of restrictions to make their applications compatible with the suite. 
Moreover, the VFS worked only on Windows-based computers, and it was a proprietary tool&mdash;
not available any longer&mdash;so its use was out of consideration.

The work presented in the next sections is, therefore, the outcome of combining the need to ease 
the design and execution of workflows, the lack of tools fulfilling the requirements here stated 
and the lessons learned from the implementation of the VFS.

# Description

# References
