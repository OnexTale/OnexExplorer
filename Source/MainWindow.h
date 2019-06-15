#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QDropEvent>
#include <QMimeData>
#include "Ui/SingleTextFilePreview.h"
#include "Ui/FileInfo.h"
#include "Openers/NosTextOpener.h"
#include "Openers/NosZlibOpener.h"
#include "Openers/NosCCInfOpener.h"
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
    void onCustomMenuShow(const QPoint& point);
    void clearMenu();
    void on_actionOpen_triggered();

    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *prev);

    void on_actionClose_selected_triggered();

    void on_actionExit_triggered();

    void on_actionClose_all_triggered();

    void on_actionOptions_triggered();

    void on_actionReplace_triggered();

    void on_actionImport_triggered();

    void on_actionExport_triggered();

    void on_actionAbout_triggered();

    void on_actionSave_as_triggered();

private:
    Ui::MainWindow *ui;
    NosTextOpener textOpener;
    NosZlibOpener zlibOpener;
    NosCCInfOpener ccinfOpener;
    QMenu* contextMenu = nullptr;

    void openFile(QString path);
    void handleOpenResults(OnexTreeItem* item);
    int hasValidHeader(QFile& file);

    virtual void dropEvent(QDropEvent *e) override;
    virtual void dragEnterEvent(QDragEnterEvent *e) override;
};

#endif // MAINWINDOW_H
