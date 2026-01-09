#pragma once

#include <Object_Constructor.h>
#include <Variable_Loader.h>


namespace Shardis
{

    class Application
    {
    private:
        LV::Object_Constructor m_object_constructor;

    public:
        Application();
        ~Application();

    private:
        void M_register_messages();
        void M_register_types();
        void M_on_components_initialized();

    private:
        void M_test_sound();
        void M_test_network();

    public:
        void run();

    };

}
