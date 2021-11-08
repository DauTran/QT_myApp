#ifndef MYMODEL_H
#define MYMODEL_H

#include <QAbstractListModel>

enum Statuses{ unchecked, checked };

struct Data {
    Data() {}
    Data( const QString& name, const QString& icon, const bool& check): name(name), icon(icon), check(check) {}

    QString name;
    QString icon;
    bool check;
};

class MyModel : public QAbstractListModel
{
    Q_OBJECT
//    Q_PROPERTY(bool check READ getCheck WRITE setCheck NOTIFY Changed)
public:
    enum Roles {
        NameRole = Qt::UserRole,
        IconRole,
        CheckRole
    };

    explicit MyModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex& parent) const override;
    QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void clear();
    Q_INVOKABLE void deleteSelection();

    bool getCheck() const;
    void setCheck();

public slots:
    void changeData(int row);
    void removeData(int row);
    void insertData(int row, Data data);
    void passAll();
    void passFile();
    void passFolder();

    void toggelStatus(int row);

private: //members
    QVector< Data > m_data;
    QString m_path;
};

#endif // MYMODEL_H
