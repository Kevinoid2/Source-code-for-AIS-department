#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QDialog>
#include <QtSql>
#include <QMainWindow>
#include <QCloseEvent>
#include <QSqlQueryModel>
#include <QPixmap>
#include <QSqlRecord>
#include <QSqlField>

namespace Ui {
class adminWindow;
}

class adminWindow : public QDialog
{
    Q_OBJECT

public:
    explicit adminWindow(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event) override;
    void addElement(QString, QString);
    ~adminWindow();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::adminWindow *ui;
    QSqlDatabase db;
    QString username;
    QString password;
};

class ImageSqlQueryModel : public QSqlQueryModel
{
public:
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (role == Qt::DecorationRole && index.column() == imageColumnIndex) {
            // Если столбец - столбец с изображением, возвращаем изображение в качестве роли Qt::DecorationRole
            QByteArray imageData = QSqlQueryModel::data(index, Qt::DisplayRole).toByteArray();
            QPixmap image;
            image.loadFromData(imageData);
            return image.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }

        // Иначе используем базовую реализацию
        return QSqlQueryModel::data(index, role);
    }

    // Установка индекса столбца с изображением
    void setImageColumnIndex(int columnIndex)
    {
        imageColumnIndex = columnIndex;
    }

private:
    int imageColumnIndex = -1;  // Индекс столбца с изображением
};

#endif // ADMINWINDOW_H
