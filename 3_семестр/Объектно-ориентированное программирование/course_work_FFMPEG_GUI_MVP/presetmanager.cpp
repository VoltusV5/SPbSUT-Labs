#include "presetmanager.h"
#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

PresetManager::PresetManager(QObject *parent) : QObject(parent) {}

static QString presetsFilePath() {
    return QCoreApplication::applicationDirPath() + "/presets.xml";
}

void PresetManager::savePreset(const QString &name, const QString &codec,
                               const QString &resolution, const QString &container) {
    QString path = presetsFilePath();
    QDomDocument doc;
    QFile file(path);
    bool exists = file.exists();

    if (exists && file.open(QIODevice::ReadOnly)) {
        doc.setContent(&file);
        file.close();
    }

    QDomElement root = doc.documentElement();
    if (root.isNull()) {
        root = doc.createElement("presets");
        doc.appendChild(root);
    }

    // Удаляем старый
    QDomNodeList nodes = root.elementsByTagName("preset");
    for (int i = 0; i < nodes.count(); ++i) {
        if (nodes.at(i).toElement().attribute("name") == name) {
            root.removeChild(nodes.at(i));
            break;
        }
    }

    // Добавляем новый
    QDomElement preset = doc.createElement("preset");
    preset.setAttribute("name", name);

    auto add = [&](const QString &tag, const QString &value) {
        QDomElement el = doc.createElement(tag);
        el.appendChild(doc.createTextNode(value));
        preset.appendChild(el);
    };

    add("codec", codec);
    add("resolution", resolution);
    add("container", container);
    root.appendChild(preset);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream stream(&file);
        stream << doc.toString(4);
        file.close();
    }
}

QMap<QString, QString> PresetManager::loadPreset(const QString &name) const {
    QMap<QString, QString> result;
    QString path = presetsFilePath();

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return result;

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        return result;
    }
    file.close();

    QDomElement root = doc.documentElement();
    QDomNodeList presets = root.elementsByTagName("preset");
    for (int i = 0; i < presets.count(); ++i) {
        QDomElement el = presets.at(i).toElement();
        if (el.attribute("name") == name) {
            result["codec"] = el.firstChildElement("codec").text();
            result["resolution"] = el.firstChildElement("resolution").text();
            result["container"] = el.firstChildElement("container").text();
            break;
        }
    }
    return result;
}

QStringList PresetManager::presetNames() const {
    QStringList names;
    QString path = presetsFilePath();

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return names;

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        return names;
    }
    file.close();

    QDomElement root = doc.documentElement();
    QDomNodeList presets = root.elementsByTagName("preset");
    for (int i = 0; i < presets.count(); ++i) {
        names << presets.at(i).toElement().attribute("name");
    }
    return names;
}

void PresetManager::removePreset(const QString &name) {
    QString path = presetsFilePath();
    QDomDocument doc;
    QFile file(path);

    if (!file.exists() || !file.open(QIODevice::ReadOnly)) return;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    QDomElement root = doc.documentElement();
    QDomNodeList nodes = root.elementsByTagName("preset");
    for (int i = 0; i < nodes.count(); ++i) {
        QDomElement el = nodes.at(i).toElement();
        if (el.attribute("name") == name) {
            root.removeChild(el);
            break;
        }
    }

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream stream(&file);
        stream << doc.toString(4);
        file.close();
    }
}
