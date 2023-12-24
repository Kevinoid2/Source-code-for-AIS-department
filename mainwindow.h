#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include "professor.h"
#include "adminwindow.h"
#include "guestWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    Professor *pWindow;
    adminWindow *aWindow;
    guestWindow *gWindow;
    bool auntification = false;
    QSqlDatabase db;
};
#endif // MAINWINDOW_H
