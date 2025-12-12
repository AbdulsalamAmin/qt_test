#include "thread.h"

#include <QThread>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>

#include <iostream>

Thread::Thread(QObject *parent, int id, int startIndex, int endIndex)
    : QThread(parent), startIndex(startIndex), endIndex(endIndex)
{
    threadId = QString(generateThreadId(id));
}

QString Thread::generateThreadId(int index)
{
    if (index < 0) return "Invalid";

    QChar letter1 = ('A' + index % 26);
    QChar letter2 = 'a' + ((index / 26) % 26);

    return QString(letter1) + QString(letter2);
}


void Thread::saveItemsToJson(const QVector<Item*>& items, const QString& fileName)
{
    QJsonArray jsonArray;

    for (const Item* item : items) {
        if (item) {
            jsonArray.append(item->toJson());
        }
    }

    QJsonObject rootObject;
    rootObject["items"] = jsonArray;
    rootObject["totalItems"] = jsonArray.size();

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(rootObject);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        qDebug() << "Saved" << jsonArray.size() << "items to" << fileName;
    } else {
        qWarning() << "Failed to save to" << fileName << ":" << file.errorString();
    }
}


void Thread::run()
{


    qDebug() << "Running" << threadId << "on thread ID:" << QThread::currentThreadId();

    int totalItems = endIndex - startIndex;

    QVector<int> numbers;

    for (int i = startIndex; i < endIndex; i++) {
        int result = i * 1000;
        numbers.push_back(i);

        QString itemName = QString("Item_%1").arg(i);
        Item* item = new Item(i, itemName);
        items.push_back(item);

    }
}

void Thread::startThreads(int totalWork)
{
    const int minChunkSize = 1000;
    int maxThreadCount = 100;

    int chunkSize = qMax(minChunkSize, totalWork / maxThreadCount);
    int threadCount = (totalWork + chunkSize - 1) / chunkSize;

    qDebug() << "totalWork:" << totalWork
             << "chunkSize:" << chunkSize
             << "threadCount:" << threadCount;

    QVector<Thread*> threads;
    threads.reserve(threadCount);

    // Create and start threads
    for (int j = 0; j < threadCount; j++) {
        int start = j * chunkSize;
        int end = qMin((j + 1) * chunkSize, totalWork);

        if (start >= end) {
            continue;
        }

        Thread *t = new Thread(nullptr, j, start, end);
        threads.push_back(t);


        // Connect signals
        QObject::connect(t, &Thread::threadStarted, [](const QString& id) {
            qDebug() << "Thread" << id << "started";
        });


        QObject::connect(t, &QThread::finished, t, &QObject::deleteLater);

        t->start();
    }

    // Wait for all threads to complete
    for (Thread* thread : threads) {
        thread->wait();
    }

    // Collect and save all items
    QVector<Item*> allItems;
    for (Thread* thread : threads) {
        QVector<Item*> threadItems = thread->getItems();
        for (Item* item : threadItems) {
            allItems.append(item);
        }
    }

    // Save to JSON
    Thread::saveItemsToJson(allItems, "threads_data.json");

    qDebug() << "All" << threadCount << "threads completed total work of" << totalWork;
}
