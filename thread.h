#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QVector>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include "item.h"

class Thread : public QThread
{
    Q_OBJECT

public:
    Thread(QObject *parent = nullptr, int id = 0, int startIndex = 0, int endIndex = 0);

    void run() override;

    static QString generateThreadId(int index);
    static void startThreads(int totalWork);

    // اضافه کردن تابع ذخیره‌سازی JSON
    static void saveItemsToJson(const QVector<Item*>& items, const QString& fileName);

    // Getters
    QString getThreadId() const { return threadId; }
    int getStartIndex() const { return startIndex; }
    int getEndIndex() const { return endIndex; }
    QVector<Item*> getItems() const { return items; }

signals:
    void threadStarted(const QString& threadId);

private:
    int startIndex;
    int endIndex;
    QString threadId;

    QVector<Item*> items;
};

#endif // THREAD_H
