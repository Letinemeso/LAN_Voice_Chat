#pragma once

#include <Data_Structures/Vector.h>

#include <IP_Address.h>

#include <Data_Structures/List.h>
#include <Stuff/Stopwatch.h>
#include <Stuff/Function_Wrapper.h>


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

    public:
        using Process_Function = LST::Function<void(const LNet::IP_Address&)>;

    private:
        Clients_List m_clients;

    private:
        Clients_List::Iterator M_find_client_it(const std::string& _address);
        Clients_List::Const_Iterator M_find_client_it(const std::string& _address) const;

    public:
        void remember_client(const LNet::IP_Address& _address);
        void forget_client(const std::string& _address);

        void process();

    public:
        void process_for_client(const Process_Function& _func, const std::string& _address) const;
        void process_for_all(const Process_Function& _func) const;
        void process_for_all(const Process_Function& _func, const std::string& _exclude) const;
        void process_for_all(const Process_Function& _func, const LDS::Vector<std::string>& _exclude) const;

    };

}
