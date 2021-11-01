#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <QtQml>
#include <QDir>

class BackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString dir READ getDir WRITE setDir NOTIFY dirChanged)

public:
    explicit BackEnd(QObject *parent = nullptr);

    QString getDir() const;
    bool setDir(const QString &dir);

    Q_INVOKABLE QStringList processDir();

signals:
    void dirChanged();

private:
    QString m_dir;
};

#endif // BACKEND_H
