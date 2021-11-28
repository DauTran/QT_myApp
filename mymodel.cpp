
#include "mymodel.h"

MyModel::MyModel(QObject *parent) : QAbstractListModel(parent), m_path("")
{
    fPtr = &MyModel::passAll;

    m_timer = new QTimer();
    QObject::connect(m_timer, &QTimer::timeout, this, &MyModel::getModifierSystem);
    m_timer->start(1000);
}

MyModel::~MyModel()
{
    delete m_timer;
//    delete m_filter;
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

//remove all data in List
void MyModel::clear()
{
    emit beginResetModel();
    m_data.clear();
    emit endResetModel();
}

//delete the item when button check is true
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

//set the path at exist position
void MyModel::setPath(const QString& path)
{
    m_path = path;
    emit changedPath();
}

//get the path of exist position
QString MyModel::getPath() const
{
    return m_path;
}

//this function get receiv signal from system watcher
void MyModel::getModifierSystem()
{
    (this->*fPtr)();
}

//this function to move into the folder at the row, which was clicked by the user
void MyModel::changeData(const int& row)
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
    else //set at the root
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
void MyModel::insertData(const int& row, const Data& data)
{
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
    checkDataLength();
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

//this function to search the folder/file and append to list m_data
bool MyModel::searchDataRecursive(const QString& path, const QString& fileName)
{
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

//this function to search the name of file/folder and inform to Listview
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

bool MyModel::selecAllItems()
{
    emit beginResetModel();
    for(QVector<Data>::Iterator it = m_data.begin(); it != m_data.end(); it++)
    {
        it->check = true;
    }
    emit endResetModel();
    return true;
}

bool MyModel::deselecAllItems()
{
    emit beginResetModel();
    for(QVector<Data>::Iterator it = m_data.begin(); it != m_data.end(); it++)
    {
        it->check = false;
    }
    emit endResetModel();
    return true;
}

void MyModel::stopTimer()
{
    if(m_timer->isActive())
    {
        m_timer->stop();
    }
}

void MyModel::startTimer()
{

    if(!m_timer->isActive())
    {
        m_timer->start(1000);

    }
}

void MyModel::setPassAll()
{
    fPtr = &MyModel::passAll;
}

void MyModel::setPassFolder()
{
    fPtr = &MyModel::passFolder;
}

void MyModel::setPassFile()
{
    fPtr = &MyModel::passFile;
}





