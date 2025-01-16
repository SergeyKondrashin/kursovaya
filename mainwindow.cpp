#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QMessageBox>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , appointmentCount(0)
{
    ui->setupUi(this);
    ui->dateEditAppointmentDate->setMinimumDate(QDate::currentDate());


    QString filePath = QCoreApplication::applicationDirPath() + "/appointments.txt";
    qDebug() << "Файл для записи:" << filePath;
    QFile file(filePath);
    if (file.exists()) {

        QFile readFile(filePath);
        if (readFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            while (!readFile.atEnd()) {
                QString line = readFile.readLine();
                if (line.startsWith("Талон #")) {
                    appointmentCount++;
                }
            }
            readFile.close();
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonBookAppointment_clicked()
{
    ui->lineEditFIO->setInputMask("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    ui->lineEditPhone->setInputMask("999-999-99-99");
    QString fio = ui->lineEditFIO->text();
    QString birthDate = ui->dateEditBirthDate->text();
    QString phone = ui->lineEditPhone->text();
    QString doctor = ui->comboBoxDoctors->currentText();
    QString appointmentDate = ui->dateEditAppointmentDate->text();
    QString appointmentTime = ui->comboBoxAppointmentTime->currentText();

    if (fio.isEmpty() || phone.isEmpty() || doctor.isEmpty() || appointmentDate.isEmpty() || appointmentTime.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните все поля.");
        return;
    }
    ui->lineEditFIO->setInputMask("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    ui->lineEditPhone->setInputMask("999-999-99-99");

    QString filePath = QCoreApplication::applicationDirPath() + "/appointments.txt";


    QFile checkFile(filePath);
    if (checkFile.exists() && !checkFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Нет прав на чтение файла.");
        return;
    }


    appointmentCount++; // Увеличиваем номер талона
    QFile file(filePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << "Талон #" << appointmentCount << "\n";
        out << "ФИО: " << fio << "\n";
        out << "Дата рождения: " << birthDate << "\n";
        out << "Номер телефона: " << phone << "\n";
        out << "Врач: " << doctor << "\n";
        out << "Дата приема: " << appointmentDate << "\n";
        out << "Время приема: " << appointmentTime << "\n";
        out << "--------------------------\n";
        file.close();
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл для записи.");
    }
    checkFile.close();


    QString output = QString("Талон #%1\nФИО: %2\nДата рождения: %3\nНомер телефона: %4\nВрач: %5\nДата приема: %6\nВремя приема: %7")
                         .arg(appointmentCount)
                         .arg(fio)
                         .arg(birthDate)
                         .arg(phone)
                         .arg(doctor)
                         .arg(appointmentDate)
                         .arg(appointmentTime);
    ui->textEditOutput->setPlainText(output);
}

