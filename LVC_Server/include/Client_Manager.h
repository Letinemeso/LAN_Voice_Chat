#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#include <Data_Structures/List.h>
#include <Stuff/Stopwatch.h>


namespace LVC
{

    class Client_Manager
    {
    private:
        struct Client_Data
        {
            sockaddr_in address;
            std::string address_str;
            LST::Stopwatch existence_duration;
        };
        using Clients_List = LDS::List<Client_Data>;

    private:
        Clients_List m_clients;

    private:
        std::string M_extract_ip_address_string(const sockaddr_in& _address) const;
        Clients_List::Iterator M_find_client_it(const std::string& _address);

    public:
        void remember_client(const sockaddr_in& _address);
        void forget_client(const sockaddr_in& _address);
        void forget_client(const std::string& _address);

        void process();

    };

}
