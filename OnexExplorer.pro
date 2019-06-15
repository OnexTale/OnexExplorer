#-------------------------------------------------
#
# Project created by QtCreator 2017-03-12T15:48:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OnexExplorer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    Source/Ui/OnexNSmpFrame.cpp \
    Source/main.cpp \
    Source/Decryptors/NosTextDatFileDecryptor.cpp \
    Source/Openers/NosTextOpener.cpp \
    Source/MainWindow.cpp \
    Source/Decryptors/NosTextOthersFileDecryptor.cpp \
    Source/Openers/INosFileOpener.cpp \
    Source/Ui/OnexTreeItem.cpp \
    Source/Ui/SingleTextFilePreview.cpp \
    Source/Decryptors/NosZlibDecryptor.cpp \
    Source/Openers/NosZlibOpener.cpp \
    Source/Ui/SingleImagePreview.cpp \
    Source/ImageConverter.cpp \
    Source/Ui/FileInfo.cpp \
    Source/Ui/OnexTreeImage.cpp \
    Source/Ui/OnexTreeText.cpp \
    Source/Ui/OnexTreeZlibItem.cpp \
    Source/Ui/OnexNSipData.cpp \
    Source/Ui/OnexNS4BbData.cpp \
    Source/Ui/OnexNStcData.cpp \
    Source/Ui/OnexNStpData.cpp \
    Source/Openers/NosCCInfOpener.cpp \
    Source/Ui/OnexNSmpData.cpp

HEADERS  += \
    Source/Decryptors/NosTextDatFileDecryptor.h \
    Source/Decryptors/INosDecryptor.h \
    Source/Openers/INosFileOpener.h \
    Source/Openers/NosTextOpener.h \
    Source/MainWindow.h \
    Source/Decryptors/NosTextOthersFileDecryptor.h \
    Source/Ui/OnexNSmpFrame.h \
    Source/Ui/OnexTreeItem.h \
    Source/Ui/SingleTextFilePreview.h \
    Source/Decryptors/NosZlibDecryptor.h \
    Source/Openers/NosZlibOpener.h \
    Source/NosEnumTypes.h \
    Source/Ui/SingleImagePreview.h \
    Source/ImageConverter.h \
    Source/Ui/FileInfo.h \
    Source/Ui/OnexTreeImage.h \
    Source/Ui/OnexTreeText.h \
    Source/Ui/OnexTreeZlibItem.h \
    Source/Ui/OnexNSipData.h \
    Source/Ui/OnexNS4BbData.h \
    Source/Ui/OnexNStcData.h \
    Source/Ui/OnexNStpData.h \
    Source/Openers/NosCCInfOpener.h \
    Source/Ui/OnexNSmpData.h

FORMS    += \
    Source/mainwindow.ui \
    Source/Ui/SingleTextFilePreview.ui \
    Source/Ui/SingleImagePreview.ui \
    Source/Ui/FileInfo.ui

DISTFILES +=

RESOURCES += \
    resources.qrc

RC_ICONS = ./resources/oxe_icon_trans.ico
