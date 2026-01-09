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

    public:
        void process();

    };

}
