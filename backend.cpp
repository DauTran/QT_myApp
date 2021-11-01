#include "backend.h"

BackEnd::BackEnd(QObject *parent) :
    QObject(parent)
{
    m_dir = ".";
}

QString BackEnd::getDir() const
{
    return m_dir;
}

bool BackEnd::setDir(const QString &dir)
{
    bool ret = false;
    if(dir != "")
    {
        m_dir = dir;
        ret = true;
    }
    return ret;
}

QStringList BackEnd::processDir()
{
    QDir dir(m_dir);
    QStringList list;

    foreach (QFileInfo var, dir.entryInfoList())
    {
        list.append(var.fileName());
    }
    return list;
}

//QString BackEnd::userName()
//{
//    return m_userName;
//}

//void BackEnd::setUserName(const QString &userName)
//{
//    if (userName == m_userName)
//        return;

//    m_userName = userName;
//    emit userNameChanged();
//}

//void test()
//{
//    QDir dir("A:/Training Life Good");
//    foreach (QFileInfo var, dir.entryInfoList()) {
//        qDebug() << var.fileName();
//        qDebug() << "Path" << var.filePath();
//    }

//}
