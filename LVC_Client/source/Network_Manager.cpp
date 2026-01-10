#include <Network_Manager.h>

#include <thread>
#include <chrono>

#include <Package_Header.h>

using namespace LVC;


Network_Manager::Network_Manager()
{
    m_handshake_package = M_construct_handshake_package();
}

Network_Manager::~Network_Manager()
{

}



void Network_Manager::M_process_package__heartbeat(const LNet::Package& _package)
{
    m_connection_stopwatch.start();
}

void Network_Manager::M_process_package__voice(const LNet::Package& _package)
{
    m_player->add_data(_package);
}


void Network_Manager::M_process_package(const LNet::Package& _package)
{
    Package_Header header = _package.parse_header<Package_Header>();

    if(header.command_type == Command_Type::Sound_Data)
        M_process_package__voice(_package);
    else if(header.command_type == Command_Type::Handshake)
        M_process_package__heartbeat(_package);
    else
        std::cout << "got some shit" << std::endl;
}


LNet::Package Network_Manager::M_construct_handshake_package() const
{
    LNet::Package result;

    Package_Header handshake_package_header;
    handshake_package_header.command_type = Command_Type::Handshake;
    result.append_header(handshake_package_header);

    return result;
}


void Network_Manager::M_connect()
{
    if(m_is_connected)
        return;

    m_socket.connect(m_server_ip, Port);

    bool handshake_success = false;
    for(unsigned int i = 0; i < 3; ++i)
    {
        m_socket.send(m_handshake_package);

        std::this_thread::sleep_for(std::chrono::milliseconds(Handshake_Expectation_Ms));

        LNet::Package response = m_socket.receive();
        if(response.empty())
            continue;

        Package_Header response_header = response.parse_header<Package_Header>();
        if(response_header.command_type != Command_Type::Handshake)
            continue;

        handshake_success = true;
        break;
    }

    m_is_connected = handshake_success;

    if(m_is_connected)
        std::cout << "successfuly connected to " << m_server_ip << std::endl;
    else
        std::cout << "unable to connect to " << m_server_ip << std::endl;

    m_heartbeat_stopwatch.start();
    m_connection_stopwatch.start();
}

void Network_Manager::M_process_heartbeat()
{
    if(m_heartbeat_stopwatch.duration() < Client_Heartbeat_Frequency)
        return;

    m_heartbeat_stopwatch.start();

    m_socket.send(m_handshake_package);
}

void Network_Manager::M_process_connection_check()
{
    if(m_connection_stopwatch.duration() < Client_Existence_Duration)
        return;

    m_is_connected = false;

    std::cout << "lost connection with " << m_server_ip << std::endl;
}



void Network_Manager::process()
{
    if(!m_is_connected)
        return M_connect();

    while(true)
    {
        LNet::Package package = m_socket.receive();
        if(package.empty())
            break;

        M_process_package(package);
    }

    M_process_heartbeat();
    M_process_connection_check();
}


void Network_Manager::send_package(const LNet::Package& _package)
{
    m_socket.send(_package);
}
