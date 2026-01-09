#pragma once

#include <Object_Constructor.h>
#include <Variable_Loader.h>

#include <Network_Manager.h>


namespace LVC
{

    class Application
    {
    private:
        Network_Manager m_network_manager;

    public:
        Application();
        ~Application();

    private:
        void M_register_messages();
        void M_register_types();
        void M_on_components_initialized();

    public:
        void run();

    };

}
