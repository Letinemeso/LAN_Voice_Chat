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

        LST::Stopwatch m_connection_stopwatch;
        LST::Stopwatch m_heartbeat_stopwatch;
        LNet::Package m_handshake_package;

        Player* m_player = nullptr;

    public:
        Network_Manager();
        ~Network_Manager();

    public:
        inline void inject_player(Player* _ptr) { m_player = _ptr; }

        inline void set_server_ip(const std::string& _value) { m_server_ip = _value; }

        inline bool connected() const { return m_is_connected; }

    private:
        void M_process_package__heartbeat(const LNet::Package& _package);
        void M_process_package__voice(const LNet::Package& _package);

        void M_process_package(const LNet::Package& _package);

        LNet::Package M_construct_handshake_package() const;

        void M_connect();
        void M_process_heartbeat();
        void M_process_connection_check();

    public:
        void process();

        void send_package(const LNet::Package& _package);

    };

}
