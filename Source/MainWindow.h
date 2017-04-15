#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include "Ui/SingleTextFilePreview.h"
#include "Openers/NosTextOpener.h"
#include "Openers/NosZlibOpener.h"
#include "Ui/SingleImagePreview.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *treeItem, int column);

    void on_actionClose_selected_triggered();

    void on_actionExit_triggered();

    void on_actionClose_all_triggered();

private:
    Ui::MainWindow *ui;
    NosTextOpener textOpener;
    NosZlibOpener zlibOpener;

    void openFile(QString path);
    void handleOpenResults(OnexTreeItem* item);
    bool hasNTHeader(QFile& file);
    bool hasGBSHeader(QFile& file);
};

#endif // MAINWINDOW_H
