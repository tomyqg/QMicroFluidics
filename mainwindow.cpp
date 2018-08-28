#include "mainwindow.h"
#include "newdesigndialog.h"
#include "configurationentity.h"
#include "pipescene.h"
#include "editorwidget.h"
#include "editorview.h"
#include <QApplication>
#include <QDebug>
#include <QMenuBar>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initMenu();

    scene = new PipeScene(this);
    dialog = new NewDesignDialog(this);
    editor = new EditorWidget(this);
    editor->getView()->setScene(scene);

    setCentralWidget(editor);

    setMinimumSize(500,500);
}

MainWindow::~MainWindow()
{

}

void MainWindow::initMenu(){
    QMenu* filemenu = menuBar()->addMenu(tr("&File"));

    QMenu* canvasmenu = menuBar()->addMenu(tr("&Canvas"));

    QAction* quitaction = filemenu->addAction(tr("&Quit"));

    QAction* newaction = canvasmenu->addAction(tr("&New"));
    QAction* clearaction = canvasmenu->addAction(tr("&Clear"));
    QAction* restoreaction = canvasmenu->addAction(tr("&Restore"));

    connect(newaction,&QAction::triggered,this,&MainWindow::createNewDesign);
    connect(clearaction,&QAction::triggered,this,&MainWindow::clearScene);
    connect(restoreaction,&QAction::triggered,this,&MainWindow::restoreScene);
    connect(quitaction,&QAction::triggered,qApp,&QApplication::quit);

    //dont know why this leads to SIGV
    //connect(clearaction,&QAction::triggered,scene,&QGraphicsScene::clear);

    //TODO : more entries
}

void MainWindow::createNewDesign(){
    dialog->clearAll();
    if (dialog->exec() == QDialog::Accepted) {
        if (dialog->getEntity()->checkValidity()) {
            scene->reset(dialog->getEntity());
            editor->resetView();
        } else {
            QMessageBox::warning(this,tr("Your Configuration is Invalid!"),tr("NOTE:\n"
                                                                              "1.size should be between %1 and %2\n"
                                                                              "2.input and output pipe index must not be identical and "
                                                                              "must be between 0 and SIZE\n").arg(PIPE_SIZE_MIN).arg(PIPE_SIZE_MAX));
        }
    }
#ifdef QT_DEBUG
    else {
        qDebug()<<"rejected";
    }
#endif
}

void MainWindow::clearScene(){
    scene->clear();
}

void MainWindow::restoreScene()
{
    scene->restore();
}
