#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "process_file.h"
#include <filesystem>
#include "openai.h"
#include <QStringListModel>

namespace fs = std::filesystem;



void processPath(fs::path p, advancedIndexer* indexer) {
    if (fs::is_directory(p)) {
        for (const fs::directory_entry& x : fs::directory_iterator{p}) {
            processPath(x, indexer);
        }
    } else {
        if ((!indexer->checkIfInGraph(p))){
            processFile(p, indexer);
        }
    }
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dir_model = new QFileSystemModel(this);
    dir_model->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    QModelIndex dIndex = dir_model->setRootPath(QString::fromStdString(dir));
    ui->treeView->setModel(dir_model);
    ui->treeView->setRootIndex(dIndex);

    file_model = new QStandardItemModel(this);
    ui->listView->setModel(file_model);


    indexer->load("indexed.bin");
    processPath(dir,indexer);

    // indexer->save("indexed.bin");

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    file_model->clear();
    fs::path clicked_path = dir_model->fileInfo(index).absoluteFilePath().toStdString();

    for (const fs::path &p : fs::directory_iterator{clicked_path}){
        std::string extension = p.extension();
        if ((extension == ".png") || (extension == ".jpg")){
            QStandardItem *item = new QStandardItem("image");
            item->setData(QString::fromStdString(p));
            item->setEditable(false);
            file_model->appendRow(item);
        }
    }

}


void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    QString clicked_path = file_model->itemFromIndex(index)->data().toString();
    QPixmap pixmap(clicked_path);
    ui->label->setPixmap(pixmap);
}


// streamline this method
void MainWindow::on_lineEditUserInput_returnPressed()
{
    std::string user_input = ui->lineEditUserInput->text().toStdString();
    std::vector<float> user_input_embedding = getEmbedding(user_input);
    int closest_n = 1;

    file_model->clear();
    std::unordered_set<std::string> added_items;

    while (closest_n <= indexer->getPathsStored()) {
        std::vector<match> closest_nodes = indexer->getClosestFromGraph(user_input_embedding, closest_n);

        bool too_large = false;

        for (const match& match : closest_nodes) {
            std::cout << match.first << match.second << std::endl;
            if (match.second >= 1) {
                too_large = true;
                break;
            }

            if (added_items.find(match.first) == added_items.end()) {
                QStandardItem *item = new QStandardItem("image");
                item->setData(QString::fromStdString(match.first));
                item->setEditable(false);
                file_model->appendRow(item);
                added_items.insert(match.first);
            }
        }

        if (too_large) {
            break;
        }
        closest_n *= 2;
    }
}
