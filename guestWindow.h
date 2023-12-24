#ifndef GUESTWINDOW_H
#define GUESTWINDOW_H

#include <QDialog>
#include <QtSql>
#include <QMainWindow>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QMessageBox>
#include <QScrollArea>
#include <QInputDialog>
#include <QSqlQueryModel>

namespace Ui {
class guestWindow;
}

class guestWindow : public QDialog
{
    Q_OBJECT

public:
    explicit guestWindow(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event) override;
    ~guestWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::guestWindow *ui;
    QSqlDatabase db;
};

#endif // GUESTWINDOW_H
