#pragma once

#include <Client_Socket.h>

#include <Common_Constants.h>
#include <Player.h>


namespace LVC
{

    class Network_Manager
    {
    private:
        LNet::Client_Socket m_socket = {Max_Message_Size};

        bool m_is_connected = false;
        std::string m_server_ip;

        Player* m_player = nullptr;

    public:
        inline void inject_player(Player* _ptr) { m_player = _ptr; }

        inline void set_server_ip(const std::string& _value) { m_server_ip = _value; }

        inline bool connected() const { return m_is_connected; }

    private:
        void M_process_package__voice(const LNet::Package& _package);

        void M_process_package(const LNet::Package& _package);

    public:
        void connect();
        void process();

        void send_package(const LNet::Package& _package);

    };

}
