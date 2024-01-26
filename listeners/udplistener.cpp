#include "udplistener.h"
#include <QDebug>
UdpListener::UdpListener()
{
}

void UdpListener::run()
{
    receiver = new QUdpSocket(this);
    receiver->bind(QHostAddress::AnyIPv4,6666);
    QHostAddress address;
    while(!mThreadExit){
        while(receiver->hasPendingDatagrams())  //拥有等待的数据报
        {
             QByteArray datagram; //拥于存放接收的数据报
            //pendingDatagramSize为返回第一个在等待读取报文的size，
            //resize函数是把datagram的size归一化到参数size的大小一样
            datagram.resize(receiver->pendingDatagramSize());
            //接收数据报，将其存放到datagram中
            //将读取到的不大于datagram.size()大小数据输入到datagram.data()中，
            //datagram.data()返回的是一个字节数组中存储数据位置的指针
            receiver->readDatagram(datagram.data(),datagram.size(),&address);
            //将数据报内容显示出来
            RLOGD("udp data %s",datagram);
            //判断数据是否完整
            if(QString(datagram).contains("searchKlj:"))
            {
                qDebug() <<receiver->writeDatagram(QString("qyyy:"+isPlay?"start":"stop").toUtf8(),address,8888);
            }
            else
            {
                qDebug() << "The data is not complete !" << endl;
            }
         }
    }
}
