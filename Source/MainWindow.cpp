#include "MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setAcceptDrops(true);

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onCustomMenuShow(QPoint)));
}

MainWindow::~MainWindow()
{
    if (contextMenu)
    {
        clearMenu();
        delete contextMenu;
    }

    delete ui;
}

void MainWindow::onCustomMenuShow(const QPoint &point)
{
    Q_UNUSED(point);

    if (contextMenu)
    {
        clearMenu();
        delete contextMenu;
        contextMenu = nullptr;
    }
    OnexTreeItem* item = static_cast<OnexTreeItem*>(ui->treeWidget->currentItem());
    if (item == nullptr)
        return;

    contextMenu = item->getContextMenu();

    if (contextMenu->isEmpty())
        return;
    qDebug() << contextMenu->height();
    contextMenu->exec(QCursor::pos());
}

void MainWindow::clearMenu()
{
    qDebug() << "Disposing menu";
    QList<QAction*> actions = contextMenu->actions();

    for (auto& action : actions)
        delete action;

    contextMenu->clear();
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog openDialog(this);

    openDialog.setFileMode(QFileDialog::ExistingFiles);
    openDialog.setNameFilter(tr("NosTale Files (*.NOS)"));
    openDialog.setViewMode(QFileDialog::Detail);

    QStringList selectedFiles;
    if (openDialog.exec())
        selectedFiles = openDialog.selectedFiles();

    if (!selectedFiles.empty())
    {
        for (auto& file : selectedFiles)
                openFile(file);
    }
}

void MainWindow::openFile(QString path)
{
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly))
        return;

    if (hasValidHeader(file) == 1)
        handleOpenResults(zlibOpener.decrypt(file));
    else if (hasValidHeader(file) == 2)
        handleOpenResults(ccinfOpener.decrypt(file));
    else
        handleOpenResults(textOpener.decrypt(file));

    file.close();
}

void MainWindow::handleOpenResults(OnexTreeItem *item)
{
    ui->treeWidget->addTopLevelItem(item);
    item->setExpanded(true);
}

int MainWindow::hasValidHeader(QFile &file)
{
    file.seek(0);
    QByteArray header = file.read(0x0B);
    if (header.mid(0, 7) == "NT Data" || header.mid(0, 10) == "32GBS V1.0" || header.mid(0, 10) == "ITEMS V1.0")
        return 1;
    else if (header.mid(0, 11) == "CCINF V1.20")
        return 2;
    return 0;
}

void MainWindow::dropEvent(QDropEvent *e)
{
    for (auto& url : e->mimeData()->urls())
    {
        QString fileName = url.toLocalFile();
        openFile(fileName);
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    for (auto& url : e->mimeData()->urls())
    {
        QString fileName = url.toLocalFile();
        if (QFileInfo(fileName).suffix() != "NOS")
            return;
    }
    e->acceptProposedAction();
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *treeItem, int column)
{
    Q_UNUSED(column);

    OnexTreeItem* item = static_cast<OnexTreeItem*>(treeItem);

    if (item->childCount() != 0)
        return;

    if (!item->hasParent())
        return;

    QWidget* previewWindow = item->onClicked();


    if (!previewWindow)
    {
        QMessageBox::warning(this, "Not supported", "This NosTale file cannot be opened yet.");
        return;
    }

    ui->mdiArea->addSubWindow(previewWindow);
    previewWindow->setAttribute(Qt::WA_DeleteOnClose);
    previewWindow->show();
}

void MainWindow::on_actionClose_selected_triggered()
{
    QList<QTreeWidgetItem*> selectedItems = ui->treeWidget->selectedItems();
    foreach (auto& item, selectedItems)
        delete item;
}

void MainWindow::on_actionReplace_triggered()
{
    if(ui->treeWidget->currentItem()){

        OnexTreeItem* item = static_cast<OnexTreeItem*>(ui->treeWidget->currentItem());
        if(item->hasParent()){
        }else{
            QMessageBox::information(NULL, tr("Info"), tr("Select correct file not *.NOS"));
        }
    }else{
        QMessageBox::information(NULL, tr("Info"), tr("Select file first"));
    }
}
void MainWindow::on_actionExport_triggered()
{
    if(ui->treeWidget->currentItem()){

        OnexTreeItem* item = static_cast<OnexTreeItem*>(ui->treeWidget->currentItem());
        if(item->hasParent()){

            item->onExportSingle();
        }else{
            QMessageBox::information(NULL, tr("Info"), tr("Select correct file not *.NOS"));
        }
    }else{
        QMessageBox::information(NULL, tr("Info"), tr("Select file first"));
    }
}
void MainWindow::on_actionImport_triggered()
{
    QMessageBox::warning(NULL, tr("Not yet"), tr("This isn't implemented yet"));
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(NULL, tr("About Project"), tr("OnexExplorer is an open-source tool for unpacking and repacking .NOS data files from game called NosTale. "
                                                     "<br>It can open almost all .NOS files and show the data stored in them."
                                                     "<br>GitHub: <a href='https://github.com/OnexTale/OnexExplorer'>https://github.com/OnexTale/OnexExplorer</a>"));
}

void MainWindow::on_actionSave_as_triggered()
{
    if(ui->treeWidget->currentItem()){

        OnexTreeItem* item = static_cast<OnexTreeItem*>(ui->treeWidget->currentItem()->parent());
        if(!item->hasParent()){
            item->onExporAsOriginal();
        }else{
            QMessageBox::information(NULL, tr("Info"), tr("Select correct *.NOS file"));
        }
    }else{
        QMessageBox::information(NULL, tr("Info"), tr("Select .NOS file first"));
    }
}

void MainWindow::on_actionExit_triggered()
{
    QMessageBox::StandardButton message = QMessageBox::question(this, "",
                                                                "Exit program?",
                                                                QMessageBox::Yes | QMessageBox::No,
                                                                QMessageBox::No);
        if (message == QMessageBox::Yes)
            QApplication::quit();
}

void MainWindow::on_actionClose_all_triggered()
{
    QMessageBox::StandardButton message = QMessageBox::question(this, "",
                                                                "Close all items?",
                                                                QMessageBox::Yes | QMessageBox::No,
                                                                QMessageBox::No);

    if (message == QMessageBox::Yes)
    {
        ui->treeWidget->clear();
    }
}

void MainWindow::on_actionOptions_triggered()
{
    OnexTreeItem* item = static_cast<OnexTreeItem*>(ui->treeWidget->currentItem());
    if (item == nullptr)
        return;
    if (item->childCount() != 0)
        return;
    FileInfo dialog(item);
    dialog.exec();
}

void MainWindow::on_actionClose_windows_triggered()
{
    ui->mdiArea->closeAllSubWindows();
}
