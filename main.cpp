#include <QCoreApplication>
#include <QDebug>
#include <QString>
#include <QObject>
#include <QJsonArray>
#include <QJsonDocument>

#include <limits>
#include <iostream>

#include "thread.h"
#include "item.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int number;

    while (true) {
        qDebug() << "Please enter number between 1000 and 100000:";

        std::cin >> number;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            qDebug() << "Invalid input! Please enter a number.";
            continue;
        }

        if (number < 1000) {
            qDebug() << "Number less than 1000! Please try again.";
            continue;
        }

        if (number > 100000) {
            qDebug() << "Number greater than 100000! Please try again.";
            continue;
        }

        break;  // input is valid
    }

    qDebug() << "You entered:" << number;

    Thread manager;
    manager.startThreads(number);


    return a.exec();
}
