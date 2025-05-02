# Overview

This is a test project to show my abilities in developing desktop applications with **Qt** and **QML**.  

<br/>

## Task  

The main task of the app is to count words occurancies in opened text file and show top 15 words (by number of occurancies) in the form of histogram on UI.  
Words are counted in another thread, UI should be adaptive to window size, histogram made without using *ChartView*.  
Also UI contains histogram, progress bar and buttons to open file, start, stop, cancel words counting.

<br/>

## Project structure

There is schematic structure of project directory:

<br/>

```
file-reader-qt/
├── src/
│   ├── abstractworker/
│   │   └── abstractworker.h/.cpp # Base class for workers that implement async control from Controller 
│   ├── controller.h/.cpp         # Manages worker thread, controls start/pause/cancel
│   ├── filereaderworker.h/.cpp   # Worker class for file reading
│   ├── main.cpp                  # Setup QML engine, register C++ types
│   ├── wordscountermodel.h/.cpp  # QAbstractListModel to expose word statistics to QML
│   └── wordscounterworker.h/.cpp # Worker class for counting top n frequent words
├── qml/
│   ├── components/
│   │   ├── HistogramBar.qml      # Bar Item of the histogram
│   │   └── HistogramView.qml     # Full histogram view
│   ├── views/
│   │   └── MainView.qml          # Main window with histogram, progress bar and buttons
│   └── Main.qml                  # Root UI
├── tests/                        # cpp unit tests
├── README.md                     # Architecture, setup, logic
├── CMakeLists.txt
└── .gitignore
```

<br/>

## Build

I built project using **CMake**, **Qt 6.9.0** (quick and core), **MinGW 64-bit**, **Qt Creator**.  
Build process should be standard as for any other Qt app developed in environment configured via *Qt Online Installer*.

<br/>

# App logic

High-level workflow of app logic:
1. Init state in which user can only open text file for next processing.
2. User click 'Open file' button and select file for processing through default system file dialog.
3. User see name of opened file in status info line under histogram and can start processing.
4. User click 'Start' button and words reading and counting starts and it works as follows:
    - *FileReaderWorker* reads file word by word and store it in temporary string list,
    - *Controller* copy read words and progress of file reading from *FileReaderWorker* after constant time interval, flash temporary string list of *FileReaderWorker* and pass it to *WordsCounterWorker*,
    - *WordsCounterWorker* add read words to *hash map* of all read words with their occurancies in file and start recalculating top n frequent words in file with the help of *priority queue*,
    - *WordsCounterWorker* pass calculated top words and progress to *WordsCounterModel*,
    - *WordsCounterModel* update *histogram* data and *progress bar* value,
    - both workers execute file reading and top of frequent words searching in separate threads in parallel,
    - *histogram* and *progress bar* are updating constantly while process is going without UI lags.
5. After processing finish user can see result words statistic and start again from step 1 (statistic will be resetted after opening new file).

Also user can interrupt processing as follows:
1. User can pause processing clicking 'Pause' button which will cause freezing *workers* threads in the middle of processing (due to *condition variable*) and stopping of *update timer*,
2. User can continue processing cliking 'Continue' button (the same 'Pause' button) which wil unfreeze *workers* and start *update timer* again,
3. User can cancel processing clicking 'Cancel' button which will stop *workers* and *update timer*, flash their temporary data and flash histogram data which will make app go into the init state.

Other key moments:    
- *workers* and *Controller* with UI in GUI thread works mostly asynchronously in parallel (excluding pausing and canceling),  
- *histogram*, *progress bar* and *status info* are updated in real time,
- *status info* line shows current app state and suggests what to do further,
- *histogram* is custom without using *ChartView* (*Reapeter* for creating multiple bars that take *WordsCounterModel* as a model),
- *histogram bars* and *progress bar* are simply animated (bar height and progress value accordingly),
- UI *buttons*, *progress bar* (only height) and texts font are fixed sized, other UI elements (full histogram and progress bar width) are adapted to window size.

<br/>

# Code technologies and patterns

## MVC

Code is divided into modules as follows: 
- **Model** (FileReaderWorker, WordsCounterWorker, WordsCounterModel),
- **View** (MainView, HistogramBar, HistogramView),
- **Controller** (Controller).

<br/>

1. FileReaderWorker, WordsCounterWorker are responsible for data processing, WordsCounterModel - storing processed data in appropriate format.
2. MainView, HistogramBar, HistogramView are responsible for correct displaying of processed data and user interface.
3. Controller converts user input from **View** to commands for **Model** .

<br/>

## Observer pattern

**Qt slots and signals** and **Qt event system** implement observer pattern. Using these mechanism give possibility to make easy enough and smooth communication between processes in different threads and classes without having to use synchronizations everywhere.

<br/>

## Producer-consumer pattern

*FileReaderWorker* and *WordsCounterWorker* produce portions of data as **producers** for *WordsCounterModel* that use these portions for UI update as **consumer**. This approach divide big amount of work on small jobs executing of which doesn't stop UI and also divide logic in *workers* in their threads and UI in GUI thread.
 
<br/>

## Used technologies

- **Qt tests** for cpp unit tests,
- **Logging in file** and output with built-in Qt logging system,
- **QAbstractModel class inheritance** for realtime UI update (histogramm and progres bar),
- **std threads synchronization primitives (shared_mutex, condition_variable, atomic)** for asynchronous controling workers from *Controller* (pause, resume, cancel),
- **QTimer** that constantly in time takes read words from *FileReaderWorker* and send them to *WordsCounterWorker*,
- **QThreads** for executing heavy operations with files and words counting in separated threads,
- **QRegularExpression** for spliting read substrings on words consisting only a-z ans A-Z symbols,
- **Q_PROPERTIES** in *Controller* and *WordsCounterModel* to smoothly update QML Items properties,
- **signals and slots** for appropriate communication between *Controller* comands and *workers*,
- **QML Layouts** for *adaptive UI*,
- **RowLayout with Reapeter** that implement *histogram bars*,
- **QML Items like ProgressBar, Buttons, TextMetrics, Texts** to create desirable UI,
- **QML animations** for *progress bar* value changing and *histogram bars* height changing.

<br/>

# Appendix

There are still some moments that could be done:
- states of app could be wrapped in **QState** objects for better understanding app elements values in different states and clear transitions between them (it would be realization of **State pattern**),
- maybe there could be better approaches in connecting processing steps (sometimes cancel process can be executed before some last data portions processing finish what will lead to executing some work after canceling what is not well),
- more time for tests and logs creation (I was limited in time so tests are temporarily turned off and logs can be not so informative, I added them to show that I use it in my job),
- when process big file, after some point *hash map of counted words* becomes too big what leads to slowing search of top frequent words on each UI update that accordingly leads to UI lags (the problem is in algorithm of constant updating, it multiplies already big time fpr processing each UI update; I don't see a solution for it, even processing hash map only once can be slow if it big enough, so using more powerful hardware and/or processing enough small files is th only option for me (I also can increase time intervals between updates but this solution also has limit)),
- adding **regular expression** for spliting words that match "[a-zA-Z]+" made processing much slower (I think about x10 times), maybe there is a better solution.
