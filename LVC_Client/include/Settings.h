#pragma once

#include <Variable_Base.h>


namespace LVC
{

    class Settings : public LV::Variable_Base
    {
    public:
        INIT_VARIABLE(LVC::Settings, LV::Variable_Base)

        INIT_FIELDS
        ADD_FIELD(float, volume)
        ADD_FIELD(std::string, server_ip)
        FIELDS_END

    public:
        float volume = 1.0f;
        std::string server_ip = "127.0.0.1";

    };

}
