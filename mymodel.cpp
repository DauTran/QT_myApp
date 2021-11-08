#include <QDir>
#include <QtCore>
#include "mymodel.h"

MyModel::MyModel(QObject *parent) : QAbstractListModel(parent), m_path("")
{
    QFileInfoList drives = QDir::drives();
    for(QFileInfo drive : drives)
    {
        QString temp = drive.path();
        temp.chop(1);
        m_data << Data(temp, "qrc:images/drive.png", unchecked);

    }

//    qDebug() << "Init list";
//    for(Data temp: m_data)
//    {
//        qDebug() << "The name: " << temp.name << " Status: " << temp.check;
//    }
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
    else if (role == CheckRole)
    {
        return data.check;
    }
    else
        return QVariant();
}

QHash<int, QByteArray> MyModel::roleNames() const
{
    static QHash<int, QByteArray> mapping {
        {NameRole, "name"},
        {IconRole, "icon"},
        {CheckRole, "check"},
    };
    return mapping;
}

void MyModel::clear()
{
    emit beginResetModel();
    m_data.clear();
    emit endResetModel();
}

void MyModel::deleteSelection()
{
    for(Data temp: m_data)
    {
        qDebug() << "The name: " << temp.name << " Status: " << temp.check;
    }
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
    QFileInfo temp(m_path + m_data.at(row).name);

    if(temp.isDir())
    {
//         m_path = temp.absolutePath();
         m_path =  m_path + m_data.at(row).name + "/";
         qDebug() << "path: " << m_path;
         passAll();
    }
}

// delete a folder
bool DelDir(const QString &path)
{
    if (path.isEmpty()){
        return false;
    }
    QDir dir(path);
    if(!dir.exists()){
        return true;
    }
         dir.setFilter (QDir :: AllEntries | QDir :: NoDotAndDotDot); // Set the filter
         QFileInfoList fileList = dir.entryInfoList (); // get all file information
         foreach (QFileInfo file, fileList) {// traverse file information
                 if (file.isFile ()) {// file, delete
            file.dir().remove(file.fileName());
                 } else {// recursive delete
            DelDir(file.absoluteFilePath());
        }
    }
         return dir.rmpath (dir.absolutePath ()); // delete folders
}

void MyModel::removeData(int row)
{
    qDebug() << "index: " << row;
    qDebug() << "length: " << m_data.length();
    if (row < 0 || row >= m_data.count())
        return;

    DelDir(m_path + m_data.at(row).name);
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
//    m_path = QDir::cleanPath(m_path);
    QDir dir(m_path);

    beginResetModel();
    m_data.clear();
    for(QFileInfo var: dir.entryInfoList())
    {
        if(var.isDir())
        {
            if(var.fileName() == "." || var.fileName() == "..")
            {
                continue;
            }
            m_data << Data(var.fileName(), "qrc:images/folder.png", false);
        }
        else if(var.isFile())
        {
             m_data << Data(var.fileName(), "qrc:images/file.png", false);
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
             m_data << Data(var.fileName(), "qrc:images/file.png", false);
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
            if(var.fileName() == "." || var.fileName() == "..")
            {
                continue;
            }
            m_data << Data(var.fileName(), "qrc:images/folder.png", false);
        }

    }
    endResetModel();
}

void MyModel::toggelStatus(int row)
{
    m_data[row].check = !m_data.at(row).check;
}





