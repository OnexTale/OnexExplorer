#include "MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete ui;
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

    if (hasValidHeader(file))
        handleOpenResults(zlibOpener.decrypt(file));
    else
        handleOpenResults(textOpener.decrypt(file));

    file.close();
}

void MainWindow::handleOpenResults(OnexTreeItem *item)
{
    ui->treeWidget->addTopLevelItem(item);
    item->setExpanded(true);
}

bool MainWindow::hasValidHeader(QFile &file)
{
    file.seek(0);
    QByteArray header = file.read(0x0B);

    return (header.indexOf("NT Data") || header.indexOf("32GBS V1.0") || header.indexOf("ITEMS V1.0") /*|| header.indexOf("CCINF V1.20")*/);
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
    OnexTreeItem* item = static_cast<OnexTreeItem*>(treeItem);

    if (item->childCount() != 0)
        return;

    if (!item->hasParent())
        return;

    NosEnumTypes type = item->getType();
    QWidget* previewWindow = nullptr;

    switch (type) {
    case NOS_TEXT:
    {
        previewWindow = new SingleTextFilePreview(item->getContent(), ui->mdiArea);
        break;
    }
    case NOS_ARCHIVE:
    {
        int headerValue = item->getHeaderValue();
        if (headerValue == NSipData ||
                headerValue == NStpData ||
                headerValue == NStpeData ||
                headerValue == NStpuData ||
                headerValue == NStcData ||
                headerValue == NS4BbData ||
                headerValue == NSipData2006)
        {
            previewWindow = new SingleImagePreview(item->getContent(), headerValue, ui->mdiArea);
        }
        break;
    }
    default:
    {
        break;
    }
    }

    if (!previewWindow)
    {
        QMessageBox::warning(this, "Not supported", "This NosTale file cannot be opened yet.");
        return;
    }

    ui->mdiArea->addSubWindow(previewWindow);
    previewWindow->setAttribute(Qt::WA_DeleteOnClose);
    previewWindow->setWindowTitle(item->getName());
    previewWindow->show();
}

void MainWindow::on_actionClose_selected_triggered()
{
    QList<QTreeWidgetItem*> selectedItems = ui->treeWidget->selectedItems();
    foreach (auto& item, selectedItems)
        delete item;
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
