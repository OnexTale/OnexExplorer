#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QScrollArea>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setAcceptDrops(true);
    this->setWindowTitle(QString("OnexExplorer %1 [Beta]").arg(VERSION));

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->filterInput, SIGNAL(textChanged(QString)), this, SLOT(filterItems()));
    connect(ui->filterInput, SIGNAL(returnPressed()), this, SLOT(filterItems()));
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onCustomMenuShow(QPoint)));
}

MainWindow::~MainWindow() {
    if (contextMenu) {
        clearMenu();
        delete contextMenu;
    }

    delete ui;
}

void MainWindow::filterItems() {
    QString searched = ui->filterInput->text();

    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++) {
        for (int j = 0; j < ui->treeWidget->topLevelItem(i)->childCount(); j++)
            ui->treeWidget->topLevelItem(i)->child(j)->setHidden(true);
    }

    QList<QTreeWidgetItem *> matches = ui->treeWidget->findItems(searched, Qt::MatchStartsWith | Qt::MatchRecursive);
    for (QTreeWidgetItem *m : matches) {
        if (m->parent() && m->parent()->isHidden()) {
            m->parent()->setHidden(false);
            if (m->parent()->parent()) {
                for (int i = 0; i < m->parent()->childCount(); i++)
                    m->parent()->child(i)->setHidden(true);
            }
        }
        m->setHidden(false);
    }
}

void MainWindow::onCustomMenuShow(const QPoint &point) {
    Q_UNUSED(point);

    if (contextMenu) {
        clearMenu();
        delete contextMenu;
        contextMenu = nullptr;
    }
    OnexTreeItem *item = static_cast<OnexTreeItem *>(ui->treeWidget->currentItem());
    if (item == nullptr)
        return;

    contextMenu = new QMenu();
    if (!item->hasParent()) {
        QAction *exportAllAction = new QAction(QObject::tr("Export"), contextMenu);
        contextMenu->addAction(exportAllAction);
        QObject::connect(exportAllAction, SIGNAL(triggered(bool)), this, SLOT(on_actionExport_triggered()));

        QAction *exportOriginalAction = new QAction(QObject::tr("Export as .NOS"), contextMenu);
        contextMenu->addAction(exportOriginalAction);
        QObject::connect(exportOriginalAction, SIGNAL(triggered(bool)), this, SLOT(on_actionSave_as_triggered()));

        QAction *replaceAction = new QAction(QObject::tr("Replace"), contextMenu);
        contextMenu->addAction(replaceAction);
        QObject::connect(replaceAction, SIGNAL(triggered(bool)), this, SLOT(on_actionReplace_triggered()));

        QAction *closeThisItem = new QAction(QObject::tr("Close"), contextMenu);
        contextMenu->addAction(closeThisItem);
        QObject::connect(closeThisItem, SIGNAL(triggered(bool)), this, SLOT(on_actionClose_selected_triggered()));
    } else {
        QAction *exportSingleAction = new QAction(QObject::tr("Export"), contextMenu);
        contextMenu->addAction(exportSingleAction);
        QObject::connect(exportSingleAction, SIGNAL(triggered(bool)), this, SLOT(on_actionExport_triggered()));

        QAction *exportSingleToRawAction = new QAction(QObject::tr("Export to raw"), contextMenu);
        contextMenu->addAction(exportSingleToRawAction);
        QObject::connect(exportSingleToRawAction, SIGNAL(triggered(bool)), this,
                         SLOT(on_actionExport_to_raw_triggered()));

        QAction *replaceAction = new QAction(QObject::tr("Replace"), contextMenu);
        contextMenu->addAction(replaceAction);
        QObject::connect(replaceAction, SIGNAL(triggered(bool)), this, SLOT(on_actionReplace_triggered()));

        QAction *replaceRawAction = new QAction(QObject::tr("Replace with raw"), contextMenu);
        contextMenu->addAction(replaceRawAction);
        QObject::connect(replaceRawAction, SIGNAL(triggered(bool)), this, SLOT(on_actionReplace_with_raw_triggered()));

        QAction *deleteAction = new QAction(QObject::tr("Delete"), contextMenu);
        contextMenu->addAction(deleteAction);
        QObject::connect(deleteAction, SIGNAL(triggered(bool)), this, SLOT(on_actionClose_selected_triggered()));
    }

    if (contextMenu->isEmpty())
        return;
    qDebug() << contextMenu->height();
    contextMenu->exec(QCursor::pos());
}

void MainWindow::clearMenu() {
    qDebug() << "Disposing menu";
    QList<QAction *> actions = contextMenu->actions();

    for (auto &action : actions)
        delete action;

    contextMenu->clear();
}

void MainWindow::on_actionOpen_triggered() {
    QFileDialog openDialog(this);

    openDialog.setFileMode(QFileDialog::ExistingFiles);
    openDialog.setNameFilter(tr("NosTale Files (*.NOS)"));
    openDialog.setViewMode(QFileDialog::Detail);

    QStringList selectedFiles;
    if (openDialog.exec())
        selectedFiles = openDialog.selectedFiles();

    if (!selectedFiles.empty()) {
        for (auto &file : selectedFiles)
            openFile(file);
    }
    filterItems();
}

void MainWindow::openFile(QString path) {
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly))
        return;

    if (hasValidHeader(file) == 1)
        handleOpenResults(zlibOpener.decrypt(file), path);
    else if (hasValidHeader(file) == 2)
        handleOpenResults(ccinfOpener.decrypt(file), path);
    else
        handleOpenResults(textOpener.decrypt(file), path);

    file.close();
}

void MainWindow::handleOpenResults(OnexTreeItem *item, QString path) {
    item->setData(0, Qt::UserRole, path);
    ui->treeWidget->addTopLevelItem(item);
    item->setExpanded(true);
}

int MainWindow::hasValidHeader(QFile &file) {
    file.seek(0);
    QByteArray header = file.read(0x0B);
    if (header.mid(0, 7) == "NT Data" || header.mid(0, 10) == "32GBS V1.0" || header.mid(0, 10) == "ITEMS V1.0")
        return 1;
    else if (header.mid(0, 11) == "CCINF V1.20")
        return 2;
    return 0;
}

QString MainWindow::getSelectedDirectory(QString suggestion) {
    QString dir = QFileDialog::getExistingDirectory(0, tr("Select Directory"), suggestion);
    if (dir.isEmpty())
        return dir;

    return dir + "/";
}

QString MainWindow::getOpenFile(QString suggestion, QString filter) {
    return QFileDialog::getOpenFileName(0, tr("Open..."), suggestion, filter);
}

QString MainWindow::getSaveDirectory(QString suggestion, QString filter) {
    return QFileDialog::getSaveFileName(0, tr("Save as..."), suggestion, filter);
}

void MainWindow::dropEvent(QDropEvent *e) {
    for (auto &url : e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        openFile(fileName);
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e) {
    for (auto &url : e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        if (QFileInfo(fileName).suffix() != "NOS")
            return;
    }
    e->acceptProposedAction();
}

void MainWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *prev) {
    Q_UNUSED(prev);
    delete ui->previewLayout->takeAt(1)->widget();
    delete ui->previewLayout->takeAt(0)->widget();

    QWidget *previewWindow;
    QWidget *infoWindow;
    QScrollArea *scrollArea = new QScrollArea();

    OnexTreeItem *item = static_cast<OnexTreeItem *>(current);
    if (item == nullptr) {
        previewWindow = nullptr;
        infoWindow = nullptr;
    } else {
        previewWindow = item->getPreview();
        infoWindow = item->getInfos();
        scrollArea->setWidget(infoWindow);
        scrollArea->setMinimumWidth(250);
        scrollArea->setMaximumWidth(250);
    }

    if (!previewWindow) {
        previewWindow = new QWidget();
        previewWindow->setMaximumSize(0, 0);
    }
    if (!infoWindow) {
        infoWindow = new QWidget();
        scrollArea->setMaximumSize(0, 0);
        infoWindow->setMaximumSize(0, 0);
    }

    ui->previewLayout->addWidget(previewWindow, 0, 0);
    ui->previewLayout->addWidget(scrollArea, 0, 1);
}

void MainWindow::on_actionClose_selected_triggered() {
    QList<QTreeWidgetItem *> selectedItems = ui->treeWidget->selectedItems();
    foreach (auto &item, selectedItems) {
        if (item->parent() && item->parent()->childCount() == 1) {
            delete item->parent();
        } else
            delete item;
    }
}

void MainWindow::on_actionReplace_triggered() {
    QList<QTreeWidgetItem *> selectedItems = ui->treeWidget->selectedItems();
    if (selectedItems.size() == 0) {
        QMessageBox::information(NULL, tr("Info"), tr("Select file first"));
        return;
    }

    int count = 0;

    if (selectedItems.size() == 1 && selectedItems.at(0)->childCount() == 0) {
        OnexTreeItem *item = static_cast<OnexTreeItem *>(selectedItems.at((0)));
        QString filter = "";
        if (item->getExportExtension() == ".png")
            filter = "PNG Image (*.png)";
        else if (item->getExportExtension() == ".txt")
            filter = "Text File (*.txt)";
        else if (item->getExportExtension() == ".lst")
            filter = "List File (*.lst)";
        else if (item->getExportExtension() == ".dat")
            filter = "DAT File (*.dat)";
        else if (item->getExportExtension() == ".obj")
            filter = "OBJ File (*.obj)";
        else
            filter = "All files (*.*)";

        QString path = getOpenFile(inExportPath + item->getName(), filter);
        inExportPath = path.mid(0,path.lastIndexOf("/")) + "/";
        if (path.isEmpty())
            return;
        count = item->onReplace(path);

    } else {
        QString directory = getSelectedDirectory(inExportPath);
        if (directory.isEmpty())
            return;

        inExportPath = directory;

        foreach (auto &s, selectedItems) {
            OnexTreeItem *item = static_cast<OnexTreeItem *>(s);
            count += item->onReplace(directory);
        }
    }

    QString text = "Replaced " + QString::number(count) + " file(s).";
    QMessageBox msgBox(QMessageBox::Information, tr("End of operation"), text);
    msgBox.exec();
}

void MainWindow::on_actionReplace_with_raw_triggered() {
    QList<QTreeWidgetItem *> selectedItems = ui->treeWidget->selectedItems();
    if (selectedItems.size() == 0) {
        QMessageBox::information(NULL, tr("Info"), tr("Select file first"));
        return;
    }

    int count = 0;

    if (selectedItems.size() == 1 && selectedItems.at(0)->childCount() == 0) {
        OnexTreeItem *item = static_cast<OnexTreeItem *>(selectedItems.at((0)));
        QString path = getOpenFile(inExportPath + item->getName(), "Rawdata (*.bin)");
        inExportPath = path.mid(0,path.lastIndexOf("/")) + "/";
        if (path.isEmpty())
            return;
        count = item->onReplaceRaw(path);
    } else {
        QString directory = getSelectedDirectory(inExportPath);
        if (directory.isEmpty())
            return;
        inExportPath = directory;

        foreach (auto &s, selectedItems) {
            OnexTreeItem *item = static_cast<OnexTreeItem *>(s);
            count += item->onReplaceRaw(directory);
        }
    }

    QString text = "Replaced " + QString::number(count) + " file(s).";
    QMessageBox msgBox(QMessageBox::Information, tr("End of operation"), text);
    msgBox.exec();
}

void MainWindow::on_actionExport_triggered() {
    QList<QTreeWidgetItem *> selectedItems = ui->treeWidget->selectedItems();
    if (selectedItems.size() == 0) {
        QMessageBox::information(NULL, tr("Info"), tr("Select file first"));
        return;
    }

    int count = 0;

    if (selectedItems.size() == 1 && selectedItems.at(0)->childCount() == 0) {
        OnexTreeItem *item = static_cast<OnexTreeItem *>(selectedItems.at((0)));
        QString filter = "";
        if (item->getExportExtension() == ".png")
            filter = "PNG Image (*.png)";
        else if (item->getExportExtension() == ".txt")
            filter = "Text File (*.txt)";
        else if (item->getExportExtension() == ".lst")
            filter = "List File (*.lst)";
        else if (item->getExportExtension() == ".dat")
            filter = "DAT File (*.dat)";
        else if (item->getExportExtension() == ".obj")
            filter = "OBJ File (*.obj)";
        else
            filter = "All files (*.*)";
        QString path = getSaveDirectory(inExportPath + item->getName(), filter);
        inExportPath = path.mid(0,path.lastIndexOf("/")) + "/";
        if (path.isEmpty())
            return;
        count = item->onExport(path);
    } else {
        QString directory = getSelectedDirectory(inExportPath);
        if (directory.isEmpty())
            return;
        inExportPath = directory;

        foreach (auto &s, selectedItems) {
            OnexTreeItem *item = static_cast<OnexTreeItem *>(s);
            count += item->onExport(directory);
        }
    }

    QString text = "Saved " + QString::number(count) + " file(s).";
    QMessageBox msgBox(QMessageBox::Information, tr("End of operation"), text);
    msgBox.exec();
}

void MainWindow::on_actionExport_to_raw_triggered() {
    QList<QTreeWidgetItem *> selectedItems = ui->treeWidget->selectedItems();
    if (selectedItems.size() == 0) {
        QMessageBox::information(NULL, tr("Info"), tr("Select file first"));
        return;
    }

    int count = 0;

    if (selectedItems.size() == 1 && selectedItems.at(0)->childCount() == 0) {
        OnexTreeItem *item = static_cast<OnexTreeItem *>(selectedItems.at((0)));
        QString path = getSaveDirectory(inExportPath + item->getName(), "Rawdata (*.bin)");
        inExportPath = path.mid(0,path.lastIndexOf("/")) + "/";
        if (path.isEmpty())
            return;
        count = item->onExportRaw(path);
    } else {
        QString directory = getSelectedDirectory(inExportPath);
        if (directory.isEmpty())
            return;
        inExportPath = directory;

        foreach (auto &s, selectedItems) {
            OnexTreeItem *item = static_cast<OnexTreeItem *>(s);
            count += item->onExportRaw(directory);
        }
    }
    QString text = "Saved " + QString::number(count) + " file(s).";
    QMessageBox msgBox(QMessageBox::Information, tr("End of operation"), text);
    msgBox.exec();
}

void MainWindow::on_actionAbout_triggered() {
    QMessageBox::information(
        NULL, tr(qPrintable(QString("About OnexExplorer %1").arg(VERSION))),
        tr(qPrintable(QString("<center><b>OnexExplorer %1</b></center>"
                              "<br>OnexExplorer is an open-source tool for unpacking and repacking .NOS "
                              "data files from game called NosTale. "
                              "<br>It can open almost all .NOS files and show and replace the data stored in "
                              "them."
                              "<br>This fork is Maintained by @Pumba98"
                              "<br>GitHub: <a "
                              "href='https://github.com/Pumbaa98/OnexExplorer'>"
                              "https://github.com/Pumbaa98/OnexExplorer</a>")
                          .arg(VERSION))));
}

void MainWindow::on_actionHelp_triggered() {
    QMessageBox::information(NULL, tr("Get Help"),
                             tr("If you have problems with OnexExplorer you can: "
                                "<br> <a href='https://github.com/Pumbaa98/OnexExplorer/issues'>"
                                "Open an issue on Github</a>"
                                "<br><a href='https://www.elitepvpers.com/forum/nostale-hacks-bots-cheats-exploits/"
                                "4632151-onexexplorer-client-modding-tool.html'>"
                                "Contact me on Elitepvpers </a>"
                                "<br>Or add me on Discord: Pumbaa98#6817"));
}

void MainWindow::on_actionSave_as_triggered() {
    if (ui->treeWidget->currentItem()) {
        OnexTreeItem *item = static_cast<OnexTreeItem *>(ui->treeWidget->currentItem());
        while (item->hasParent()) {
            item = static_cast<OnexTreeItem *>(item->QTreeWidgetItem::parent());
        }
        if (nosPath.isNull()) {
            nosPath = item->data(0, Qt::UserRole).toString();
            nosPath = nosPath.mid(0,nosPath.lastIndexOf("/")) + "/";
        }
        QString path = getSaveDirectory(nosPath + item->getName(), "NOS Archive (*.NOS)");
        nosPath = path.mid(0,path.lastIndexOf("/")) + "/";
        item->onExportAsOriginal(path);
    } else {
        QMessageBox::information(NULL, tr("Info"), tr("Select .NOS file first"));
    }
}

void MainWindow::on_actionSave_triggered() {
    if (ui->treeWidget->currentItem()) {
        OnexTreeItem *item = static_cast<OnexTreeItem *>(ui->treeWidget->currentItem());
        while (item->hasParent()) {
            item = static_cast<OnexTreeItem *>(item->QTreeWidgetItem::parent());
        }
        item->onExportAsOriginal(item->data(0, Qt::UserRole).toString());
    }
}

void MainWindow::on_actionExit_triggered() {
    QMessageBox::StandardButton message =
        QMessageBox::question(this, "", "Exit program? All unsaved changes will be lost!",
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (message == QMessageBox::Yes)
        QApplication::quit();
}

void MainWindow::on_actionClose_all_triggered() {
    QMessageBox::StandardButton message =
        QMessageBox::question(this, "", "Close all items? All unsaved changes will be lost!",
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (message == QMessageBox::Yes) {
        ui->treeWidget->clear();
    }
}
