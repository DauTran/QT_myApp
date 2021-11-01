#include <QDir>
#include <QtCore>
#include "mymodel.h"

MyModel::MyModel(QObject *parent) : QAbstractListModel(parent), m_path("A:/")
{
    QDir dir(m_path);
    for(QFileInfo var: dir.entryInfoList())
    {
        if(var.isDir())
        {
            m_data << Data(var.fileName(), "qrc:images/folder.png");
            qDebug() << var.fileName();
        }
        if(var.isFile())
        {
             m_data << Data(var.fileName(), "qrc:images/file.png");
             qDebug() << var.fileName();
        }

    }
}

int MyModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_data.count();
}

QVariant MyModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
        return QVariant();

    const Data &data = m_data.at(index.row());
    if ( role == NameRole ){
        return data.name;
    }
    else if ( role == IconRole )
        return data.icon;
    else
        return QVariant();
}

QHash<int, QByteArray> MyModel::roleNames() const
{
    static QHash<int, QByteArray> mapping {
          {NameRole, "name"},
          {IconRole, "icon"},
      };
    return mapping;
}

void MyModel::clear()
{
    emit beginResetModel();
    m_data.clear();
    emit endResetModel();
}

//void MyModel::setPath(QString path)
//{
//    m_path = path;
//}

//QString MyModel::getPath() const
//{
//    return m_path;
//}

void MyModel::changeData(int row)
{
    QFileInfo temp(m_path + "/" + m_data.at(row).name);
    qDebug() << "judge: " << temp.isFile();

    if(temp.isDir())
    {
         m_path += "/" + m_data.at(row).name;

//        QDir dir(m_path + "/" + m_data.at(row).name);
         passAll();
    }
}

void MyModel::removeData(int row)
{
    qDebug() << "index: " << row;
    qDebug() << "length: " << m_data.length();
    if (row < 0 || row >= m_data.count())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_data.removeAt(row);
    endRemoveRows();
}

void MyModel::insertData(int row, Data data)
{
    if (row < 0 || row >= m_data.count())
        return;

    beginInsertRows(QModelIndex(), row, row);
    m_data.insert(row, data);
    endInsertRows();
}

void MyModel::passAll()
{
    QDir dir(m_path);

    beginResetModel();
    m_data.clear();
    for(QFileInfo var: dir.entryInfoList())
    {
        if(var.isDir())
        {
            m_data << Data(var.fileName(), "qrc:images/folder.png");
        }
        if(var.isFile())
        {
             m_data << Data(var.fileName(), "qrc:images/file.png");
        }
    }
    endResetModel();
}

void MyModel::passFile()
{
    QDir dir(m_path);

    beginResetModel();
    m_data.clear();
    for(QFileInfo var: dir.entryInfoList())
    {
        if(var.isFile())
        {
             m_data << Data(var.fileName(), "qrc:images/file.png");
        }
    }
    endResetModel();
}

void MyModel::passFolder()
{
    QDir dir(m_path);

    beginResetModel();
    m_data.clear();
    for(QFileInfo var: dir.entryInfoList())
    {
        if(var.isDir())
        {
            m_data << Data(var.fileName(), "qrc:images/folder.png");
        }

    }
    endResetModel();
}





