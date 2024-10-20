#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QStandardItemModel>
#include "advanced_indexer.h"




QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_lineEditUserInput_returnPressed();

    void on_listView_doubleClicked(const QModelIndex &index);

    void on_treeView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    std::string dir = "/Users/pav/Projects/FileExplorer/files/";
    QFileSystemModel *dir_model;
    QStandardItemModel *file_model;
    advancedIndexer* indexer = new advancedIndexer(10000);



};
#endif // MAINWINDOW_H
