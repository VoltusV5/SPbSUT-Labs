#ifndef PRESETMANAGER_H
#define PRESETMANAGER_H

#include <QObject>
#include <QMap>

class PresetManager : public QObject {
    Q_OBJECT
public:
    explicit PresetManager(QObject *parent = nullptr);
    void savePreset(const QString &name, const QString &codec,
                    const QString &resolution, const QString &container);
    void removePreset(const QString &name);
    QMap<QString, QString> loadPreset(const QString &name) const;
    QStringList presetNames() const;
};

#endif // PRESETMANAGER_H
