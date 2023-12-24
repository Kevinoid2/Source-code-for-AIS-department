#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/database/database.db3");

    if (db.open()) {
        qDebug() << "Открылась";
    }
    else{
        qDebug() << "Ошибка при открытии базы данных:" << db.lastError().text();
    }

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    hide();
    gWindow = new guestWindow(this);
    gWindow->show();
}


void MainWindow::on_pushButton_2_clicked()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/database/database.db3");
    db.open();

    QString username = ui->userEdit->text();
    QString password = ui->passwordEdit->text();

    QSqlQuery query;
    query.prepare("SELECT * FROM Administrator WHERE Логин = :username AND Пароль = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        // В результате запроса есть хотя бы одна запись
        auntification = true; // Верные логин и пароль
        qDebug() << "Вход произошел" << username;

        hide();
        aWindow = new adminWindow(this);
        aWindow->addElement(username, password);
        aWindow->show();
    }
    else{
        query.prepare("SELECT * FROM Professor WHERE Email = :username AND Пароль = :password");
        query.bindValue(":username", username);
        query.bindValue(":password", password);
        if (query.exec() && query.next()) {
            // В результате запроса есть хотя бы одна запись
            auntification = true; // Верные логин и пароль
            qDebug() << "Вход произошел" << username;

            hide();
            pWindow = new Professor(this);
            pWindow->addElement(username, password);
            pWindow->show();
        }
        else{
        // Нет записей в результатах запроса или ошибка при выполнении запроса
        auntification = false; // Неверный логин или пароль
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль. Пожалуйста, повторите попытку.");
        qDebug() << "Неверный логин или пароль" << username;
        qDebug() << "Неверный логин или пароль" << password;
        qDebug() << "Последний выполненный запрос:" << query.lastQuery();
        qDebug() << "Ошибка запроса:" << query.lastError().text();
        }
    }
    db.close();
}


