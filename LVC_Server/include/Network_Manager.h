#pragma once

#include <Server_Socket.h>

#include <Common_Constants.h>
#include <Client_Manager.h>


namespace LVC
{

    class Network_Manager
    {
    private:
        LNet::Server_Socket m_socket = { Port, Max_Message_Size };

        Client_Manager m_client_manager;

    public:
        Network_Manager();
        ~Network_Manager();

    private:
        void M_respond_to_handshake(const LNet::IP_Address& _respond_to, const LNet::Package& _package);

    public:
        void process();

    };

}
