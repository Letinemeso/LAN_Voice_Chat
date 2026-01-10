#include <Client_Manager.h>

#include <Common_Constants.h>

using namespace LVC;


Client_Manager::Clients_List::Iterator Client_Manager::M_find_client_it(const std::string& _address)
{
    for(Clients_List::Iterator it = m_clients.begin(); !it.end_reached(); ++it)
    {
        if(it->address.address_str() == _address)
            return it;
    }

    return {};
}



void Client_Manager::remember_client(const LNet::IP_Address& _address)
{
    const std::string& address_str = _address.address_str();
    if(address_str.size() == 0)
        return;

    Clients_List::Iterator maybe_registred_client = M_find_client_it(address_str);
    if(maybe_registred_client.is_ok())
    {
        maybe_registred_client->existence_duration.start();
        return;
    }

    Client_Data data;
    data.address = _address;
    data.existence_duration.start();
    m_clients.push_back(data);
}

void Client_Manager::forget_client(const std::string& _address)
{
    Clients_List::Iterator maybe_registred_client = M_find_client_it(_address);
    L_ASSERT(maybe_registred_client.is_ok());

    m_clients.erase(maybe_registred_client);
}


void Client_Manager::process()
{
    Clients_List::Iterator it = m_clients.begin();

    while(!it.end_reached())
    {
        Client_Data& client = *it;

        if(client.existence_duration.duration() >= Client_Existence_Duration)
            it = m_clients.erase_and_iterate_forward(it);
        else
            ++it;
    }
}
