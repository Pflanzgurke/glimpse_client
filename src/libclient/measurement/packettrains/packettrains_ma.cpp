#include "packettrains_ma.h"
#include <QUdpSocket>
#include <arpa/inet.h>
#include <iostream>
using namespace std;
#include <iomanip>
#include "../../log/logger.h"
#include "../../network/networkmanager.h"

LOGGER(PacketTrainsMA);


PacketTrainsMA::PacketTrainsMA()
{
}

bool PacketTrainsMA::start()
{
    // ctrl
    QByteArray buffer;
    buffer.resize(1500);

    struct msg* message = reinterpret_cast<msg*>(buffer.data());
    message->type = MSG_CTRL;
    message->data.c.iter = htons(definition->iterations);
    message->data.c.size = htons(definition->packetSize);

    m_udpSocket->writeDatagram(buffer, QHostAddress(definition->host), definition->port);

    // trains
    buffer.clear();
    buffer.resize(definition->packetSize);
    message = reinterpret_cast<msg*>(buffer.data());
    message->type = MSG_MSRMNT;

    // calculate disperson
    quint64 disp[definition->iterations];
    quint64 R_MIN = 10485760;
    quint64 R_MAX = 262144000;
    struct timespec dispersion;
    dispersion.tv_sec = 0;
    dispersion.tv_nsec = 0;
    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = 200000000;

    for(int i = 0; i < definition->iterations; i++)
    {
        disp[i] = (uint64_t) (definition->packetSize * 1000000000.0 / ((R_MAX - R_MIN) / definition->iterations * i + R_MIN)); // Linear Rate
    }

    // send trains
    for(int i = 0; i < definition->trainLength * definition->iterations; i++)
    {
        message->data.m.iter = htons(i / definition->trainLength);
        message->data.m.id = i % definition->trainLength;
        clock_gettime(CLOCK_MONOTONIC_RAW, &message->data.m.otime);
        m_udpSocket->writeDatagram(buffer, QHostAddress(definition->host), definition->port);

        dispersion.tv_nsec = disp[i / definition->trainLength];
        nanosleep(&dispersion, NULL);
        if(i % definition->trainLength == definition->trainLength -1)
        {
            nanosleep(&delay, NULL);
        }
    }

    return true;
}

void PacketTrainsMA::handleError(QAbstractSocket::SocketError socketError)
{
    if(socketError == QAbstractSocket::RemoteHostClosedError)
        return;

    QAbstractSocket* socket = qobject_cast<QAbstractSocket*>(sender());
    cout<<"Socket Error: "<<socket->errorString().toStdString()<<endl;
}


Measurement::Status PacketTrainsMA::status() const
{
    return Unknown;
}

bool PacketTrainsMA::prepare(NetworkManager *networkManager, const MeasurementDefinitionPtr &measurementDefinition)
{
    definition = measurementDefinition.dynamicCast<PacketTrainsDefinition>();
    if ( definition.isNull() ) {
        LOG_WARNING("Definition is empty");
    }

    QString hostname = QString("%1:%2").arg(definition->host).arg(definition->port);

    m_udpSocket = qobject_cast<QUdpSocket*>(networkManager->establishConnection(hostname, "packetTrains_ma", definition->toVariant(), NetworkManager::UdpSocket));
    if (!m_udpSocket) {
        LOG_ERROR("Preparation failed");
        return false;
    }

    m_udpSocket->setParent(this);

    // Signal for errors
    connect(m_udpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleError(QAbstractSocket::SocketError)));

    return true;
}

bool PacketTrainsMA::stop()
{
    return true;
}

ResultPtr PacketTrainsMA::result() const
{
    return ResultPtr();
}
