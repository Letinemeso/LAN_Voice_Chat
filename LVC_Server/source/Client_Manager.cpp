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

Client_Manager::Clients_List::Const_Iterator Client_Manager::M_find_client_it(const std::string& _address) const
{
    for(Clients_List::Const_Iterator it = m_clients.begin(); !it.end_reached(); ++it)
    {
        if(it->address.address_str() == _address)
            return it;
    }

    return {};
}



bool Client_Manager::client_is_registered(const LNet::IP_Address& _address) const
{
    Clients_List::Const_Iterator maybe_client = M_find_client_it(_address.address_str());
    return maybe_client.is_ok();
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

    std::cout << "client [" << _address.address_str() << "] registered" << std::endl;
}

void Client_Manager::forget_client(const std::string& _address)
{
    Clients_List::Iterator maybe_registred_client = M_find_client_it(_address);
    L_ASSERT(maybe_registred_client.is_ok());

    m_clients.erase(maybe_registred_client);

    std::cout << "client [" << _address << "] disconnected" << std::endl;
}


void Client_Manager::process()
{
    Clients_List::Iterator it = m_clients.begin();

    while(!it.end_reached())
    {
        Client_Data& client = *it;

        if(client.existence_duration.duration() < Client_Existence_Duration)
        {
            ++it;
            continue;
        }

        std::cout << "client [" << client.address.address_str() << "] disconnected" << std::endl;
        it = m_clients.erase_and_iterate_forward(it);
    }
}



void Client_Manager::process_for_client(const Process_Function& _func, const std::string& _address) const
{
    L_ASSERT(_func);

    Clients_List::Const_Iterator maybe_client = M_find_client_it(_address);
    if(!maybe_client.is_ok())
        return;

    _func(maybe_client->address);
}

void Client_Manager::process_for_all(const Process_Function& _func) const
{
    L_ASSERT(_func);

    for(Clients_List::Const_Iterator it = m_clients.begin(); !it.end_reached(); ++it)
    {
        const Client_Data& client = *it;
        _func(client.address);
    }
}

void Client_Manager::process_for_all(const Process_Function& _func, const std::string& _exclude) const
{
    L_ASSERT(_func);

    for(Clients_List::Const_Iterator it = m_clients.begin(); !it.end_reached(); ++it)
    {
        const Client_Data& client = *it;

        if(client.address.address_str() == _exclude)
            continue;

        _func(client.address);
    }
}

void Client_Manager::process_for_all(const Process_Function& _func, const LDS::Vector<std::string>& _exclude) const
{
    L_ASSERT(_func);

    for(Clients_List::Const_Iterator it = m_clients.begin(); !it.end_reached(); ++it)
    {
        const Client_Data& client = *it;

        if(_exclude.find(client.address.address_str()).is_ok())
            continue;

        _func(client.address);
    }
}
