#include <Network_Manager.h>

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

    std::cout << "received message" << std::endl;

    m_client_manager.remember_client(message.client_address);
}
