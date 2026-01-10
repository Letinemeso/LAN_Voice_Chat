#include <Player.h>

#include <Devices/Input_Device.h>

#include <Common_Constants.h>
#include <Package_Header.h>

using namespace LVC;


Player::Player()
{

}

Player::~Player()
{
    while(m_active_sounds.size() != 0)
    {
        Sound_Data& data = m_active_sounds.back();

        delete data.data;
        delete data.sound;

        m_active_sounds.pop_back();
    }
}



// void Player::M_on_sound_end()
// {
//     if(!m_sound.sound_data())
//         return;

//     if(m_sound.is_playing())
//         return;

//     m_sound.set_sound_data(nullptr);
//     delete m_sound_data;
//     m_sound_data = nullptr;
// }

// void Player::M_start_next_sound_if_needed()
// {
//     if(m_sound.sound_data())
//         return;

//     LSound::Sound_Data* next_sound_data = m_packages_queue->extract_top_data();
//     if(!next_sound_data)
//         return;

//     m_sound_data = next_sound_data;
//     m_sound.set_sound_data(next_sound_data);
//     m_sound.play();
// }



void Player::add_data(const LNet::Package& _voice_package)
{
    L_DEBUG_FUNC_NOARG([&]()
    {
        Package_Header header = _voice_package.parse_header<Package_Header>();
        L_ASSERT(header.command_type == Command_Type::Sound_Data);
    });

    unsigned int size_without_header = _voice_package.raw_data_size_without_header<Package_Header>();
    L_ASSERT(size_without_header > 0);

    const char* data_with_offset = _voice_package.raw_data_without_header<Package_Header>();

    LSound::Input_Device_Settings default_settings = LSound::Input_Device_Settings::voip_standard(0.1f);

    LSound::Raw_Sound_Data raw_sound_data;
    raw_sound_data.format = default_settings.format;
    raw_sound_data.frequency = default_settings.frequency;

    raw_sound_data.size = size_without_header;
    raw_sound_data.data = new char[size_without_header];
    memcpy(raw_sound_data.data, data_with_offset, size_without_header);

    Sound_Data sound_data;

    sound_data.data = new LSound::Sound_Data;
    sound_data.data->set_raw_data(raw_sound_data);

    sound_data.sound = new LSound::Sound;
    sound_data.sound->set_sound_data(sound_data.data);
    sound_data.sound->play();

    m_active_sounds.push_back(sound_data);
}



void Player::process()
{
    LDS::List<Sound_Data>::Iterator it = m_active_sounds.begin();
    while(!it.end_reached())
    {
        Sound_Data& data = *it;

        if(data.sound->is_playing())
            ++it;
        else
            it = m_active_sounds.erase_and_iterate_forward(it);
    }
}
