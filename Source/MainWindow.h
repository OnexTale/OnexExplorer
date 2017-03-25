#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include "Ui/SingleTextFilePreview.h"
#include "Openers/NosTextOpener.h"
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

private:
    Ui::MainWindow *ui;
    NosTextOpener textOpener;

    void openFile(QString path);
    void handleOpenResults(OnexTreeItem* item);
};

#endif // MAINWINDOW_H
