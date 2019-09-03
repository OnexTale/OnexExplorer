#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "Ui/TreeItems/OnexNSmpData.h"
#include <QScrollArea>
#include <QJsonDocument>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setAcceptDrops(true);
    this->setWindowTitle(QString("OnexExplorer %1 [Beta]").arg(VERSION));
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem * , int)), this,
            SLOT(itemEdited(QTreeWidgetItem * , int)));
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

void MainWindow::on_actionOpen_triggered() {
    QStringList selectedFiles = getOpenFiles(nosPath, "NosTale Files (*.NOS)");
    if (!selectedFiles.empty()) {
        for (auto &file : selectedFiles)
            openFile(file);
    }
    filterItems();
    ui->treeWidget->sortItems(0, Qt::AscendingOrder);
}

void MainWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *prev) {
    Q_UNUSED(prev)
    delete ui->previewLayout->takeAt(1)->widget();
    delete ui->previewLayout->takeAt(0)->widget();
    QWidget *previewWindow;
    FileInfo *infoWindow;
    auto *scrollArea = new QScrollArea();
    auto *item = dynamic_cast<OnexTreeItem *>(current);
    if (item == nullptr) {
        previewWindow = nullptr;
        infoWindow = nullptr;
    } else {
        previewWindow = item->getPreview();
        infoWindow = item->getInfos();
    }
    if (!previewWindow) {
        previewWindow = new QWidget();
        previewWindow->setMaximumSize(0, 0);
    }
    if (infoWindow) {
        connect(infoWindow, SIGNAL(replaceFile()), this, SLOT(on_actionReplace_triggered()));
        connect(infoWindow, SIGNAL(replaceRawFile()), this, SLOT(on_actionReplace_with_raw_triggered()));
        connect(infoWindow, SIGNAL(updateInfo(FileInfo * )), this, SLOT(replaceInfo(FileInfo * )));
        scrollArea->setWidget(infoWindow);
        scrollArea->setMinimumWidth(250);
        scrollArea->setMaximumWidth(250);
    } else {
        infoWindow = new FileInfo();
        scrollArea->setMaximumSize(0, 0);
        infoWindow->setMaximumSize(0, 0);
    }

    ui->previewLayout->addWidget(previewWindow, 0, 0);
    ui->previewLayout->addWidget(scrollArea, 0, 1, Qt::AlignRight);
}

void MainWindow::replaceInfo(FileInfo *info) {
    delete ui->previewLayout->takeAt(1)->widget();
    auto *scrollArea = new QScrollArea();
    connect(info, SIGNAL(updateInfo(FileInfo * )), this, SLOT(replaceInfo(FileInfo * )));
    scrollArea->setWidget(info);
    scrollArea->setMinimumWidth(250);
    scrollArea->setMaximumWidth(250);
    ui->previewLayout->addWidget(scrollArea, 0, 1);
}

void MainWindow::on_actionReplace_triggered() {
    fileOperationOnSelectedItems(&OnexTreeItem::onReplace, inExportPath, "Replaced", false);
}

void MainWindow::on_actionReplace_with_raw_triggered() {
    fileOperationOnSelectedItems(&OnexTreeItem::onReplaceRaw, inExportPath, "Replaced", false, "Rawdata (*.bin)");
}

void MainWindow::on_actionExport_triggered() {
    fileOperationOnSelectedItems(&OnexTreeItem::onExport, inExportPath, "Exported", true);
}

void MainWindow::on_actionExport_to_raw_triggered() {
    fileOperationOnSelectedItems(&OnexTreeItem::onExportRaw, inExportPath, "Exported", true, "Rawdata (*.bin)");
}

void MainWindow::on_actionSave_triggered() {
    OnexTreeItem *root = getTreeRoot();
    if (root != nullptr) {
        if (root->data(0, Qt::UserRole).toString().isEmpty())
            on_actionSave_as_triggered();
        else
            root->onExportAsOriginal(root->data(0, Qt::UserRole).toString());
    } else {
        QMessageBox::information(nullptr, tr("Info"), tr("Select .NOS file first"));
    }
}

void MainWindow::on_addButton_clicked() {
    if (ui->treeWidget->currentItem()) {
        OnexTreeItem *item = static_cast<OnexTreeItem *>(ui->treeWidget->currentItem());
        OnexTreeItem *root = item;
        while (root->hasParent()) {
            root = static_cast<OnexTreeItem *>(root->QTreeWidgetItem::parent());
        }
        if (root->getContent().mid(0, 10) == "NT Data 06") {
            QMessageBox::warning(nullptr, "Ups", "This isn't implemented yet!");
            return;
        }

        OnexTreeItem *emptyItem;
        if (root->getContent().mid(0, 10) == "NT Data 17" || root->getContent().mid(0, 10) == "NT Data 14") {
            if (item->hasParent()) {
                if (item->childCount() == 0) {
                    item = static_cast<OnexTreeItem *>(item->QTreeWidgetItem::parent());
                }
                if (item->childCount() > 0) {
                    auto nsmp = static_cast<OnexNSmpData *>(item);
                    emptyItem = nsmp->addFrame();
                }
            } else {
                emptyItem = getOpener(root->getContent())->getEmptyItem(root->getContent());
                item->insertChild(item->indexOfChild(ui->treeWidget->currentItem()) + 1, emptyItem);
                auto nsmp = static_cast<OnexNSmpData *>(emptyItem);
                nsmp->addFrame();
            }
        } else {
            emptyItem = getOpener(root->getContent())->getEmptyItem(root->getContent());
            root->insertChild(root->indexOfChild(ui->treeWidget->currentItem()) + 1, emptyItem);
        }

        ui->treeWidget->setCurrentItem(emptyItem);
        ui->treeWidget->scrollToItem(emptyItem);
        ui->treeWidget->editItem(emptyItem);
    } else {
        QMessageBox::information(nullptr, tr("Info"), tr("Select .NOS file first"));
    }
}

void MainWindow::on_deleteButton_clicked() {
    on_actionClose_selected_triggered();
}

void MainWindow::on_actionSave_as_triggered() {
    OnexTreeItem *root = getTreeRoot();
    if (root != nullptr) {
        if (nosPath.isNull())
            nosPath = neatPath(root->data(0, Qt::UserRole).toString());
        QString path = getSaveFile(nosPath + root->getName(), "NOS Archive (*.NOS)");
        nosPath = neatPath(path);
        root->onExportAsOriginal(path);
    } else {
        QMessageBox::information(nullptr, tr("Info"), tr("Select .NOS file first"));
    }
}

void MainWindow::on_actionExport_with_config_triggered() {
    OnexTreeItem *root = getTreeRoot();
    if (root != nullptr) {
        QString directory = getSelectedDirectory(inExportPath);
        if (directory.isEmpty())
            return;
        inExportPath = directory;
        int count = root->onExport(directory);
        QByteArray output = QJsonDocument(root->generateConfig()).toJson();
        QFile file(directory + root->getName() + ".json");
        if (file.open(QIODevice::WriteOnly)) {
            file.write(output);
            file.close();
            QMessageBox::information(this, "End of operation", "Exported " + QString::number(count) + " file(s).");
        } else {
            QMessageBox::critical(nullptr, tr("Error!"), tr(QString("Couldn't open " + directory + root->getName() + ".json for writing!").toLocal8Bit()));
        }
    } else {
        QMessageBox::information(nullptr, tr("Info"), tr("Select .NOS file first"));
    }
}

void MainWindow::on_actionImport_from_config_triggered() {
    QString path = getOpenFile(inExportPath, "JSON File (*.json)");
    if (path.isEmpty())
        return;
    inExportPath = neatPath(path);

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;
    handleOpenResults(jsonOpener.load(file, neatPath(path)), "");
    file.close();
}

void MainWindow::filterItems() {
    QString searched = ui->filterInput->text();
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++) {
        for (int j = 0; j < ui->treeWidget->topLevelItem(i)->childCount(); j++)
            ui->treeWidget->topLevelItem(i)->child(j)->setHidden(true);
    }
    QList<QTreeWidgetItem *> matches = ui->treeWidget->findItems(searched, Qt::MatchContains | Qt::MatchRecursive);
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
    Q_UNUSED(point)
    if (contextMenu) {
        clearMenu();
        delete contextMenu;
        contextMenu = nullptr;
    }
    auto *item = dynamic_cast<OnexTreeItem *>(ui->treeWidget->currentItem());
    if (item == nullptr)
        return;
    contextMenu = new QMenu();
    if (!item->hasParent()) {
        auto *exportAllAction = new QAction(QObject::tr("Export"), contextMenu);
        contextMenu->addAction(exportAllAction);
        QObject::connect(exportAllAction, SIGNAL(triggered(bool)), this, SLOT(on_actionExport_triggered()));
        auto *exportWithConfigAction = new QAction(QObject::tr("Export with Config"), contextMenu);
        contextMenu->addAction(exportWithConfigAction);
        QObject::connect(exportWithConfigAction, SIGNAL(triggered(bool)), this, SLOT(on_actionExport_with_config_triggered()));
        auto *exportOriginalAction = new QAction(QObject::tr("Export as .NOS"), contextMenu);
        contextMenu->addAction(exportOriginalAction);
        QObject::connect(exportOriginalAction, SIGNAL(triggered(bool)), this, SLOT(on_actionSave_as_triggered()));
        auto *replaceAction = new QAction(QObject::tr("Replace"), contextMenu);
        contextMenu->addAction(replaceAction);
        QObject::connect(replaceAction, SIGNAL(triggered(bool)), this, SLOT(on_actionReplace_triggered()));
        auto *closeThisItem = new QAction(QObject::tr("Close"), contextMenu);
        contextMenu->addAction(closeThisItem);
        QObject::connect(closeThisItem, SIGNAL(triggered(bool)), this, SLOT(on_actionClose_selected_triggered()));
    } else {
        auto *exportSingleAction = new QAction(QObject::tr("Export"), contextMenu);
        contextMenu->addAction(exportSingleAction);
        QObject::connect(exportSingleAction, SIGNAL(triggered(bool)), this, SLOT(on_actionExport_triggered()));
        auto *exportSingleToRawAction = new QAction(QObject::tr("Export to raw"), contextMenu);
        contextMenu->addAction(exportSingleToRawAction);
        QObject::connect(exportSingleToRawAction, SIGNAL(triggered(bool)), this,
                         SLOT(on_actionExport_to_raw_triggered()));
        auto *replaceAction = new QAction(QObject::tr("Replace"), contextMenu);
        contextMenu->addAction(replaceAction);
        QObject::connect(replaceAction, SIGNAL(triggered(bool)), this, SLOT(on_actionReplace_triggered()));
        auto *replaceRawAction = new QAction(QObject::tr("Replace with raw"), contextMenu);
        contextMenu->addAction(replaceRawAction);
        QObject::connect(replaceRawAction, SIGNAL(triggered(bool)), this, SLOT(on_actionReplace_with_raw_triggered()));
        if (item->flags() & Qt::ItemIsEditable) {
            QAction *editAction = new QAction(QObject::tr("Rename"), contextMenu);
            contextMenu->addAction(editAction);
            QObject::connect(editAction, SIGNAL(triggered(bool)), this, SLOT(on_actionRename_triggered()));
        }
        auto *deleteAction = new QAction(QObject::tr("Delete"), contextMenu);
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

void MainWindow::on_actionClose_selected_triggered() {
    QList<QTreeWidgetItem *> selectedItems = ui->treeWidget->selectedItems();
    if (selectedItems.size() == 0)
        return;
    int topItems = ui->treeWidget->topLevelItemCount();
    QTreeWidgetItem *next = ui->treeWidget->itemBelow(selectedItems.last());
    for (auto &item : selectedItems) {
        if (item->parent() != nullptr && item->parent()->childCount() == 1 && !selectedItems.contains(item->parent())) {
            item = item->parent();
        }
        if (item->parent())
            delete item;
        else {
            QMessageBox::StandardButton message = QMessageBox::question(this, "", "Close .NOS Archive?",
                                                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if (message == QMessageBox::Yes) {
                ui->treeWidget->setCurrentItem(item, 0);
                delete item;
            }
        }
    }
    if (topItems == ui->treeWidget->topLevelItemCount())
        ui->treeWidget->setCurrentItem(next, 0);
}

void MainWindow::on_actionClose_all_triggered() {
    QMessageBox::StandardButton message =
            QMessageBox::question(this, "", "Close all items? All unsaved changes will be lost!");
    if (message == QMessageBox::Yes) {
        ui->treeWidget->clear();
    }
}

void MainWindow::on_actionExit_triggered() {
    QMessageBox::StandardButton message =
            QMessageBox::question(this, "", "Exit program? All unsaved changes will be lost!");
    if (message == QMessageBox::Yes)
        QApplication::quit();
}

void MainWindow::on_actionAbout_triggered() {
    QMessageBox::information(
            nullptr, tr(qPrintable(QString("About OnexExplorer %1").arg(VERSION))),
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
    QMessageBox::information(nullptr, tr("Get Help"),
                             tr("If you have problems with OnexExplorer you can: "
                                "<br> <a href='https://github.com/Pumbaa98/OnexExplorer/issues'>"
                                "Open an issue on Github</a>"
                                "<br><a href='https://www.elitepvpers.com/forum/nostale-hacks-bots-cheats-exploits/"
                                "4632151-onexexplorer-client-modding-tool.html'>"
                                "Contact me on Elitepvpers </a>"
                                "<br>Or add me on Discord: Pumbaa98#6817"));
}

void MainWindow::on_actionRename_triggered() {
    ui->treeWidget->editItem(ui->treeWidget->currentItem());
}

void MainWindow::itemEdited(QTreeWidgetItem *item, int column) {
    OnexTreeItem *e = static_cast<OnexTreeItem *>(item);
    e->setName(e->text(column));
    ui->treeWidget->sortItems(0, Qt::AscendingOrder);
    ui->treeWidget->scrollToItem(item);
}

void MainWindow::openFile(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;
    QByteArray header = file.read(0x0B);
    handleOpenResults(getOpener(header)->decrypt(file), path);
    file.close();
}

void MainWindow::handleOpenResults(OnexTreeItem *item, const QString &path) {
    if (item == nullptr)
        return;
    item->setData(0, Qt::UserRole, path);
    item->setFlags(item->flags() & (~Qt::ItemIsEditable));
    ui->treeWidget->addTopLevelItem(item);
    item->setExpanded(true);
}

INosFileOpener *MainWindow::getOpener(const QByteArray &header) {
    if (header.mid(0, 7) == "NT Data" || header.mid(0, 10) == "32GBS V1.0" || header.mid(0, 10) == "ITEMS V1.0")
        return &zlibOpener;
    else if (header.mid(0, 11) == "CCINF V1.20")
        return &ccinfOpener;
    return &textOpener;
}

template<typename TreeFunction>
void MainWindow::fileOperationOnSelectedItems(TreeFunction treeFunction, QString &defaultPath, QString operationName, bool saveDialog, QString filter) {
    QList<QTreeWidgetItem *> selectedItems = ui->treeWidget->selectedItems();
    if (selectedItems.empty()) {
        QMessageBox::information(nullptr, tr("Info"), tr("Select file first"));
        return;
    }

    QString path;
    int count = 0;
    if (selectedItems.size() == 1 && selectedItems.at(0)->childCount() == 0) {
        auto *item = dynamic_cast<OnexTreeItem *>(selectedItems.at((0)));
        if (saveDialog)
            path = getSaveFile(defaultPath + item->getName(), filter.isEmpty() ? item->getExportExtensionFilter() : filter);
        else
            path = getOpenFile(defaultPath + item->getName(), filter.isEmpty() ? item->getExportExtensionFilter() : filter);
    } else
        path = getSelectedDirectory(defaultPath);
    if (path.isEmpty())
        return;
    defaultPath = neatPath(path);

    for (auto &s: selectedItems) {
        auto *item = dynamic_cast<OnexTreeItem *>(s);
        auto callFunc = std::bind(treeFunction, item, path);
        count += callFunc();
    }

    QMessageBox::information(this, "End of operation", operationName + " " + QString::number(count) + " file(s).");
}

QString MainWindow::getSelectedDirectory(const QString &suggestion) {
    QString dir = QFileDialog::getExistingDirectory(nullptr, tr("Select Directory"), suggestion);
    if (dir.isEmpty())
        return dir;
    return dir + "/";
}

QString MainWindow::getOpenFile(const QString &suggestion, const QString &filter) {
    return QFileDialog::getOpenFileName(nullptr, tr("Open..."), suggestion, filter);
}

QStringList MainWindow::getOpenFiles(const QString &suggestion, const QString &filter) {
    QFileDialog openDialog(this);
    openDialog.setFileMode(QFileDialog::ExistingFiles);
    openDialog.setViewMode(QFileDialog::Detail);
    openDialog.setDirectory(suggestion);
    openDialog.setNameFilter(filter);
    QStringList selectedFiles;
    if (openDialog.exec())
        selectedFiles = openDialog.selectedFiles();
    return selectedFiles;
}

QString MainWindow::getSaveFile(const QString &suggestion, const QString &filter) {
    return QFileDialog::getSaveFileName(nullptr, tr("Save as..."), suggestion, filter);
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

OnexTreeItem *MainWindow::getTreeRoot() {
    if (!ui->treeWidget->currentItem())
        return nullptr;
    auto *item = dynamic_cast<OnexTreeItem *>(ui->treeWidget->currentItem());
    while (item->hasParent()) {
        item = dynamic_cast<OnexTreeItem *>(item->QTreeWidgetItem::parent());
    }
    return item;
}

QString MainWindow::neatPath(QString path) {
    return path.mid(0, path.lastIndexOf("/") + 1);
}
