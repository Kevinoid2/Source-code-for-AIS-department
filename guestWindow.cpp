#include "guestWindow.h"
#include "ui_guestWindow.h"

guestWindow::guestWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::guestWindow)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/database/database.db3");
    db.open();
}

guestWindow::~guestWindow()
{
    delete ui;
}

void guestWindow::closeEvent(QCloseEvent *event)
{
    // Вызываем closeEvent базового класса
    QWidget::closeEvent(event);

    // Показываем родительское окно после закрытия окна профессора
    if (parentWidget()) {
        parentWidget()->show();
    }
}

void guestWindow::on_pushButton_clicked()
{
    QString group = ui->textEdit->toPlainText();

    // Открываем соединение с базой данных
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/database/database.db3");

    if (!db.open()) {
        qDebug() << "Не удалось открыть базу данных";
        return;
    }

    // Создаем SQL-запрос для выборки расписания для заданной группы
    QSqlQueryModel *model = new QSqlQueryModel;
    QString queryText = "SELECT "
                        "Название_занятия AS Занятие, "
                        "День_недели, "
                        "ClassTime.Начало AS Начало_занятия, "
                        "ClassTime.Конец AS Конец_занятия, "
                        "ClassType.Название_типа_занятия AS Тип_занятия "
                        "FROM Class "
                        "JOIN ClassTime ON Class.ID_время_занятия = ClassTime.ID_время_занятия "
                        "JOIN ClassType ON Class.ID_тип_занятия = ClassType.ID_тип_занятия "
                        "LEFT JOIN WeekType ON Class.ID_тип_недели = WeekType.ID_тип_недели "
                        "WHERE Группа = :group";
    QSqlQuery query;
    query.prepare(queryText);
    query.bindValue(":group", group);

    // Выполняем запрос
    if (query.exec()) {
        model->setQuery(query);
        if (model->lastError().isValid()) {
            qDebug() << "Ошибка выполнения запроса:" << model->lastError().text();
                    QMessageBox::warning(this, "Ошибка", "Ошибка выполнения запроса. Пожалуйста, повторите попытку.");
            return;
        }

        // Устанавливаем модель для отображения в таблице
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();
        ui->tableView->show();
    } else {
        qDebug() << "Ошибка выполнения SQL-запроса:" << query.lastError().text();
                QMessageBox::warning(this, "Ошибка", "Ошибка выполнения SQL-запроса. Пожалуйста, повторите попытку.");
    }

    // Закрываем соединение с базой данных
    db.close();

}

void guestWindow::on_pushButton_3_clicked()
{
// Открываем соединение с базой данных
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/database/database.db3");

    if (!db.open()) {
        qDebug() << "Не удалось открыть базу данных";
        return;
    }

    // Создаем модель для списка преподавателей
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT ID_преподаватель, Имя, Фамилия, Отчество FROM Professor");

    if (model->lastError().isValid()) {
        qDebug() << "Ошибка выполнения запроса:" << model->lastError().text();
        QMessageBox::warning(this, "Ошибка", "Ошибка выполнения запроса. Пожалуйста, повторите попытку.");
        db.close();
        return;
    }

    // Отображаем окно выбора преподавателя
    bool ok;
    QStringList professorList;
    int rowCount = model->rowCount();

    for (int i = 0; i < rowCount; ++i) {
        QString professor = model->record(i).value("Имя").toString() + " "
                          + model->record(i).value("Фамилия").toString() + " "
                          + model->record(i).value("Отчество").toString();
        professorList.append(professor);
    }

    QString selectedProfessor = QInputDialog::getItem(this, "Выберите преподавателя", "Преподаватель:", professorList, 0, false, &ok);

    if (!ok || selectedProfessor.isEmpty()) {
        db.close();
        return;  // Пользователь отменил выбор или не выбрал преподавателя
    }

    // Получаем информацию о выбранном преподавателе
    QSqlQuery query;

    int professorId = model->record(professorList.indexOf(selectedProfessor)).value("ID_преподаватель").toInt();
    query.prepare("SELECT Имя, Фамилия, Отчество, Учёная_степень, Image FROM Professor WHERE ID_преподаватель = :id");
    query.bindValue(":id", professorId);

    if (query.exec() && query.next()) {
        QString name = query.value("Имя").toString();
        QString surname = query.value("Фамилия").toString();
        QString patronymic = query.value("Отчество").toString();
        QString academicDegree = query.value("Учёная_степень").toString();
        QByteArray imageData = query.value("Image").toByteArray();

        // Закрываем соединение с базой данных
        db.close();

        // Отображаем окно с информацией о преподавателе
        QDialog professorDialog(this);
        professorDialog.setWindowTitle("Информация о преподавателе");
        professorDialog.setGeometry(100, 100, 400, 300);

        QVBoxLayout *layout = new QVBoxLayout(&professorDialog);

        QLabel *nameLabel = new QLabel("Имя: " + name, &professorDialog);
        QLabel *surnameLabel = new QLabel("Фамилия: " + surname, &professorDialog);
        QLabel *patronymicLabel = new QLabel("Отчество: " + patronymic, &professorDialog);
        QLabel *academicDegreeLabel = new QLabel("Учёная степень: " + academicDegree, &professorDialog);

        // Отображение фотографии преподавателя
        QPixmap image;
        image.loadFromData(imageData);
        QLabel *imageLabel = new QLabel(&professorDialog);
        imageLabel->setPixmap(image.scaled(100, 100, Qt::KeepAspectRatio));

        layout->addWidget(nameLabel);
        layout->addWidget(surnameLabel);
        layout->addWidget(patronymicLabel);
        layout->addWidget(academicDegreeLabel);
        layout->addWidget(imageLabel);

        professorDialog.exec();
    } else {
        qDebug() << "Ошибка выполнения SQL-запроса:" << query.lastError().text();
        QMessageBox::warning(this, "Ошибка", "Ошибка выполнения SQL-запроса. Пожалуйста, повторите попытку.");
        db.close();
    }
}




void guestWindow::on_pushButton_2_clicked()
{
// Открываем соединение с базой данных
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/database/database.db3");

    if (!db.open()) {
        qDebug() << "Не удалось открыть базу данных";
        return;
    }

    // Создаем модель для списка кафедр
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT ID_кафедра, Название_кафедры FROM Department");

    if (model->lastError().isValid()) {
        qDebug() << "Ошибка выполнения запроса:" << model->lastError().text();
        QMessageBox::warning(this, "Ошибка", "Ошибка выполнения запроса. Пожалуйста, повторите попытку.");
        db.close();
        return;
    }

    // Отображаем окно выбора кафедры
    bool ok;
    QStringList departmentList;
    int rowCount = model->rowCount();

    for (int i = 0; i < rowCount; ++i) {
        QString department = model->record(i).value("Название_кафедры").toString();
        departmentList.append(department);
    }

    QString selectedDepartment = QInputDialog::getItem(this, "Выберите кафедру", "Кафедра:", departmentList, 0, false, &ok);

    if (!ok || selectedDepartment.isEmpty()) {
        db.close();
        return;  // Пользователь отменил выбор или не выбрал кафедру
    }

    // Получаем информацию о выбранной кафедре
    QSqlQuery query;
    query.prepare("SELECT Название_кафедры, Дата_создания, Расположение, ID_заведующий_кафедры FROM Department WHERE Название_кафедры = :name");
    query.bindValue(":name", selectedDepartment);

    if (query.exec() && query.next()) {
        QString departmentName = query.value("Название_кафедры").toString();
        QString creationDate = query.value("Дата_создания").toString();
        QString location = query.value("Расположение").toString();
        int headId = query.value("ID_заведующий_кафедры").toInt();

        // Получаем информацию о заведующем кафедрой
        query.prepare("SELECT Имя, Фамилия, Отчество, Image FROM Professor WHERE ID_преподаватель = :id");
        query.bindValue(":id", headId);

        if (query.exec() && query.next()) {
            QString headName = query.value("Имя").toString();
            QString headSurname = query.value("Фамилия").toString();
            QString headPatronymic = query.value("Отчество").toString();
            QByteArray headImageData = query.value("Image").toByteArray();

            // Закрываем соединение с базой данных
            db.close();

            // Отображаем окно с информацией о кафедре
            QDialog departmentDialog(this);
            departmentDialog.setWindowTitle("Информация о кафедре");
            departmentDialog.setGeometry(100, 100, 400, 300);

            QVBoxLayout *layout = new QVBoxLayout(&departmentDialog);

            QLabel *nameLabel = new QLabel("Название кафедры: " + departmentName, &departmentDialog);
            QLabel *creationDateLabel = new QLabel("Дата создания: " + creationDate, &departmentDialog);
            QLabel *locationLabel = new QLabel("Расположение: " + location, &departmentDialog);
            QLabel *headLabel = new QLabel("Заведующий кафедрой: " + headName + " " + headSurname + " " + headPatronymic, &departmentDialog);

            // Отображение фотографии заведующего кафедрой
            QPixmap headImage;
            headImage.loadFromData(headImageData);
            QLabel *headImageLabel = new QLabel(&departmentDialog);
            headImageLabel->setPixmap(headImage.scaled(100, 100, Qt::KeepAspectRatio));

            layout->addWidget(nameLabel);
            layout->addWidget(creationDateLabel);
            layout->addWidget(locationLabel);
            layout->addWidget(headLabel);
            layout->addWidget(headImageLabel);

            departmentDialog.exec();
        } else {
            qDebug() << "Ошибка выполнения SQL-запроса:" << query.lastError().text();
            QMessageBox::warning(this, "Ошибка", "Ошибка выполнения SQL-запроса. Пожалуйста, повторите попытку.");
            db.close();
        }
    } else {
        qDebug() << "Ошибка выполнения SQL-запроса:" << query.lastError().text();
        QMessageBox::warning(this, "Ошибка", "Ошибка выполнения SQL-запроса. Пожалуйста, повторите попытку.");
        db.close();
    }
}

