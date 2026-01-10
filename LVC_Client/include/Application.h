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

    public:
        void run();

    };

}
