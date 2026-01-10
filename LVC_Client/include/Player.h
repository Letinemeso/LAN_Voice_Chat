#pragma once

#include <Sound.h>

#include <Voice_Package_Queue.h>


namespace LVC
{

    class Player
    {
    private:
        Voice_Package_Queue* m_packages_queue = nullptr;

        LSound::Sound_Data* m_sound_data = nullptr;
        LSound::Sound m_sound;

    public:
        Player();
        ~Player();

    public:
        inline void inject_packages_queue(Voice_Package_Queue* _ptr) { m_packages_queue = _ptr; }

    private:
        void M_on_sound_end();
        void M_start_next_sound_if_needed();

    public:
        void process();

    };

}
