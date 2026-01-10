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
        };

    private:
        LDS::List<Sound_Data> m_active_sounds;

        LSound::Sound* m_sound_0 = new LSound::Sound;
        LSound::Sound* m_sound_1 = new LSound::Sound;

    public:
        Player();
        ~Player();

    public:
        void add_data(const LNet::Package& _voice_package);

    public:
        void process();

    };

}
