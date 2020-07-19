#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(const QString& strHost, int nPort, QWidget* parent) : QMainWindow(parent)
                    , m_nNextBlockSize(0), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pTcpSocket = new QTcpSocket(this);

    m_pTcpSocket->connectToHost(strHost, nPort);

    connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));

    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(slotError(QAbstractSocket::SocketError)));

    connect(ui->m_ptxtInput, SIGNAL(returnPressed()), this, SLOT(slotSendToServer()));

    ui->m_ptxtInfo->setReadOnly(true);
//    ui->pcmd->setText("&Send");

    connect(ui->pcmd, SIGNAL(clicked()), SLOT(slotSendToServer()));

    model = new QFileSystemModel(this);
    model->setFilter(QDir::Files|QDir::Dirs|QDir::NoDot);
    model->setRootPath("");
    ui->listIm->setModel(model);
    ui->listIm->setRootIndex(model->index("/home/helena/Загрузки/Изображения"));

    ui->label->setText("<H1>Client</H1>");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ViewDir()
{
    QModelIndex index = ui->listIm->selectionModel()->currentIndex();
    QFileInfo fileInfo = model->fileInfo(index);
    QString path = fileInfo.absoluteFilePath();
//    ui->lineEdit_1->setText(path);
}

void MainWindow::slotReadyRead()
{
    QDataStream in(m_pTcpSocket);
    in.setVersion(QDataStream::Qt_4_2);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (m_pTcpSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QTime   time;
        QString str;
        in >> time >> str;

        ui->m_ptxtInfo->append(time.toString() + " " + str);
        m_nNextBlockSize = 0;
    }
}

void MainWindow::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
        "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(m_pTcpSocket->errorString())
                    );
    ui->m_ptxtInfo->append(strError);
}

void MainWindow::slotSendToServer()
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint16(0) << QTime::currentTime() << ui->m_ptxtInput->text();

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    m_pTcpSocket->write(arrBlock);
    ui->m_ptxtInput->setText("");
}

void MainWindow::slotConnected()
{
    ui->m_ptxtInfo->append("Received the connected() signal");
}

//void MainWindow::on_pushButton_clicked()
//{

//}

void ContentList(QDir &dir, QFileInfoList &contentList)
{
    foreach(QFileInfo info, dir.entryInfoList(QDir::Files|QDir::Dirs|QDir::NoDot, QDir::Name|QDir::DirsFirst)){
        contentList.append(info);
        if(info.isDir() && dir.cd(info.fileName())){
            ContentList(dir, contentList);
            dir.cdUp();
        }
    }
}

void MainWindow::on_listIm_doubleClicked(const QModelIndex &index)
{
    QListView * listView = (QListView*)sender();
    QFileInfo fileInfo = model->fileInfo(index);
    if(fileInfo.fileName() == ".."){
        QDir dir = fileInfo.dir();
        dir.cdUp();//можно также cd("..")
        listView->setRootIndex(model->index(dir.absolutePath()));
    }
    else if (fileInfo.isDir()) {
        listView->setRootIndex(index);
    }
}
