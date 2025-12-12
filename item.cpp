#include "item.h"

#include <QString>

Item::Item(int id, const QString &name, QObject *parent)
    : QObject(parent), id(id), name(name) {}

void Item::setId(int id) {
    this->id = id;
}

void Item::setName(const QString &name) {
    this->name = name;
}

int Item::getId() const {
    return id;
}

QString Item::getName() const {
    return name;
}

QJsonObject Item::toJson() const {
    QJsonObject json;
    json["id"] = id;
    json["name"] = name;
    return json;
}

Item* Item::fromJson(const QJsonObject &json, QObject *parent) {
    int id = json["id"].toInt();
    QString name = json["name"].toString();
    return new Item(id, name, parent);
}



