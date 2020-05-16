#include "qpushactiveinfo.h"
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkInterface>
#include <QSysInfo>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QTextCodec>
#include <QEventLoop>
QMutex QPushActiveInfo::m_Mutex;
QAtomicPointer<QPushActiveInfo> QPushActiveInfo::m_pInstance;

QPushActiveInfo::QPushActiveInfo(QObject *parent) : QObject(parent)
{

}


QString QPushActiveInfo::gethostMac()
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();
    int nCnt = nets.count();
    QString strMacAddr = "";
    for(int i = 0; i < nCnt; i ++)
    {
        if(nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning)
                && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            strMacAddr = nets[i].hardwareAddress();
            break;
        }
    }
    return strMacAddr;
}

QString QPushActiveInfo::gethostIp()
{
    QString strIpAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    int nListSize = ipAddressesList.size();
    for (int i = 0; i < nListSize; ++i)
    {
       if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
           ipAddressesList.at(i).toIPv4Address())
       {
           strIpAddress = ipAddressesList.at(i).toString();
           break;
       }
     }
     if (strIpAddress.isEmpty())
        strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
     return strIpAddress;
}

bool QPushActiveInfo::IPLive(QString ip, int port)
{
    QTcpSocket tcpClient;
    tcpClient.abort();
    tcpClient.connectToHost(ip, port);
    //100毫秒没有连接上则判断不在线
    return tcpClient.waitForConnected(100);
}

QString QPushActiveInfo::getOSInfo()
{
    QString osInfo;
    QString machineName = QHostInfo::localHostName();
//    qDebug() << "buildCpuArchitecture: " << QSysInfo::buildCpuArchitecture();
    osInfo = QSysInfo::prettyProductName() +" [" +QSysInfo::productType() +" "+ QSysInfo::productVersion() +"(" + QString::number(QSysInfo::windowsVersion()) + ")]";
    osInfo += " | " + QSysInfo::buildAbi() + " | " + QSysInfo::currentCpuArchitecture() + " | " + QSysInfo::kernelType() + " " + QSysInfo::kernelVersion() + " | " + machineName;
    return osInfo;
}

IpInfoStruct QPushActiveInfo::GetNetIPInfo()
{
    IpInfoStruct iis;
    QString url = "http://whois.pconline.com.cn/";
    QByteArray res = GetHtml(url);
    QString netIp = GetNetIPPconline(res);
    QString areaInfo = GetAttributionPconline(res);
    iis.netIp = netIp;
    iis.areaInfo = areaInfo;
    return iis;
}

QByteArray QPushActiveInfo::GetHtml(QString url)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
    QByteArray responseData;
    QEventLoop eventLoop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    responseData = reply->readAll();
    return responseData;
}

QString QPushActiveInfo::GetNetIPPconline(QByteArray responseData)
{
   QTextCodec* pTextCodec = QTextCodec::codecForName("GB2312");
   if (NULL == pTextCodec)
   {
       return "";
   }
   QString webCode = pTextCodec->toUnicode(responseData);
   QString ipinfo = "";
   QString web = webCode.replace(" ", "");
   web = web.replace("\r", "");
   web = web.replace("\n", "");
   web = web.replace("\t", "");
   QStringList list = web.split("</form>");
   if(list.count() > 0)
   {
       ipinfo = list[0];
       list = ipinfo.split("value=");
       ipinfo = list.last();
       list = ipinfo.split("><");
       ipinfo = list[0];
   }
   return ipinfo.replace("\"","");

}

QString QPushActiveInfo::GetAttributionPconline(QByteArray responseData)
{
    QTextCodec* pTextCodec = QTextCodec::codecForName("GB2312");
    if (NULL == pTextCodec)
    {
        return "";
    }
    QString webCode = pTextCodec->toUnicode(responseData);
    //    webCode = webCode.fromLocal8Bit(responseData.data());

    QString attribution = "";
    QString web = webCode; //.replace(" ", "");
    web = web.replace("\r", "");
    web = web.replace("\n", "");
    web = web.replace("\t", "");
    QStringList list = web.split("</form>");
    if(list.count() > 0)
    {
        attribution = list[0];
        list = attribution.split("<p>");
        attribution = list.last();
        list = attribution.split("</p>");
        attribution = list[0];
    }

    QStringList addInfo =  attribution.split("：");
    if(addInfo.count() > 1)
    {
        return addInfo[1].trimmed();
    }
    return attribution;
}
