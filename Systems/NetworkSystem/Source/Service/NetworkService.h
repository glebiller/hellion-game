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

#pragma once

#include <boost/asio.hpp>

#include "Proto/Server/UpstreamMessage.pb.h"
#include "Proto/Server/DownstreamMessage.pb.h"

class NetworkSystem;

class NetworkService {
public:
    
    /**
     * Default constructor.
     */
    NetworkService(NetworkSystem* networkSystem);
    
    /**
     * Default destructor.
     */
    ~NetworkService(void);

    /**
     * Connect to the remote server.
     *
     * @param   host    The hostname to connect to.
     * @param   port    The port to connect to.
     */
    void connect(std::string host, std::string port);

    /**
     * Send the given message to the server.
     *
     * @param   downstreamMessageProto  The message to add to queue.
     */
    void send(const DownstreamMessageProto& downstreamMessageProto);

    /**
     * Receive the messages from the server.
     */
    void receive(void);

    /**
     * If we are connected, then disconnect.
     */
    void disconnect(void);

private:

    /**
     * Handle the connection result to the server.
     *
     * @param   error   The error if the connection failed.
     */
    void onConnected(const boost::system::error_code& error);

    /**
     * Handle the read from the server.
     *
     * @param   error   The error if the read failed.
     */
    void onRead(const boost::system::error_code& error);

    /**
     * Handle the authentication success.
     *
     * @param   upstreamMessageProto    The authenticated message proto to parse.
     */
    void onAuthenticated(const UpstreamMessageProto& upstreamMessageProto);

    /**
     * Handle the object created.
     *
     * @param   upstreamMessageProto    The object updated message proto to parse.
     */
    void onObjectCreated(const UpstreamMessageProto& upstreamMessageProto);

    /**
     * Handle the object updated.
     *
     * @param   upstreamMessageProto    The object updated message proto to parse.
     */
    void onObjectUpdated(const UpstreamMessageProto& upstreamMessageProto);

    /**
     * Handle the object deleted.
     *
     * @param   upstreamMessageProto    The object updated message proto to parse.
     */
    void onObjectDeleted(const UpstreamMessageProto& upstreamMessageProto);
    
    NetworkSystem*                  m_pSystem;
    bool                            m_connected;
    boost::asio::io_service         m_ioService;
    boost::asio::ip::tcp::socket*   m_pSocket;
    boost::asio::streambuf          m_readBuffer;
    std::vector<char>               m_writeBuffer;
    
    typedef boost::function<void (const UpstreamMessageProto& upstreamMessageProto)> MessageHandler;
    typedef std::map<UpstreamMessageProto::Type, MessageHandler> MessageHandlerMap;
    MessageHandlerMap               m_messageHandlers;

};

