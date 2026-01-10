#include <Network_Manager.h>

#include <thread>
#include <chrono>

#include <Package_Header.h>

using namespace LVC;


void Network_Manager::M_process_package__voice(const LNet::Package& _package)
{
    L_ASSERT(m_packages_queue);

    m_packages_queue->add_data(_package);
}


void Network_Manager::M_process_package(const LNet::Package& _package)
{
    Package_Header header = _package.parse_header<Package_Header>();

    if(header.command_type == Command_Type::Sound_Data)
        M_process_package__voice(_package);
    else
        std::cout << "got some shit" << std::endl;
}



void Network_Manager::connect()
{
    if(m_is_connected)
        return;

    m_socket.connect(m_server_ip, Port);

    LNet::Package handshake_package;
    Package_Header handshake_package_header;
    handshake_package_header.command_type = Command_Type::Handshake;
    handshake_package.append_header(handshake_package_header);

    bool handshake_success = false;
    for(unsigned int i = 0; i < 3; ++i)
    {
        m_socket.send(handshake_package);

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
}

void Network_Manager::process()
{
    if(!m_is_connected)
        return connect();

    while(true)
    {
        LNet::Package package = m_socket.receive();
        if(package.empty())
            break;

        M_process_package(package);
    }
}


void Network_Manager::send_package(const LNet::Package& _package)
{
    m_socket.send(_package);
}
