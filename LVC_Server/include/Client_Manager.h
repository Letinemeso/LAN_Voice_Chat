#pragma once

#include <IP_Address.h>

#include <Data_Structures/List.h>
#include <Stuff/Stopwatch.h>


namespace LVC
{

    class Client_Manager
    {
    private:
        struct Client_Data
        {
            LNet::IP_Address address;
            LST::Stopwatch existence_duration;
        };
        using Clients_List = LDS::List<Client_Data>;

    private:
        Clients_List m_clients;

    private:
        Clients_List::Iterator M_find_client_it(const std::string& _address);

    public:
        void remember_client(const LNet::IP_Address& _address);
        void forget_client(const std::string& _address);

        void process();

    };

}
