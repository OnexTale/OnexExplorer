#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Openers/NosCCInfOpener.h"
#include "Openers/NosTextOpener.h"
#include "Openers/NosZlibOpener.h"
#include "Ui/Previews/SingleImagePreview.h"
#include "Ui/Previews/SingleTextFilePreview.h"
#include <QDebug>
#include <QDropEvent>
#include <QFileDialog>
#include <QMainWindow>
#include <QMimeData>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    const QString VERSION = "v0.5";


public slots:
    void filterItems();
    void onCustomMenuShow(const QPoint &point);
    void clearMenu();
    void on_actionOpen_triggered();
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *prev);
    void on_actionClose_selected_triggered();
    void on_actionExit_triggered();
    void on_actionClose_all_triggered();
    void on_actionReplace_triggered();
    void on_actionExport_triggered();
    void on_actionExport_to_raw_triggered();
    void on_actionReplace_with_raw_triggered();
    void on_actionAbout_triggered();
    void on_actionHelp_triggered();
    void on_actionSave_as_triggered();

private slots:
    void on_actionSave_triggered();

private:
    Ui::MainWindow *ui;
    NosTextOpener textOpener;
    NosZlibOpener zlibOpener;
    NosCCInfOpener ccinfOpener;
    QMenu *contextMenu = nullptr;

    void openFile(QString path);
    void handleOpenResults(OnexTreeItem *item, QString path);
    int hasValidHeader(QFile &file);

    QString getSelectedDirectory();

    virtual void dropEvent(QDropEvent *e) override;
    virtual void dragEnterEvent(QDragEnterEvent *e) override;
};

#endif // MAINWINDOW_H
