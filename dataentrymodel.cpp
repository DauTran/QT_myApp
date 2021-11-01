#include "dataentrymodel.h"

DataEntryModel::DataEntryModel(QObject *parent)
    : QAbstractListModel(parent), m_path(".")
{
    changeItemModel();
}

DataEntryModel::~DataEntryModel()
{
}

int DataEntryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    // return our data count
    return m_data.count();
}

QVariant DataEntryModel::data(const QModelIndex &index, int role) const
{
    qDebug() << __FUNCTION__ << " Here ";
    // the index returns the requested row and column information.
    // we ignore the column and only use the row information
    int row = index.row();

    // boundary check for the row
    if(row < 0 || row >= m_data.count()) {
        return QVariant();
    }

    // A model can return data for different roles.
    // The default role is the display role.
    // it can be accesses in QML with "model.display"
    switch(role) {
        case Qt::DisplayRole:
            // Return the color name for the particular row
            // Qt automatically converts it to the QVariant type
            return m_data.value(row);
    }

    // The view asked for other data, just return an empty QVariant
    return QVariant();
}

QString DataEntryModel::getPath()const
{
    return m_path;
}

void DataEntryModel::setPath(QString path)
{
    if(path != "")
    {
        m_path = path;
    }
    else
    {
        qDebug() << "Path is not true";
    }
}

void DataEntryModel::changeItemModel()
{
    // initialize our data (QList<QString>) with a list of file and folder names
    QDir dir(m_path);
    QStringList list;

    foreach (QFileInfo var, dir.entryInfoList())
    {
        list.append(var.fileName());
    }
    m_data = list;
}
