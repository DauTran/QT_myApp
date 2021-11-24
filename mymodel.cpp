
#include "mymodel.h"

MyModel::MyModel(QObject *parent) : QAbstractListModel(parent), m_path("")
{
    passAll();

    m_notifier = new QFileSystemWatcher();
    QObject::connect(m_notifier, &QFileSystemWatcher::directoryChanged, this, &MyModel::passAll);
}

MyModel::~MyModel()
{
    delete m_notifier;
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

//    const Data &data = m_data.at(index.row());
    if ( role == NameRole ){
        return m_data.at(index.row()).name;
    }
    else if ( role == IconRole )
    {
        QString iconPath = "";
        switch (m_data.at(index.row()).icon)
        {
        case DISK_ICON:
            iconPath = "qrc:images/drive.png";
            break;
        case FOLDER_ICON:
            iconPath = "qrc:images/folder.png";
            break;
        case FILE_ICON:
            iconPath = "qrc:images/file.png";
            break;
        }
        return iconPath;
    }
    else if (role == CheckRole)
    {
        return m_data.at(index.row()).check;
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

int MyModel::count() const
{
    return rowCount(QModelIndex());
}

void MyModel::checkDataLength()
{

    if(m_data.length() == 0)
    {
        emit countChanged(true);
    }
    else
    {
        emit countChanged(false);
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
        if(m_data[i].check)
        {
            removeData(i);
            --i;
        }
    }
}

void MyModel::setPath(const QString& path)
{
    m_path = path;
    emit changedPath();
}

QString MyModel::getPath() const
{
//    qDebug() << __FUNCTION__ << __LINE__ << "here";
    return m_path;
}

void MyModel::changeData(int row)
{
    if( FILE_ICON != m_data.at(row).icon)
    {
        if(m_data.at(row).name.contains('/') ) // using when search file/folder
        {
            m_path =  m_data.at(row).name + "/";
        }
        else// using in normal case when user click on the folder
        {
            m_path =  m_path + m_data.at(row).name + "/";
        }
        passAll();
        checkDataLength();
    }
}

//this function to back the parent folder(like cd command)
void MyModel::changeDirUp()
{
    QDir dir(m_path);
    if(dir.cdUp()) // check the direction when
    {
        m_path = dir.canonicalPath();
        if(m_path.back() != '/')
        {
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

//this function to remove the folder/file at a row
void MyModel::removeData(const int& row)
{
    if (row < 0 || row >= m_data.count())
        return;

    emit beginRemoveRows(QModelIndex(), row, row);
//    QFileInfo temp(m_path + m_data.at(row).name);
    QDir dir(m_path + m_data.at(row).name);

    if(FOLDER_ICON == m_data.at(row).icon)
    {
        dir.removeRecursively();
    }
    else if( FILE_ICON == m_data.at(row).icon )
    {
        dir.remove(m_path + m_data.at(row).name);
    }
    m_data.removeAt(row);
    emit endRemoveRows();
    checkDataLength();
}

//this function to insert the folder/file at a row
void MyModel::insertData(int row, const Data& data)
{
    qDebug() << "row in dec " << row;
    if (row < 0)
        return;

    emit beginInsertRows(QModelIndex(), row, row);
    m_data.insert(row, data);
    emit endInsertRows();
}

//this function to pass all(the folder/file)
void MyModel::passAll()
{
    emit inRoot(true);
    if(m_path == "")
    {
        emit inRoot(false);
        emit beginResetModel();
        m_data.clear();
        QFileInfoList drives = QDir::drives();
        for(QFileInfo drive : drives)
        {
            QString temp = drive.path();
            temp.chop(1);
            m_data << Data(temp, DISK_ICON, UNCHECKED);
        }
        emit endResetModel();
    }
    else
    {
        m_notifier->addPath(m_path);
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
                m_data << Data(var.fileName(), FOLDER_ICON, UNCHECKED);
            }
            else if(var.isFile())
            {
                m_data << Data(var.fileName(), FILE_ICON, UNCHECKED);
            }
        }
        emit endResetModel();
    }
    emit changedPath();
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
            m_data << Data(var.fileName(), FILE_ICON, UNCHECKED);
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
            m_data << Data(var.fileName(), FOLDER_ICON, UNCHECKED);
        }

    }
    emit endResetModel();
}

void MyModel::toggelStatus(int row)
{
    m_data[row].check = !m_data.at(row).check;
}

bool MyModel::searchDataRecursive(const QString& path, const QString& fileName)
{
//    qDebug() << __FUNCTION__ << __LINE__ << ": We get " << path;
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
                m_data << Data(file.filePath(), FILE_ICON, UNCHECKED);
            }
        }
        else
        {
            if(file.fileName().contains(fileName))
            {
                m_data << Data(file.absoluteFilePath(), FOLDER_ICON, UNCHECKED);
            }
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
    checkDataLength();
}



bool MyModel::addFolder(const QString& fileName)
{
    QDir directory;
    QString temp = m_path + fileName;
    bool status = false;

    if(directory.mkdir(temp))
    {
        insertData(count(), Data(fileName, FOLDER_ICON, UNCHECKED));
        status = true;
    }
    checkDataLength();

    return status;
}







