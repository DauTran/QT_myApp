#ifndef MYMODEL_H
#define MYMODEL_H

#include <QDir>
#include <QtCore>
#include <QAbstractListModel>

enum Statuses { UNCHECKED, CHECK };
enum Icon {DISK_ICON, FOLDER_ICON, FILE_ICON};

struct Data {
    Data() {}
    Data( QString name, char icon, bool check): name(name), icon(icon), check(check) {}

    QString name;
    char icon;
    bool check;
};

class MyModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString path READ getPath WRITE setPath NOTIFY changedPath)
public:
    enum Roles {
        NameRole = Qt::UserRole,
        IconRole,
        CheckRole
    };

    explicit MyModel(QObject *parent = nullptr);
    virtual ~MyModel() override;

    void setPath(const QString& path);
    QString getPath() const;

    int rowCount(const QModelIndex& parent) const override;
    QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;
    QHash<int, QByteArray> roleNames() const override;
    int count() const;

    void checkDataLength();

    Q_INVOKABLE void clear();
    Q_INVOKABLE void deleteSelection();

signals:
    void changedPath(); // inform the path changed
    void countChanged(bool visibleData); // inform no data in the list data
    void inRoot(bool visibleButton); // at the root no visible back button

public slots:
    void changeData(int row);
    void changeDirUp();
    void removeData(const int& row);
    void insertData(int row, const Data& data);
    void passAll();
    void passFile();
    void passFolder();

    void toggelStatus(int row);

    bool searchDataRecursive(const QString& path, const QString& fileName);
    void search(const QString& fileName);
    bool addFolder(const QString& fileName);


private: //members
    QVector< Data > m_data;
    QString m_path;
    QFileSystemWatcher *m_notifier;
};

#endif // MYMODEL_H
