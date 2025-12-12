#ifndef ITEM_H
#define ITEM_H

#include <QObject>
#include <QString>
#include <QJsonObject>

class Item : public QObject
{
    Q_OBJECT

public:
    explicit Item(int id = 0, const QString &name = QString(), QObject *parent = nullptr);

    // Getters and setters
    void setId(int id);
    void setName(const QString &name);

    int getId() const;
    QString getName() const;

    // JSON operations
    QJsonObject toJson() const;
    static Item* fromJson(const QJsonObject &json, QObject *parent = nullptr);


private:
    int id;
    QString name;
};

#endif // ITEM_H
