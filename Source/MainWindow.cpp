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

    textOpener.decrypt(file);

    file.close();
}
