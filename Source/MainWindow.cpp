#include "MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog openDialog(this);

    openDialog.setFileMode(QFileDialog::ExistingFile);
    openDialog.setNameFilter(tr("NosTale Files (*.NOS)"));
    openDialog.setViewMode(QFileDialog::Detail);

    QStringList selectedFiles;
    if (openDialog.exec())
        selectedFiles = openDialog.selectedFiles();

    if (!selectedFiles.empty())
        openFile(selectedFiles.first());
}

void MainWindow::openFile(QString path)
{
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly))
        return;

    if (hasNTHeader(file))
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

bool MainWindow::hasNTHeader(QFile &file)
{
    file.seek(0);
    QByteArray header = file.read(7);

    return (header == "NT Data");
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
        if (headerValue == NSipData || headerValue == NStpData || headerValue == NStpeData || headerValue == NStpuData)
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
    previewWindow->show();
}

void MainWindow::on_actionClose_selected_triggered()
{
    int currentIndex = ui->treeWidget->currentColumn();
    if (QTreeWidgetItem* item = ui->treeWidget->takeTopLevelItem(currentIndex))
        delete item;
}
