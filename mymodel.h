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
//    Q_PROPERTY(bool path /*READ getPath*/ WRITE setPath NOTIFY changedPath)
public:
    enum Roles {
        NameRole = Qt::UserRole,
        IconRole,
        CheckRole
    };

    explicit MyModel(QObject *parent = nullptr);

//    bool getPath() const;
//    void setPath(bool path);

    int rowCount(const QModelIndex& parent) const override;
    QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;
    QHash<int, QByteArray> roleNames() const override;

    void checkDataLength();

    Q_INVOKABLE void clear();
    Q_INVOKABLE void deleteSelection();

signals:
    void changedPath(); // inform the path changed
    void noData(bool visibleData); // inform no data in the list data
    void visibleBack(bool visibleButton); // at the root no visible back button

public slots:
    void changeData(int row);
    void changeDirUp();
    void removeData(const int& row);
    void insertData(int row, Data data);
    void passAll();
    void passFile();
    void passFolder();

    void toggelStatus(int row);

    bool searchDataRecursive(const QString& path, const QString& fileName);
    void search(const QString& fileName);


private: //members
    QVector< Data > m_data;
    QString m_path;
    bool DelDir(const QString &path);
};

#endif // MYMODEL_H
