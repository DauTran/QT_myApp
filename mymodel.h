#ifndef MYMODEL_H
#define MYMODEL_H

#include <QAbstractListModel>

struct Data {
    Data() {}
    Data( const QString& name, const QString& icon): name(name), icon(icon) {}

    QString name;
    QString icon;
};

class MyModel : public QAbstractListModel
{
    Q_OBJECT
//    Q_PROPERTY(QString path READ getPath WRITE setPath NOTIFY pathChanged)
public:
    enum Roles {
        NameRole = Qt::UserRole,
        IconRole
    };
    explicit MyModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex& parent) const override;
    QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void clear();
//    void setPath(QString path);
//    QString getPath()const;

public slots:
    void changeData(int row);
    void removeData(int row);
    void insertData(int row, Data data);
    void passAll();
    void passFile();
    void passFolder();

// signals:
//    void pathChanged();

private: //members
    QVector< Data > m_data;
    QString m_path;
};

#endif // MYMODEL_H
