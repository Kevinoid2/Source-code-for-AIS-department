#include "adminwindow.h"
#include "ui_adminwindow.h"
#include <QtWidgets>
#include <QDebug>

adminWindow::adminWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adminWindow)
{
    ui->setupUi(this);
}

adminWindow::~adminWindow()
{
    delete ui;
}

void adminWindow::closeEvent(QCloseEvent *event)
{
    // Вызываем closeEvent базового класса
    QWidget::closeEvent(event);

    // Показываем родительское окно после закрытия окна профессора
    if (parentWidget()) {
        parentWidget()->show();
    }
}

void adminWindow::addElement(QString _username, QString _password){
    username = _username;
    password = _password;
}

void adminWindow::on_pushButton_2_clicked()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/database/database.db3");
    db.open();
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM Administrator");

    if (model->lastError().isValid()) {
        // Обработка ошибки выполнения запроса
        qDebug() << "Ошибка выполнения запроса:" << model->lastError().text();
                QMessageBox::warning(this, "Ошибка", "Ошибка выполнения запроса. Пожалуйста, повторите попытку.");
        db.close();
        return;
    }

    QTableView *tableView = new QTableView;
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->show();
    db.close();
}


void adminWindow::on_pushButton_3_clicked()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/database/database.db3");
    db.open();

    ImageSqlQueryModel *model = new ImageSqlQueryModel;
    model->setQuery("SELECT * FROM Professor");

    if (model->lastError().isValid()) {
        // Обработка ошибки выполнения запроса
        qDebug() << "Ошибка выполнения запроса:" << model->lastError().text();
                QMessageBox::warning(this, "Ошибка", "Ошибка выполнения запроса. Пожалуйста, повторите попытку.");
        db.close();
        return;
    }

    // Устанавливаем индекс столбца с изображением
    model->setImageColumnIndex(model->record().indexOf("Image"));

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->show();

    db.close();
}



void adminWindow::on_pushButton_4_clicked()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/database/database.db3");
    db.open();

    // Создаем новое диалоговое окно
    QDialog addAdminDialog(this);
    addAdminDialog.setWindowTitle("Добавление нового администратора");

    // Поля для ввода данных
    QLineEdit *loginEdit = new QLineEdit(&addAdminDialog);
    QLineEdit *passwordEdit = new QLineEdit(&addAdminDialog);

    // Устанавливаем типы полей ввода для пароля
    passwordEdit->setEchoMode(QLineEdit::Password);

    // Добавляем кнопки "Добавить" и "Отмена"
    QPushButton *addButton = new QPushButton("Добавить", &addAdminDialog);
    QPushButton *cancelButton = new QPushButton("Отмена", &addAdminDialog);

    // Создаем слоты для кнопок
    connect(addButton, &QPushButton::clicked, &addAdminDialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &addAdminDialog, &QDialog::reject);

    // Создаем вертикальный лейаут и добавляем элементы управления
    QVBoxLayout *layout = new QVBoxLayout(&addAdminDialog);
    layout->addWidget(new QLabel("Логин:", &addAdminDialog));
    layout->addWidget(loginEdit);
    layout->addWidget(new QLabel("Пароль:", &addAdminDialog));
    layout->addWidget(passwordEdit);
    layout->addWidget(addButton);
    layout->addWidget(cancelButton);

    // Показываем диалоговое окно
    if (addAdminDialog.exec() == QDialog::Accepted) {
        // Здесь добавьте логику добавления нового администратора в базу данных
        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO Administrator (Логин, Пароль) VALUES (:login, :password)");
        insertQuery.bindValue(":login", loginEdit->text());
        insertQuery.bindValue(":password", passwordEdit->text());

        if (insertQuery.exec()) {
            qDebug() << "Новый администратор успешно добавлен";
        } else {
            qDebug() << "Ошибка при добавлении администратора:" << insertQuery.lastError().text();
        }
    }
    db.close();
}




void adminWindow::on_pushButton_5_clicked()
{
db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/database/database.db3");
    db.open();

    // Создаем новое диалоговое окно
    QDialog addProfessorDialog(this);
    addProfessorDialog.setWindowTitle("Добавление нового преподавателя");

    // Поля для ввода данных
    QLineEdit *firstNameEdit = new QLineEdit(&addProfessorDialog);
    QLineEdit *lastNameEdit = new QLineEdit(&addProfessorDialog);
    QLineEdit *middleNameEdit = new QLineEdit(&addProfessorDialog);
    QLineEdit *emailEdit = new QLineEdit(&addProfessorDialog);
    QLineEdit *passwordEdit = new QLineEdit(&addProfessorDialog);
    QLineEdit *addressEdit = new QLineEdit(&addProfessorDialog);
    QLineEdit *phoneNumberEdit = new QLineEdit(&addProfessorDialog);
    QLineEdit *birthdateEdit = new QLineEdit(&addProfessorDialog);
    QLineEdit *scienceDegreeEdit = new QLineEdit(&addProfessorDialog);

    // Добавляем поле для выбора изображения
    QLineEdit *imagePathEdit = new QLineEdit(&addProfessorDialog);
    QPushButton *browseButton = new QPushButton("Обзор", &addProfessorDialog);

    // Устанавливаем типы полей ввода для пароля и даты
    passwordEdit->setEchoMode(QLineEdit::Password);
    birthdateEdit->setPlaceholderText("ГГГГ-ММ-ДД");

    // Добавляем кнопки "Добавить" и "Отмена"
    QPushButton *addButton = new QPushButton("Добавить", &addProfessorDialog);
    QPushButton *cancelButton = new QPushButton("Отмена", &addProfessorDialog);

    // Создаем слот для кнопки обзора
    connect(browseButton, &QPushButton::clicked, [&]() {
        QString imagePath = QFileDialog::getOpenFileName(&addProfessorDialog, "Выберите изображение", "", "Images (*.png *.jpg *.bmp)");
        if (!imagePath.isEmpty()) {
            imagePathEdit->setText(imagePath);
        }
    });

    // Создаем слоты для кнопок
    connect(addButton, &QPushButton::clicked, &addProfessorDialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &addProfessorDialog, &QDialog::reject);

    // Создаем вертикальный лейаут и добавляем элементы управления
    QVBoxLayout *layout = new QVBoxLayout(&addProfessorDialog);
    layout->addWidget(new QLabel("Имя:", &addProfessorDialog));
    layout->addWidget(firstNameEdit);
    layout->addWidget(new QLabel("Фамилия:", &addProfessorDialog));
    layout->addWidget(lastNameEdit);
    layout->addWidget(new QLabel("Отчество:", &addProfessorDialog));
    layout->addWidget(middleNameEdit);
    layout->addWidget(new QLabel("Email:", &addProfessorDialog));
    layout->addWidget(emailEdit);
    layout->addWidget(new QLabel("Пароль:", &addProfessorDialog));
    layout->addWidget(passwordEdit);
    layout->addWidget(new QLabel("Адрес:", &addProfessorDialog));
    layout->addWidget(addressEdit);
    layout->addWidget(new QLabel("Телефон:", &addProfessorDialog));
    layout->addWidget(phoneNumberEdit);
    layout->addWidget(new QLabel("Дата рождения (ГГГГ-ММ-ДД):", &addProfessorDialog));
    layout->addWidget(birthdateEdit);
    layout->addWidget(new QLabel("Ученая степень:", &addProfessorDialog));
    layout->addWidget(scienceDegreeEdit);

    // Добавляем элементы для изображения
    layout->addWidget(new QLabel("Изображение:", &addProfessorDialog));
    QHBoxLayout *imageLayout = new QHBoxLayout;
    imageLayout->addWidget(imagePathEdit);
    imageLayout->addWidget(browseButton);
    layout->addLayout(imageLayout);

    layout->addWidget(addButton);
    layout->addWidget(cancelButton);

    // Показываем диалоговое окно
    if (addProfessorDialog.exec() == QDialog::Accepted) {
        // Здесь добавьте логику добавления нового преподавателя в базу данных
        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO Professor (Имя, Фамилия, Отчество, Email, Пароль, Адрес_проживания, Номер_телефона, Дата_рождения, Учёная_степень, Image) "
                            "VALUES (:first_name, :last_name, :middle_name, :email, :password, :address, :phone_number, :birthdate, :science_degree, :image)");

        insertQuery.bindValue(":first_name", firstNameEdit->text());
        insertQuery.bindValue(":last_name", lastNameEdit->text());
        insertQuery.bindValue(":middle_name", middleNameEdit->text());
        insertQuery.bindValue(":email", emailEdit->text());
        insertQuery.bindValue(":password", passwordEdit->text());
        insertQuery.bindValue(":address", addressEdit->text());
        insertQuery.bindValue(":phone_number", phoneNumberEdit->text());
        insertQuery.bindValue(":birthdate", birthdateEdit->text());
        insertQuery.bindValue(":science_degree", scienceDegreeEdit->text());

        QString imagePath = imagePathEdit->text();
        if (!imagePath.isEmpty()) {
            QImage image(imagePath);
            QByteArray imageData;
            QBuffer buffer(&imageData);
            buffer.open(QIODevice::WriteOnly);
            image.save(&buffer, "PNG");  // Можете использовать другой формат

            insertQuery.bindValue(":image", imageData);
        } else {
            insertQuery.bindValue(":image", QVariant(QVariant::ByteArray));
        }

        if (insertQuery.exec()) {
            qDebug() << "Новый преподаватель успешно добавлен";
        } else {
            qDebug() << "Ошибка при добавлении преподавателя:" << insertQuery.lastError().text();
        }
    }

    db.close();
}

void adminWindow::on_pushButton_6_clicked()
{
// Открываем соединение с базой данных
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/database/database.db3");
    if (!db.open()) {
        qDebug() << "Не удалось открыть базу данных";
        return;
    }

    // Создаем новое диалоговое окно выбора преподавателя
    QDialog selectProfessorDialog(this);
    selectProfessorDialog.setWindowTitle("Выберите преподавателя");

    // Создаем модель для отображения списка преподавателей
    QSqlQueryModel *professorModel = new QSqlQueryModel;
    professorModel->setQuery("SELECT Email FROM Professor");

    // Создаем выпадающий список для выбора преподавателя
    QComboBox *professorComboBox = new QComboBox(&selectProfessorDialog);
    professorComboBox->setModel(professorModel);

    // Добавляем кнопки "Выбрать" и "Отмена"
    QPushButton *selectButton = new QPushButton("Выбрать", &selectProfessorDialog);
    QPushButton *cancelButton = new QPushButton("Отмена", &selectProfessorDialog);

    // Создаем слоты для кнопок
    connect(selectButton, &QPushButton::clicked, &selectProfessorDialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &selectProfessorDialog, &QDialog::reject);

    // Создаем вертикальный лейаут и добавляем элементы управления
    QVBoxLayout *layout = new QVBoxLayout(&selectProfessorDialog);
    layout->addWidget(new QLabel("Выберите преподавателя:", &selectProfessorDialog));
    layout->addWidget(professorComboBox);
    layout->addWidget(selectButton);
    layout->addWidget(cancelButton);

    // Показываем диалоговое окно выбора преподавателя
    if (selectProfessorDialog.exec() == QDialog::Accepted) {
        // Получаем выбранного преподавателя
        QString selectedEmail = professorComboBox->currentText();

        // Создаем новое диалоговое окно для изменения данных преподавателя
        QDialog editProfessorDialog(this);
        editProfessorDialog.setWindowTitle("Редактирование данных преподавателя");

        // Поля для ввода данных, аналогичные тем, что использовались ранее
        QLineEdit *firstNameEdit = new QLineEdit(&editProfessorDialog);
        QLineEdit *lastNameEdit = new QLineEdit(&editProfessorDialog);
        QLineEdit *middleNameEdit = new QLineEdit(&editProfessorDialog);
        QLineEdit *emailEdit = new QLineEdit(&editProfessorDialog);
        QLineEdit *addressEdit = new QLineEdit(&editProfessorDialog);
        QLineEdit *phoneNumberEdit = new QLineEdit(&editProfessorDialog);
        QLineEdit *birthdateEdit = new QLineEdit(&editProfessorDialog);
        QLineEdit *scienceDegreeEdit = new QLineEdit(&editProfessorDialog);

        // QLabel для отображения изображения
        QLabel *imageLabel = new QLabel(&editProfessorDialog);
        imageLabel->setFixedSize(200, 200);  // Установите желаемый фиксированный размер
        imageLabel->setScaledContents(true);

        // Кнопка для выбора изображения
        QPushButton *browseImageButton = new QPushButton("Обзор", &editProfessorDialog);

        // Добавляем кнопки "Сохранить" и "Отмена"
        QPushButton *saveButton = new QPushButton("Сохранить", &editProfessorDialog);
        QPushButton *cancelEditButton = new QPushButton("Отмена", &editProfessorDialog);

        // Создаем слоты для кнопок
        connect(saveButton, &QPushButton::clicked, &editProfessorDialog, &QDialog::accept);
        connect(cancelEditButton, &QPushButton::clicked, &editProfessorDialog, &QDialog::reject);
        connect(browseImageButton, &QPushButton::clicked, [&]() {
            QString imagePath = QFileDialog::getOpenFileName(&editProfessorDialog, "Выберите изображение", "", "Images (*.png *.jpg *.bmp)");
            if (!imagePath.isEmpty()) {
                QPixmap image(imagePath);
                imageLabel->setPixmap(image);
            }
        });

        // Создаем вертикальный лейаут и добавляем элементы управления
        QVBoxLayout *editLayout = new QVBoxLayout(&editProfessorDialog);
        editLayout->addWidget(new QLabel("Имя:", &editProfessorDialog));
        editLayout->addWidget(firstNameEdit);
        editLayout->addWidget(new QLabel("Фамилия:", &editProfessorDialog));
        editLayout->addWidget(lastNameEdit);
        editLayout->addWidget(new QLabel("Отчество:", &editProfessorDialog));
        editLayout->addWidget(middleNameEdit);
        editLayout->addWidget(new QLabel("Email:", &editProfessorDialog));
        editLayout->addWidget(emailEdit);
        editLayout->addWidget(new QLabel("Адрес:", &editProfessorDialog));
        editLayout->addWidget(addressEdit);
        editLayout->addWidget(new QLabel("Телефон:", &editProfessorDialog));
        editLayout->addWidget(phoneNumberEdit);
        editLayout->addWidget(new QLabel("Дата рождения (ГГГГ-ММ-ДД):", &editProfessorDialog));
        editLayout->addWidget(birthdateEdit);
        editLayout->addWidget(new QLabel("Ученая степень:", &editProfessorDialog));
        editLayout->addWidget(scienceDegreeEdit);

        // Добавляем элементы для изображения
        editLayout->addWidget(new QLabel("Изображение:", &editProfessorDialog));
        QHBoxLayout *imageLayout = new QHBoxLayout;
        imageLayout->addWidget(imageLabel);
        imageLayout->addWidget(browseImageButton);
        editLayout->addLayout(imageLayout);

        editLayout->addWidget(saveButton);
        editLayout->addWidget(cancelEditButton);

        // Подключение к базе данных SQLite (не обязательно, если уже подключены)
        // QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        // db.setDatabaseName(QCoreApplication::applicationDirPath() + "/database/database.db3");

        // Подготовка SQL-запроса для получения информации о выбранном преподавателе
        QSqlQuery selectQuery;
        selectQuery.prepare("SELECT * FROM Professor WHERE Email = :professor_email");
        selectQuery.bindValue(":professor_email", selectedEmail);

        // Выполнение SQL-запроса для получения данных о преподавателе
        if (selectQuery.exec() && selectQuery.next()) {
            // Получение значений из результата запроса
            firstNameEdit->setText(selectQuery.value("Имя").toString());
            lastNameEdit->setText(selectQuery.value("Фамилия").toString());
            middleNameEdit->setText(selectQuery.value("Отчество").toString());
            emailEdit->setText(selectQuery.value("Email").toString());
            addressEdit->setText(selectQuery.value("Адрес_проживания").toString());
            phoneNumberEdit->setText(selectQuery.value("Номер_телефона").toString());
            birthdateEdit->setText(selectQuery.value("Дата_рождения").toString());
            scienceDegreeEdit->setText(selectQuery.value("Учёная_степень").toString());

            // Отобразим изображение, если оно есть в базе данных
            QByteArray imageData = selectQuery.value("Image").toByteArray();
            if (!imageData.isEmpty()) {
                QPixmap image;
                image.loadFromData(imageData);
                imageLabel->setPixmap(image);
            }
        } else {
            qDebug() << "Ошибка при выполнении запроса для получения данных преподавателя:" << selectQuery.lastError().text();
        }

        // Показываем диалоговое окно для изменения данных преподавателя
        if (editProfessorDialog.exec() == QDialog::Accepted) {
            // Здесь добавьте логику изменения данных преподавателя в базе данных
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE Professor SET Имя = :first_name, Фамилия = :last_name, "
                                "Отчество = :middle_name, Email = :email, Адрес_проживания = :address, "
                                "Номер_телефона = :phone_number, Дата_рождения = :birthdate, "
                                "Учёная_степень = :science_degree, Image = :image WHERE Email = :professor_email");

            updateQuery.bindValue(":first_name", firstNameEdit->text());
            updateQuery.bindValue(":last_name", lastNameEdit->text());
            updateQuery.bindValue(":middle_name", middleNameEdit->text());
            updateQuery.bindValue(":email", emailEdit->text());
            updateQuery.bindValue(":address", addressEdit->text());
            updateQuery.bindValue(":phone_number", phoneNumberEdit->text());
            updateQuery.bindValue(":birthdate", birthdateEdit->text());
            updateQuery.bindValue(":science_degree", scienceDegreeEdit->text());

            // Получаем изображение из QLabel и сохраняем его в базе данных
            QPixmap currentImage = imageLabel->pixmap(Qt::ReturnByValue);
            QByteArray imageData;
            QBuffer buffer(&imageData);
            buffer.open(QIODevice::WriteOnly);
            currentImage.save(&buffer, "PNG");  // Можете использовать другой формат

            updateQuery.bindValue(":image", imageData);
            updateQuery.bindValue(":professor_email", selectedEmail);

            if (updateQuery.exec()) {
                qDebug() << "Данные преподавателя успешно изменены";
            } else {
                qDebug() << "Ошибка при изменении данных преподавателя:" << updateQuery.lastError().text();
            }
        }
    }

    // Закрытие соединения с базой данных (если еще не закрыто)
    if (db.isOpen()) {
        db.close();
    }
}

void adminWindow::on_pushButton_clicked()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/database/database.db3");
    db.open();

    QString SQL = ui->textEdit->toPlainText();
    qDebug() << SQL;

    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(SQL);

    if (model->lastError().isValid()) {
        // Обработка ошибки выполнения запроса
        qDebug() << "Ошибка выполнения запроса:" << model->lastError().text();
                QMessageBox::warning(this, "Ошибка", "Ошибка выполнения запроса. Пожалуйста, повторите попытку.");
        db.close();
        return;
    }


    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->show();
    db.close();

}


void adminWindow::on_pushButton_7_clicked()
{
// Подключение к базе данных SQLite
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/database/database.db3");
    if (!db.open()) {
        qDebug() << "Не удалось открыть базу данных";
        return;
    }

    // Создаем новое диалоговое окно выбора преподавателя
    QDialog selectProfessorDialog(this);
    selectProfessorDialog.setWindowTitle("Выберите преподавателя для удаления");

    // Создаем модель для отображения списка преподавателей
    QSqlQueryModel *professorModel = new QSqlQueryModel;
    professorModel->setQuery("SELECT Email FROM Professor");

    // Создаем выпадающий список для выбора преподавателя
    QComboBox *professorComboBox = new QComboBox(&selectProfessorDialog);
    professorComboBox->setModel(professorModel);

    // Добавляем кнопки "Выбрать" и "Отмена"
    QPushButton *selectButton = new QPushButton("Выбрать", &selectProfessorDialog);
    QPushButton *cancelButton = new QPushButton("Отмена", &selectProfessorDialog);

    // Создаем слоты для кнопок
    connect(selectButton, &QPushButton::clicked, &selectProfessorDialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &selectProfessorDialog, &QDialog::reject);

    // Создаем вертикальный лейаут и добавляем элементы управления
    QVBoxLayout *layout = new QVBoxLayout(&selectProfessorDialog);
    layout->addWidget(new QLabel("Выберите преподавателя:", &selectProfessorDialog));
    layout->addWidget(professorComboBox);
    layout->addWidget(selectButton);
    layout->addWidget(cancelButton);

    // Показываем диалоговое окно выбора преподавателя
    if (selectProfessorDialog.exec() == QDialog::Accepted) {
        // Получаем выбранного преподавателя
        QString selectedEmail = professorComboBox->currentText();

        // Здесь добавьте логику удаления преподавателя из базы данных
        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM Professor WHERE Email = :professor_email");
        deleteQuery.bindValue(":professor_email", selectedEmail);

        if (deleteQuery.exec()) {
            qDebug() << "Преподаватель успешно удален";
        } else {
            qDebug() << "Ошибка при удалении преподавателя:" << deleteQuery.lastError().text();
        }
    }

    // Закрытие соединения с базой данных
    db.close();
}


void adminWindow::on_pushButton_8_clicked()
{
// Подключение к базе данных SQLite
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/database/database.db3");
    if (!db.open()) {
        qDebug() << "Не удалось открыть базу данных";
        return;
    }

    // Создаем новое диалоговое окно выбора администратора
    QDialog selectAdminDialog(this);
    selectAdminDialog.setWindowTitle("Выберите администратора для удаления");

    // Создаем модель для отображения списка администраторов
    QSqlQueryModel *adminModel = new QSqlQueryModel;
    adminModel->setQuery("SELECT Логин FROM Administrator");

    // Создаем выпадающий список для выбора администратора
    QComboBox *adminComboBox = new QComboBox(&selectAdminDialog);
    adminComboBox->setModel(adminModel);

    // Добавляем кнопки "Выбрать" и "Отмена"
    QPushButton *selectButton = new QPushButton("Выбрать", &selectAdminDialog);
    QPushButton *cancelButton = new QPushButton("Отмена", &selectAdminDialog);

    // Создаем слоты для кнопок
    connect(selectButton, &QPushButton::clicked, &selectAdminDialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &selectAdminDialog, &QDialog::reject);

    // Создаем вертикальный лейаут и добавляем элементы управления
    QVBoxLayout *layout = new QVBoxLayout(&selectAdminDialog);
    layout->addWidget(new QLabel("Выберите администратора:", &selectAdminDialog));
    layout->addWidget(adminComboBox);
    layout->addWidget(selectButton);
    layout->addWidget(cancelButton);

    // Показываем диалоговое окно выбора администратора
    if (selectAdminDialog.exec() == QDialog::Accepted) {
        // Получаем выбранного администратора
        QString selectedLogin = adminComboBox->currentText();

        // Здесь добавьте логику удаления администратора из базы данных
        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM Administrator WHERE Логин = :admin_login");
        deleteQuery.bindValue(":admin_login", selectedLogin);

        if (deleteQuery.exec()) {
            qDebug() << "Администратор успешно удален";
        } else {
            qDebug() << "Ошибка при удалении администратора:" << deleteQuery.lastError().text();
        }
    }

    // Закрытие соединения с базой данных
    db.close();
}

