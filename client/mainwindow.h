#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "viewim.h"

#include <QFileSystemModel>
#include <QMainWindow>
#include <QTcpSocket>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDirIterator>
#include <QLabel>
#include <QTime>


//QT_BEGIN_NAMESPACE
//namespace Ui { class MainWindow; }
//QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT
private:
    QTcpSocket* m_pTcpSocket;
//    QTextEdit*  m_ptxtInfo;
//    QLineEdit*  m_ptxtInput;
    quint16     m_nNextBlockSize;
    Ui::MainWindow *ui;
    QFileSystemModel *model;

public:
    MainWindow(const QString& strHost, int nPort, QWidget* parent = 0);
    ~MainWindow();

private slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError err);
    void slotSendToServer();
    void slotConnected();
    void on_listIm_doubleClicked(const QModelIndex &index);
    void on_viewButton_clicked();
    void on_downloadBtn_clicked();
};

//возвращает все элементы внутри dir
void ContentList(QDir &dir, QFileInfoList &contentList);
#endif // MAINWINDOW_H
