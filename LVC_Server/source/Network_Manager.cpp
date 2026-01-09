#include <Network_Manager.h>

#include <Package_Header.h>

using namespace LVC;

Network_Manager::Network_Manager()
{
    m_socket.set_listen_timeout_ms(250);
}

Network_Manager::~Network_Manager()
{

}



void Network_Manager::process()
{
    m_client_manager.process();

    LNet::Server_Socket::Message message = m_socket.receive();
    if(message.package.raw_data_size() == 0)
        return;

    m_client_manager.remember_client(message.client_address);

    Package_Header header = message.package.parse_header<Package_Header>();
    if(header.command_type == Command_Type::Handshake)
        std::cout << "client shake it shake it!" << std::endl;
    else if(header.command_type == Command_Type::Sound_Data)
        std::cout << "client has farted" << std::endl;
    else
        std::cout << "mystery..." << std::endl;
}
