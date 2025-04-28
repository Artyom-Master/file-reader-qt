# Overview

This is a test project to show my abilities in developing desktop applications with **Qt** and **QML**.  

## Task  

The main task of the app is to count words occurancies in opened text file and show top 15 words (by number of occurancies) in the form of histogram on UI.  
Words are counted in another thread, UI should be adaptive to window size, histogram made without using *ChartView*.  
Also UI contains histogram, progress bar and buttons to open file, start, stop, cancel words counting.

## Project structure

There is schematic structure of project directory:

<br/>

```
file-reader-qt/
├── src/
│   ├── main.cpp                  # Setup QML engine, register C++ types
│   ├── filereaderworker.h/.cpp   # Worker object for file reading and word counting
│   ├── wordstatsmodel.h/.cpp     # QAbstractListModel to expose word statistics to QML
│   └── controller.h/.cpp         # Manages worker thread, controls start/pause/cancel
├── qml/
│   ├── Main.qml                  # Root UI
│   ├── components/
│   │   ├── HistogramBar.qml      # One bar in the histogram
│   │   └── HistogramView.qml     # Full histogram view
│   └── views/
│       └── MainView.qml          # Main window with histogram, progress bar and buttons
├── tests/                        # cpp unit tests
├── README.md                     # Architecture, setup, logic
├── CMakeLists.txt
└── .gitignore
```

<br/>

## Used technologies

- **Qt tests** for cpp unit tests
- **Logging in file** and output with built-in Qt logging system
- **QAbstractModel class inheritance** for realtime UI update (histogramm and progres bar)

<br/>

**_ADD HERE_**

# Build

I built project using **CMake**, **Qt 6.9.0** (quick and core), **MinGW 64-bit**, **Qt Creator**.  
Build process should be standard as for any other Qt app developed in environment configured via *Qt Online Installer*.

# App logic

## MVC

All app logic divided into: 
- **Model** (FileReaderWorker.cpp)
- **View** (MainView.qml, HistogramBar.qml, HistogramView.qml)
- **Controller** (Controller.cpp)

<br/>

1. FileReaderWorker is responsible for data processing and storing in appropriate format
2. MainView.qml, HistogramBar.qml, HistogramView.qml and WordStatsModel.cpp are responsible for correct displaying of data model and user interface
3. Controller.cpp converts user input from **View** to commands for **Model** 

<br/>

*I added WordStatsModel to **View** because it works in GUI thread and data for displaying is gotten by it from FileReaderWorker when it should be displayed*

<br/>

**_ADD HERE_**
