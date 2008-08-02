TEMPLATE = app
TARGET = uman
QT += core \
    gui \
    xml
CONFIG += debug
HEADERS += tasks/renameTaskDialog/QURenameConditionDelegate.h \
    tasks/renameTaskDialog/QURenameSourceDelegate.h \
    tasks/renameTaskDialog/QURenameTextDelegate.h \
    tasks/renameTaskDialog/QURenameDataTable.h \
    tasks/renameTaskDialog/QURenameTaskDialog.h \
    QUPictureDialog.h \
    tasks/QUCleanTask.h \
    QUMessageBox.h \
    reports/QUPlainTextReport.h \
    reports/QUBooleanSongData.h \
    reports/QUAbstractReport.h \
    reports/QUHtmlReport.h \
    reports/QUReportItem.h \
    reports/QUSongTagData.h \
    reports/QUAbstractReportData.h \
    QUReportDialog.h \
    songtree/QUSongTree.h \
    songtree/QUSongItem.h \
    QUProgressDialog.h \
    details/QUTagItem.h \
    details/QUDetailsTable.h \
    details/QUDetailItem.h \
    tasks/QUPreparatoryTask.h \
    tasks/QURenameTask.h \
    tasks/QUTaskList.h \
    tasks/QUTaskItem.h \
    tasks/QUAudioTagTask.h \
    tasks/QUAbstractTask.h \
    QU.h \
    QUTextDialog.h \
    QUDropDownDelegate.h \
    main.h \
    QUMonty.h \
    QUTagOrderDialog.h \
    QUSongFile.h \
    QUMainWindow.h
SOURCES += tasks/renameTaskDialog/QURenameConditionDelegate.cpp \
    tasks/renameTaskDialog/QURenameSourceDelegate.cpp \
    tasks/renameTaskDialog/QURenameTextDelegate.cpp \
    tasks/renameTaskDialog/QURenameDataTable.cpp \
    tasks/renameTaskDialog/QURenameTaskDialog.cpp \
    QUPictureDialog.cpp \
    tasks/QUCleanTask.cpp \
    QUMessageBox.cpp \
    reports/QUPlainTextReport.cpp \
    reports/QUBooleanSongData.cpp \
    reports/QUAbstractReport.cpp \
    reports/QUHtmlReport.cpp \
    reports/QUReportItem.cpp \
    reports/QUSongTagData.cpp \
    reports/QUAbstractReportData.cpp \
    QUReportDialog.cpp \
    songtree/QUSongTree.cpp \
    songtree/QUSongItem.cpp \
    QUProgressDialog.cpp \
    details/QUTagItem.cpp \
    details/QUDetailsTable.cpp \
    details/QUDetailItem.cpp \
    tasks/QUPreparatoryTask.cpp \
    tasks/QURenameTask.cpp \
    tasks/QUAudioTagTask.cpp \
    tasks/QUTaskList.cpp \
    tasks/QUTaskItem.cpp \
    tasks/QUAbstractTask.cpp \
    QUTextDialog.cpp \
    QUDropDownDelegate.cpp \
    QUMonty.cpp \
    QUTagOrderDialog.cpp \
    QUSongFile.cpp \
    QUMainWindow.cpp \
    main.cpp
FORMS += QURenameTaskDialog.ui \
    QUPictureDialog.ui \
    QUMessageBox.ui \
    QUReportDialog.ui \
    QUProgressDialog.ui \
    QUTextDialog.ui \
    QUTagOrderDialog.ui \
    QUMainWindow.ui
RESOURCES += images/uman.qrc
TRANSLATIONS = uman_de.ts
INCLUDEPATH += tasks \
    details \
    songtree \
    reports \
    tasks/renameTaskDialog
win32 { 
    RC_FILE = uman.rc
    INCLUDEPATH += ../include/taglib
    LIBS += -L"../lib" \
        -ltag
}
unix { 
    INCLUDEPATH += /usr/include/taglib
    LIBS += -L"/usr/lib" \
        -ltag
}
QMAKE_EXTRA_TARGETS += revtarget
PRE_TARGETDEPS += version.h
revtarget.target = version.h
revtarget.commands = @echo \
    "const char *revision = \"r$(shell svnversion .)\";" > $$revtarget.target
revtarget.depends = $$SOURCES \
    $$HEADERS \
    $$FORMS
