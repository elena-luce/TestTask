#include "myserver.h"

MyServer::MyServer(int nPort/*, QWidget *pwgt): QTcpServer(pwgt*/): m_nNextBlockSize(0)
{
    m_ptcpServer = new QTcpServer(this);
    if (!m_ptcpServer->listen(QHostAddress::Any, nPort)) {
        qDebug("Server Error");
        qDebug() << "Unable to start the server:" << m_ptcpServer->errorString();
        m_ptcpServer->close();
        return;
    }
    connect(m_ptcpServer, SIGNAL(newConnection()),
            this,         SLOT(slotNewConnection())
           );
    qDebug() << "Server";
}

void MyServer::sendToClient(QTcpSocket *pSocket, const QString &str)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    out << quint16(0) << QTime::currentTime() << str;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    pSocket->write(arrBlock);
}

//void MyServer::sendIm(QTcpSocket *pSocket)
//{

//}

void MyServer::slotNewConnection()
{
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(deleteLater()));
//    connect(pClientSocket, SIGNAL(readyRead()),
//            this,          SLOT(slotReadClient()));
    connect(pClientSocket, SIGNAL(readyRead()),
            this,          SLOT(slotRecieveFile()));
    sendToClient(pClientSocket, "Server Response: Connected!");

}

void MyServer::slotReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QTime   time;
        QString str;
        in >> time >> str;

        QString strMessage =
            time.toString() + " " + "Client has sended - " + str;
        qDebug() << strMessage;

        m_nNextBlockSize = 0;

        sendToClient(pClientSocket,
                     "Server Response: Received \"" + str + "\"");
    }
}

bool downloading_in_process = true;
//bool cycle = 0;

void MyServer::slotRecieveFile()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_DefaultCompiledVersion);
//    qDebug() << "Entered downloading";
    QTime   time;
    QString str;//, name;
    in >> time;// >> m_fileName;
//    qDebug() << "cycle" << (downloading_in_process == false || cycle == 0);
//    if(downloading_in_process == false || cycle == 0)
//        name = str;
//    qDebug() << str;
//    QDir dir;
    QByteArray ba("!@#Rtasd#$%sdfs!!!)()334rrer");
    QByteArray finalBA;

//    dir.mkpath("/home/helena/Загрузки/server/MyServer/");
    QFile loadedFile("/home/helena/_LETI_/4сем/test_task/TestTask/server/MyServer/" + m_fileName);
//    qDebug() << loadedFile.isOpen();
    qDebug() << loadedFile.fileName();
    loadedFile.open(QIODevice::WriteOnly | QIODevice::Append);
    qDebug() << loadedFile.isOpen();
//    while(downloading_in_process == true){
        while(pClientSocket->bytesAvailable())
        {
            line = pClientSocket->readAll();
            qDebug() << "Line: " << line << "line size: " << line.size();

            dataVector.append(line);
//            qDebug() << "Vector: " << dataVector << "vector size: " << dataVector.size();
//      cycle = 1;
//            qDebug() << "downloading_in_process" << downloading_in_process;
            if (line.contains(ba)) {
                qDebug() << "Downloaded!";
                downloading_in_process = false;
//                qDebug() << "downloading_in_process" << downloading_in_process;

                dataVector.append(line);
            }
        }
//    }
//    qDebug() << "cycle" << (downloading_in_process == false || cycle == 0);
    foreach (QByteArray dataBA, dataVector) {
        finalBA.append(dataBA);
    }
    while (finalBA.contains(ba))
    {
        int a = line.lastIndexOf(ba);
//        qDebug() << "line.lastIndexOf(ba): " << line.lastIndexOf(ba);

        finalBA.remove(a, ba.size()+100);
    }
    int a = line.lastIndexOf(ba);
    finalBA.remove(a, ba.size()+100);
//    QDataStream it(&loadedFile);
//    it << dataVector;
    loadedFile.write(finalBA);
    loadedFile.waitForBytesWritten(30000);
    dataVector.clear();
}
