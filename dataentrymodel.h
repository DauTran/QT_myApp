#ifndef DATAENTRYMODEL_H
#define DATAENTRYMODEL_H

#include <QtCore>
#include <QtGui>

class DataEntryModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString path READ getPath WRITE setPath NOTIFY pathChanged )

public:
    explicit DataEntryModel(QObject *parent = nullptr);
    ~DataEntryModel();

    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    //
    QString getPath()const;
    void setPath(QString path);

signals:
    void pathChanged();

public slots:
    void changeItemModel();


private:
    QList<QString> m_data;
    QString m_path;

};

#endif // DATAENTRYMODEL_H
