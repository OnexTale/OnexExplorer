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
    Source/main.cpp \
    Source/ImageConverter.cpp \
    Source/MainWindow.cpp \
    Source/Ui/OnexTreeItem.cpp \
    Source/Decryptors/NosTextDatFileDecryptor.cpp \
    Source/Decryptors/NosTextOthersFileDecryptor.cpp \
    Source/Decryptors/NosZlibDecryptor.cpp \
    Source/Openers/NosZlibOpener.cpp \
    Source/Openers/NosCCInfOpener.cpp \
    Source/Openers/NosTextOpener.cpp \
    Source/Openers/INosFileOpener.cpp \
    Source/Ui/Previews/SingleTextFilePreview.cpp \
    Source/Ui/Previews/SingleImagePreview.cpp \
    Source/Ui/Previews/MultiImagePreview.cpp \
    Source/Ui/TreeItems/OnexTreeImage.cpp \
    Source/Ui/TreeItems/OnexTreeText.cpp \
    Source/Ui/TreeItems/OnexTreeZlibItem.cpp \
    Source/Ui/TreeItems/OnexNSipData.cpp \
    Source/Ui/TreeItems/OnexNS4BbData.cpp \
    Source/Ui/TreeItems/OnexNStcData.cpp \
    Source/Ui/TreeItems/OnexNStpData.cpp \
    Source/Ui/TreeItems/OnexNStpMipMap.cpp \
    Source/Ui/TreeItems/OnexNSmpData.cpp \
    Source/Ui/TreeItems/OnexNSmpFrame.cpp

HEADERS  += \
    Source/NosEnumTypes.h \
    Source/ImageConverter.h \
    Source/MainWindow.h \
    Source/Ui/OnexTreeItem.h \
    Source/Decryptors/NosTextDatFileDecryptor.h \
    Source/Decryptors/NosTextOthersFileDecryptor.h \
    Source/Decryptors/NosZlibDecryptor.h \
    Source/Openers/NosZlibOpener.h \
    Source/Openers/NosCCInfOpener.h \
    Source/Openers/NosTextOpener.h \
    Source/Openers/INosFileOpener.h \
    Source/Ui/Previews/SingleTextFilePreview.h \
    Source/Ui/Previews/SingleImagePreview.h \
    Source/Ui/Previews/MultiImagePreview.h \
    Source/Ui/TreeItems/OnexTreeImage.h \
    Source/Ui/TreeItems/OnexTreeText.h \
    Source/Ui/TreeItems/OnexTreeZlibItem.h \
    Source/Ui/TreeItems/OnexNSipData.h \
    Source/Ui/TreeItems/OnexNS4BbData.h \
    Source/Ui/TreeItems/OnexNStcData.h \
    Source/Ui/TreeItems/OnexNStpData.h \
    Source/Ui/TreeItems/OnexNStpMipMap.h \
    Source/Ui/TreeItems/OnexNSmpData.h \
    Source/Ui/TreeItems/OnexNSmpFrame.h

FORMS    += \
    Source/mainwindow.ui \
    Source/Ui/Previews/SingleTextFilePreview.ui \
    Source/Ui/Previews/SingleImagePreview.ui \
    Source/Ui/Previews/MultiImagePreview.ui \

DISTFILES +=

RESOURCES += \
    resources.qrc

RC_ICONS = ./resources/oxe_icon_trans.ico
