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



void Network_Manager::M_respond_to_handshake(const LNet::IP_Address& _respond_to, const LNet::Package& _package)
{
    std::cout << "client " << _respond_to.address_str() << " shake it shake it!" << std::endl;
    m_socket.send(_package, _respond_to);
}

void Network_Manager::M_respond_to_voice(const LNet::IP_Address& _respond_to, const LNet::Package& _package)
{
    m_client_manager.process_for_all([&](const LNet::IP_Address& _address)
    {
        m_socket.send(_package, _address);
    }, _respond_to.address_str());
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
        M_respond_to_handshake(message.client_address, message.package);
    else if(header.command_type == Command_Type::Sound_Data)
        M_respond_to_voice(message.client_address, message.package);
    else
        std::cout << "mystery..." << std::endl;
}
