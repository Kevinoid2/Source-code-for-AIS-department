#include "professor.h"
#include "ui_professor.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QMessageBox>

Professor::Professor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Professor)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/database/database.db3");
    db.open();

    ui->setupUi(this);
}

Professor::~Professor()
{
    delete ui;
}

void Professor::closeEvent(QCloseEvent *event)
{
    // Вызываем closeEvent базового класса
    QWidget::closeEvent(event);

    // Показываем родительское окно после закрытия окна профессора
    if (parentWidget()) {
        parentWidget()->show();
    }
}

void Professor::addElement(QString _First_name, QString _passwoed){
    username = _First_name;
    password = _passwoed;
    qDebug() << "Username:" << username;
    qDebug() << "Password:" << password;
    // Создаем модель для запроса
    QSqlQueryModel *model = new QSqlQueryModel(this);

    // Выполняем запрос к базе данных
    QSqlQuery query;
    query.prepare("SELECT Class.Название_занятия, Class.Аудитория, Class.Группа, "
                  "ClassType.Название_типа_занятия, WeekType.Номер_недели, "
                  "Class.День_недели, ClassTime.Начало, ClassTime.Конец "
                  "FROM Class "
                  "JOIN ID ON Class.ID_занятие = ID.ID_занятие "
                  "JOIN Professor ON Professor.ID_преподаватель = ID.ID_преподаватель "
                  "JOIN ClassTime ON Class.ID_время_занятия = ClassTime.ID_время_занятия "
                  "JOIN ClassType ON Class.ID_тип_занятия = ClassType.ID_тип_занятия "
                  "JOIN WeekType ON Class.ID_тип_недели = WeekType.ID_тип_недели "
                  "WHERE Professor.Email = :username AND Professor.Пароль = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    query.exec();
    qDebug() << "Query executed:" << query.exec();
    if (!query.exec()) {
        qDebug() << "Query execution failed:" << query.lastError().text();
    }

    model->setQuery(query);
    // Добавляем виджет таблицы в вертикальный лейаут
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    // Установка режима ResizeToContents для первой колонки и второй
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableView->show();
    db.close();
}

void Professor::on_pushButton_clicked()
{
// Создаем новое диалоговое окно
    QDialog editDialog(this);
    editDialog.setWindowTitle("Редактирование данных преподавателя");

    // Поля для ввода данных
    QLineEdit *emailEdit = new QLineEdit(&editDialog);
    QLineEdit *addressEdit = new QLineEdit(&editDialog);
    QLineEdit *phoneNumberEdit = new QLineEdit(&editDialog);
    QLineEdit *scienceDegreeEdit = new QLineEdit(&editDialog);

    // Добавляем кнопки "Сохранить" и "Отмена"
    QPushButton *saveButton = new QPushButton("Сохранить", &editDialog);
    QPushButton *cancelButton = new QPushButton("Отмена", &editDialog);

    // Создаем слоты для кнопок
    connect(saveButton, &QPushButton::clicked, &editDialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &editDialog, &QDialog::reject);

    // Создаем вертикальный лейаут и добавляем элементы управления
    QVBoxLayout *layout = new QVBoxLayout(&editDialog);
    layout->addWidget(new QLabel("Email:", &editDialog));
    layout->addWidget(emailEdit);
    layout->addWidget(new QLabel("Адрес:", &editDialog));
    layout->addWidget(addressEdit);
    layout->addWidget(new QLabel("Телефон:", &editDialog));
    layout->addWidget(phoneNumberEdit);
    layout->addWidget(new QLabel("Ученая степень:", &editDialog));
    layout->addWidget(scienceDegreeEdit);

    layout->addWidget(saveButton);
    layout->addWidget(cancelButton);

    // Подключение к базе данных SQLite
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/database/database.db3");

    if (!db.open()) {
        qDebug() << "Ошибка при открытии базы данных:" << db.lastError().text();
        return;
    }

    // Подготовка SQL-запроса для получения информации о преподавателе
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT * FROM Professor WHERE Email = :username AND Пароль = :password");
    selectQuery.bindValue(":username", username);
    selectQuery.bindValue(":password", password);

    // Выполнение SQL-запроса для получения данных о преподавателе
    if (selectQuery.exec() && selectQuery.next()) {
        // Получение значений из результата запроса
        emailEdit->setText(selectQuery.value("Email").toString());
        addressEdit->setText(selectQuery.value("Адрес_проживания").toString());
        phoneNumberEdit->setText(selectQuery.value("Номер_телефона").toString());
        scienceDegreeEdit->setText(selectQuery.value("Учёная_степень").toString());
    } else {
        qDebug() << "Ошибка при выполнении запроса для получения данных преподавателя:" << selectQuery.lastError().text();
    }

    // Добавляем QLabel для отображения текста в нижней части диалога
    QLabel *infoLabel = new QLabel("<font color='red'>Если вы хотите изменить имя, фамилию или отчество, обратитесь к администратору базы данных.</font>", &editDialog);
    layout->addWidget(infoLabel, 0, Qt::AlignBottom); // Устанавливаем QLabel в нижней части диалога

    // Показываем диалоговое окно
    if (editDialog.exec() == QDialog::Accepted) {
        // Здесь добавьте логику сохранения данных в базе данных
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE Professor SET Email = :email, Адрес_проживания = :address, "
                            "Номер_телефона = :phone_number, Учёная_степень = :science_degree "
                            "WHERE Email = :username AND Пароль = :password");
        updateQuery.bindValue(":email", emailEdit->text());
        updateQuery.bindValue(":address", addressEdit->text());
        updateQuery.bindValue(":phone_number", phoneNumberEdit->text());
        updateQuery.bindValue(":science_degree", scienceDegreeEdit->text());
        updateQuery.bindValue(":username", username);
        updateQuery.bindValue(":password", password);

        if (updateQuery.exec()) {
            username = emailEdit->text();
            qDebug() << "Данные успешно обновлены";
        } else {
            qDebug() << "Ошибка при обновлении данных:" << updateQuery.lastError().text();
        }
    }

    // Закрытие соединения с базой данных
    db.close();
}


void Professor::on_pushButton_2_clicked()
{
// Создаем новое диалоговое окно
    QDialog editDialog(this);
    editDialog.setWindowTitle("Изменение пароля");

    // Поля для ввода данных
    QLineEdit *oldPasswordEdit = new QLineEdit(&editDialog);
    QLineEdit *newPasswordEdit = new QLineEdit(&editDialog);
    QLineEdit *confirmPasswordEdit = new QLineEdit(&editDialog);

    // Устанавливаем типы полей ввода для паролей
    oldPasswordEdit->setEchoMode(QLineEdit::Password);
    newPasswordEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);

    // Добавляем кнопки "Сохранить" и "Отмена"
    QPushButton *saveButton = new QPushButton("Сохранить", &editDialog);
    QPushButton *cancelButton = new QPushButton("Отмена", &editDialog);

    // Создаем слоты для кнопок
    connect(saveButton, &QPushButton::clicked, &editDialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &editDialog, &QDialog::reject);

    // Создаем вертикальный лейаут и добавляем элементы управления
    QVBoxLayout *layout = new QVBoxLayout(&editDialog);
    layout->addWidget(new QLabel("Старый пароль:", &editDialog));
    layout->addWidget(oldPasswordEdit);
    layout->addWidget(new QLabel("Новый пароль:", &editDialog));
    layout->addWidget(newPasswordEdit);
    layout->addWidget(new QLabel("Повторите новый пароль:", &editDialog));
    layout->addWidget(confirmPasswordEdit);

    layout->addWidget(saveButton);
    layout->addWidget(cancelButton);

    // Подключение к базе данных SQLite
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/database/database.db3");

    if (!db.open()) {
        qDebug() << "Ошибка при открытии базы данных:" << db.lastError().text();
        return;
    }

    // Показываем диалоговое окно
    if (editDialog.exec() == QDialog::Accepted) {
        // Получаем старый пароль после отображения диалогового окна
        QString oldPassword = oldPasswordEdit->text();

        // Проверяем соответствие старого пароля
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT * FROM Professor WHERE Email = :username AND Пароль = :password");
        checkQuery.bindValue(":username", username);
        checkQuery.bindValue(":password", oldPassword);

        if (!checkQuery.exec() || !checkQuery.next()) {
            // Неправильный старый пароль
            QMessageBox::warning(&editDialog, "Ошибка", "Неправильный старый пароль. Пожалуйста, повторите попытку.");
            db.close();
            return;
        }

        // Здесь добавьте логику изменения пароля в базе данных
        QString newPassword = newPasswordEdit->text();
        QString confirmedPassword = confirmPasswordEdit->text();

        if (newPassword != confirmedPassword) {
            // Пароли не совпадают
            QMessageBox::warning(&editDialog, "Ошибка", "Новый пароль и подтверждение пароля не совпадают. Пожалуйста, повторите попытку.");
            db.close();
            return;
        }

        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE Professor SET Пароль = :new_password WHERE Email = :username");
        updateQuery.bindValue(":new_password", newPassword);
        updateQuery.bindValue(":username", username);

        if (updateQuery.exec()) {
            qDebug() << "Пароль успешно изменен";
        } else {
            qDebug() << "Ошибка при изменении пароля:" << updateQuery.lastError().text();
        }
    }

    // Закрытие соединения с базой данных
    db.close();
}

