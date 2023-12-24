#ifndef PROFESSOR_H
#define PROFESSOR_H

#include <QDialog>
#include <QtSql>
#include <QMainWindow>
#include <QCloseEvent>
#include <QSqlQueryModel>
#include <QPixmap>
#include <QByteArray>

namespace Ui {
class Professor;
}

class Professor : public QDialog
{
    Q_OBJECT

public:
    explicit Professor(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event) override;
    void addElement(QString, QString);
    ~Professor();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    QString username;
    QString password;
    Ui::Professor *ui;
    QSqlDatabase db;

};

class CustomSqlModel : public QSqlQueryModel
{
public:
    CustomSqlModel(QObject *parent = nullptr)
        : QSqlQueryModel(parent)
    {
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (role == Qt::DecorationRole && index.column() == yourImageColumnIndex) {
            // Извлекаем изображение из модели
            QByteArray imageData = record(index.row()).value(yourImageColumnName).toByteArray();

            // Возвращаем изображение в качестве значка
            return QPixmap::fromImage(QImage::fromData(imageData));
        }

        return QSqlQueryModel::data(index, role);
    }

private:
    int yourImageColumnIndex = 3; // Индекс колонки с изображением (подставьте свои данные)
    QString yourImageColumnName = "Image"; // Имя колонки с изображением (подставьте свои данные)
};

#endif // PROFESSOR_H
