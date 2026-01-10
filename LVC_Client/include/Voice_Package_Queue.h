#pragma once

#include <Data_Structures/List.h>

#include <Sound_Data.h>

#include <Package.h>


namespace LVC
{

    class Voice_Package_Queue
    {
    private:
        LDS::List<LSound::Sound_Data*> m_queue;

    public:
        Voice_Package_Queue();
        ~Voice_Package_Queue();

    public:
        void add_data(const LNet::Package& _voice_package);
        void clear();

    public:
        LSound::Sound_Data* extract_top_data();

    };

}
