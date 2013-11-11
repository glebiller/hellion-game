// Copyright © 2008-2009 Intel Corporation
// All Rights Reserved
//
// Permission is granted to use, copy, distribute and prepare derivative works of this
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  THIS SOFTWARE IS PROVIDED "AS IS."
// INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, AND ALL LIABILITY,
// INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES, FOR THE USE OF THIS SOFTWARE,
// INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY RIGHTS, AND INCLUDING THE
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.

#include <boost/bind.hpp>
#include <google/protobuf/io/coded_stream.h>

#include "NetworkService.h"
#include "MatchingVarintPrefix.h"
#include "Scene.h"

#include "Proto/Message.pb.h"

/**
 * @inheritDoc
 */
NetworkService::NetworkService(NetworkSystem* networkSystem) 
    : m_connected(false)
    , m_pSystem(networkSystem)
    , m_pSocket(new boost::asio::ip::tcp::socket(m_ioService)) {
    m_messageHandlers[UpstreamMessageProto_Type_AUTHENTICATED] = boost::bind(&NetworkService::onAuthenticated, this, _1);
    m_messageHandlers[UpstreamMessageProto_Type_OBJECT_CREATED] = boost::bind(&NetworkService::onObjectCreated, this, _1);
    m_messageHandlers[UpstreamMessageProto_Type_OBJECT_UPDATED] = boost::bind(&NetworkService::onObjectUpdated, this, _1);
    m_messageHandlers[UpstreamMessageProto_Type_OBJECT_DELETED] = boost::bind(&NetworkService::onObjectDeleted, this, _1);

    // TODO change the receive part to be able
    // to handle more than 2048 if needed
    m_readBuffer.prepare(2048);
}

/**
 * @inheritDoc
 */
NetworkService::~NetworkService(void) {
    disconnect();
    delete m_pSocket;
}

/**
 * @inheritDoc
 */
void NetworkService::connect(std::string host, std::string port) {
    boost::asio::ip::tcp::resolver resolver(m_ioService);
    boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), host, port);
    boost::asio::async_connect(*m_pSocket, resolver.resolve(query), 
        boost::bind(&NetworkService::onConnected, this, boost::asio::placeholders::error));
}

/**
 * @inheritDoc
 */
void NetworkService::send(const DownstreamMessageProto& downstreamMessageProto) {
    if (!m_connected) {
        return;
    }

    int serializedSize = downstreamMessageProto.ByteSize();
    size_t totalOutputSize = google::protobuf::io::CodedOutputStream::VarintSize32(serializedSize) + serializedSize;

    if (m_writeBuffer.size() < totalOutputSize) {
        m_writeBuffer.resize(totalOutputSize);
    }

    google::protobuf::uint8* offset = (google::protobuf::uint8*) &m_writeBuffer[0];
    offset = google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(serializedSize, offset);
    downstreamMessageProto.SerializeToArray(offset, serializedSize);
    boost::asio::write(*m_pSocket, boost::asio::buffer(m_writeBuffer, totalOutputSize));
}

/**
 * @inheritDoc
 */
void NetworkService::receive(void) {
    m_ioService.poll();
}

/**
 * @inheritDoc
 */
void NetworkService::disconnect(void) {
    if (!m_connected) {
        return;
    }

    m_pSocket->close();
}

/**
 * @inheritDoc
 */
void NetworkService::onConnected(const boost::system::error_code& error) {
    if (error) {
        // Handle
        return;
    }

    m_connected = true;
    boost::asio::async_read_until(*m_pSocket, m_readBuffer, MatchingVarintPrefix(), boost::bind(&NetworkService::onRead, this, _1)); 
}

/**
 * @inheritDoc
 */
void NetworkService::onRead(const boost::system::error_code& error) {
    if (error) {
        // Handle
        return;
    }

    const char* data = boost::asio::buffer_cast<const char*>(m_readBuffer.data());
    int startIndex = 0;
    while (data[startIndex] & 0x80) {
        startIndex++;
    }
    startIndex++;
    
    UpstreamMessageProto upstreamMessageProto;
    upstreamMessageProto.ParseFromArray(data + startIndex, m_readBuffer.size() - startIndex);
    MessageHandlerMap::const_iterator handler = m_messageHandlers.find(upstreamMessageProto.type());
    if (handler != m_messageHandlers.end()) {
        handler->second(upstreamMessageProto);
    }

    // Get next message
    m_readBuffer.consume(m_readBuffer.size());
    onConnected(error);
}

/**
 * @inheritDoc
 */
void NetworkService::onAuthenticated(const UpstreamMessageProto& upstreamMessageProto) {
    Proto::Authenticated authenticated;
    authenticated.ParseFromString(upstreamMessageProto.data());
    if (authenticated.players().size() > 0) {
        m_pSystem->getSystemScene<NetworkScene>()->queueCreateObjects(authenticated.players());
    }
}

/**
 * @inheritDoc
 */
void NetworkService::onObjectCreated(const UpstreamMessageProto& upstreamMessageProto) {
    Proto::ObjectUpdated objectUpdated;
    objectUpdated.ParseFromString(upstreamMessageProto.data());
    if (objectUpdated.objects().size() > 0) {
        m_pSystem->getSystemScene<NetworkScene>()->queueCreateObjects(objectUpdated.objects());
    }
}

/**
 * @inheritDoc
 */
void NetworkService::onObjectUpdated(const UpstreamMessageProto& upstreamMessageProto) {
    Proto::ObjectUpdated objectUpdated;
    objectUpdated.ParseFromString(upstreamMessageProto.data());
    if (objectUpdated.objects().size() > 0) {
        m_pSystem->getSystemScene<NetworkScene>()->updateObjects(objectUpdated.objects());
    }
}

/**
 * @inheritDoc
 */
void NetworkService::onObjectDeleted(const UpstreamMessageProto& upstreamMessageProto) {
    Proto::ObjectUpdated objectUpdated;
    objectUpdated.ParseFromString(upstreamMessageProto.data());
    if (objectUpdated.objects().size() > 0) {
        m_pSystem->getSystemScene<NetworkScene>()->queueDeleteObjects(objectUpdated.objects());
    }
}