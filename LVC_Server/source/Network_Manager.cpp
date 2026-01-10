#include <Network_Manager.h>

#include <Utility.h>
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
    m_client_manager.remember_client(_respond_to);
    m_socket.send(_package, _respond_to);
}

void Network_Manager::M_respond_to_voice(const LNet::IP_Address& _respond_to, const LNet::Package& _package)
{
    Package_Header header;
    header.command_type = Command_Type::Sound_Data;
    header.timestamp = current_timestamp();

    LNet::Package package;
    package.append_header(header);
    package.append_data(_package.raw_data_without_header<Package_Header>(), _package.raw_data_size_without_header<Package_Header>());

    m_client_manager.process_for_all([&](const LNet::IP_Address& _address)
    {
        m_socket.send(package, _address);
    }, _respond_to.address_str());
}



void Network_Manager::process()
{
    m_client_manager.process();

    while(true)
    {
        LNet::Server_Socket::Message package = m_socket.receive();
        if(package.package.raw_data_size() == 0)
            break;

        Package_Header header = package.package.parse_header<Package_Header>();
        if(header.command_type == Command_Type::Handshake)
            M_respond_to_handshake(package.client_address, package.package);
        else if(header.command_type == Command_Type::Sound_Data)
            M_respond_to_voice(package.client_address, package.package);
        else
            std::cout << "mystery..." << std::endl;
    }

}
