# TrustPort
Selected programming examples from my work at TrustPort
C++ Programming Examples

Selected C++ programming examples from my work at TrustPort.

The examples demonstrate object-oriented programming, Windows GUI development using MFC, custom controls and graph-related functionality.

Graph

The graph module contains shared functionality for 2D and 3D graphs.

CGraf

A common base class derived from CStatic, providing:

Conversion of graph values into pixel coordinates.

Automatic selection of axis scales and intervals.

Numeric axis division and formatting.

Time-axis calculations for minutes, hours, days, months and years.

Axis labels and font handling.

CGraf2

Graph drawing functionality, including different graph types and time-based data presentation.

GUI

Custom Windows GUI components developed using C++ and MFC.

ButtonMy

A custom button control for managing expandable and collapsible text sections.

A dedicated class was implemented to which expandable text sections are assigned. This class manages their functionality, including expanding and collapsing the text when the user clicks the corresponding button.

The expandable text sections can display plain text or another dialog.

The functionality is illustrated in:

gui2.png – collapsed text sections.

gui2a.png – expanded text sections.

Technologies

C++

Microsoft Foundation Classes (MFC)

Windows API

Object-oriented programming

GDI drawing
