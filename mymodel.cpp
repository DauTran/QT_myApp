#include <QDir>
#include <QtCore>
#include "mymodel.h"

MyModel::MyModel(QObject *parent) : QAbstractListModel(parent), m_path("")
{
    passAll();
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

void MyModel::checkDataLength()
{

    if(m_data.length() == 0)
    {
        emit noData(true);
    }
    else
    {
        emit noData(false);
    }
}

void MyModel::clear()
{
    emit beginResetModel();
    m_data.clear();
    emit endResetModel();
}

void MyModel::deleteSelection()
{
    for(int i = 0; i < m_data.length(); i++)
    {
//        qDebug() << "The name: " << temp.name << " Status: " << temp.check;
        if(m_data[i].check)
        {
            DelDir(m_path + m_data.at(i).name);
            emit beginRemoveRows(QModelIndex(), i, i);
            m_data.remove(i);
            emit endRemoveRows();
        }
    }
}


//void MyModel::setPath(bool path)
//{
//    path = !path;
//    emit changedPath();
//}

//bool MyModel::getPath() const
//{
//    return path;
//}

void MyModel::changeData(int row)
{

    QFileInfo temp(m_path + m_data.at(row).name);

    if(temp.isDir())
    {
         m_path =  m_path + m_data.at(row).name + "/";
//         qDebug() << "path: " << m_path << "absolutePath: " << temp.absoluteFilePath();
         passAll();

         checkDataLength();
    }
}

void MyModel::changeDirUp()
{
//    qDebug() << __FUNCTION__ << __LINE__ << "path " << m_path;
    QDir dir(m_path);
    if(dir.cdUp()) // check the direction when
    {
        m_path = dir.canonicalPath();
        if(m_path.back() != '/')
        {
            qDebug() << "In here";
            m_path.append('/');
        }
    }
    else
    {
        m_path = "";
    }
    passAll();

     checkDataLength();
}


void MyModel::removeData(const int& row)
{
    qDebug() << "index: " << row;
    qDebug() << "length: " << m_data.length();
    if (row < 0 || row >= m_data.count())
        return;

    DelDir(m_path + m_data.at(row).name);
    emit beginRemoveRows(QModelIndex(), row, row);
    m_data.removeAt(row);
    emit endRemoveRows();

     checkDataLength();
}

void MyModel::insertData(int row, Data data)
{
    if (row < 0 || row >= m_data.count())
        return;

    emit beginInsertRows(QModelIndex(), row, row);
    m_data.insert(row, data);
    emit endInsertRows();
}

void MyModel::passAll()
{
    emit visibleBack(true);
    if(m_path == "")
    {
        emit visibleBack(false);
        emit beginResetModel();
        m_data.clear();
        QFileInfoList drives = QDir::drives();
        for(QFileInfo drive : drives)
        {
            QString temp = drive.path();
            temp.chop(1);
            m_data << Data(temp, "qrc:images/drive.png", unchecked);
        }
        emit endResetModel();


        return ;
    }
    QDir dir(m_path);
    emit beginResetModel();
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
    emit endResetModel();
}

//filter the folder
void MyModel::passFile()
{
    QDir dir(m_path);

    emit beginResetModel();
    m_data.clear();
    for(QFileInfo var: dir.entryInfoList())
    {
        if(var.isFile())
        {
             m_data << Data(var.fileName(), "qrc:images/file.png", false);
        }
    }
    emit endResetModel();
}

//filter the file
void MyModel::passFolder()
{
    QDir dir(m_path);

    emit beginResetModel();
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
    emit endResetModel();
}

// delete a folder
bool MyModel::DelDir(const QString &path)
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
        }
        else // recursive delete
        {
            DelDir(file.absoluteFilePath());
        }
    }
    return dir.rmpath (dir.absolutePath ()); // delete folders
}

void MyModel::toggelStatus(int row)
{
    m_data[row].check = !m_data.at(row).check;
}

bool MyModel::searchDataRecursive(const QString& path, const QString& fileName)
{
    qDebug() << __FUNCTION__ << __LINE__ << ": We get " << path;
    if( path.isEmpty() )
    {
        return false;
    }

    QDir dir(path);
    if(!dir.exists())
    {
        return true;
    }
    dir.setFilter( QDir:: AllEntries | QDir :: NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    for(QFileInfo file : fileList)
    {   
        if( file.isFile())
        {
            if(file.fileName().contains(fileName))
            {
//                emit beginResetModel();
                m_data << Data(file.filePath(), "qrc:images/file.png", false);
//                emit endResetModel();
            }
        }
        else
        {
//            if(file.fileName().contains(fileName))
//            {
//                emit beginResetModel();
//                m_data << Data(file.fileName(), "qrc:images/folder.png", false);
//                emit endResetModel();
//            }
            searchDataRecursive(file.absoluteFilePath(), fileName);
        }
    }
    return true;
}


void MyModel::search(const QString& fileName)
{
    m_data.clear();

    emit beginResetModel();
    searchDataRecursive(m_path, fileName);
    emit endResetModel();
    qDebug() << "Hilll";
    if(m_data.length() == 0)
    {
        emit noData(true);
    }
}






