#ifndef MYSERVER_H
#define MYSERVER_H

#include <QWidget>
#include <QDebug>
#include <QTime>

#include <QTcpServer>
#include <QTcpSocket>
#include <QDir>

//class QTcpServer;
//class QTextEdit;
//class QTcpSocket;

class MyServer : public QTcpServer {
Q_OBJECT
private:
    QTcpServer* m_ptcpServer;
//    QTextEdit*  m_ptxt;
    quint16     m_nNextBlockSize;
    QByteArray line;
    QVector<QByteArray> dataVector;
    QString m_fileName = "Valeria.txt";

public:
    MyServer(int nPort/*, QWidget* pwgt = 0*/);

private:
    void sendToClient(QTcpSocket* pSocket, const QString& str);

//    void sendIm(QTcpSocket* pSocket);

public slots:
    virtual void slotNewConnection();
            void slotReadClient();
            void slotRecieveFile();
};

#endif // MYSERVER_H
