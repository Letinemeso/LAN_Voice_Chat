#pragma once

#include <Sound.h>

#include <Package.h>


namespace LVC
{

    class Player
    {
    private:
        struct Sound_Data
        {
            LSound::Sound_Data* data = nullptr;
            LSound::Sound* sound = nullptr;
            unsigned int start_timestamp = 0;
        };

    private:
        LDS::List<Sound_Data> m_active_sounds;

    public:
        Player();
        ~Player();

    public:
        void add_data(const LNet::Package& _voice_package);

    private:
        void M_process_fade_out(Sound_Data& _sound_data);

    public:
        void process();

    };

}
