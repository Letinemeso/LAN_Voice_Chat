#include <Player.h>

#include <Devices/Input_Device.h>

#include <Common_Constants.h>
#include <Package_Header.h>
#include <Utility.h>

using namespace LVC;

namespace LVC
{
    // constexpr float Package_Play_Speed_Multiplier = 0.9f;
    constexpr float Package_Play_Speed_Multiplier = 1.0f;
    constexpr float Expected_Package_Duration_Ms = (float)Voice_Package_Duration_Ms / Package_Play_Speed_Multiplier;
}


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
    sound_data.sound->settings().play_speed = Package_Play_Speed_Multiplier;
    sound_data.sound->play();

    sound_data.start_timestamp = current_timestamp();

    m_active_sounds.push_back(sound_data);
}



void Player::M_process_fade_out(Sound_Data& _sound_data)
{
    if(!_sound_data.sound->is_playing())
        return;

    constexpr float Fade_Out_Start_Ratio = 0.95f;
    constexpr float Fade_Out_Ratio_Left = 1.0f - Fade_Out_Start_Ratio;

    unsigned int timestamp = current_timestamp();
    unsigned int ms_passed = timestamp - _sound_data.start_timestamp;

    float passed_ratio = (float)ms_passed / Expected_Package_Duration_Ms;
    if(passed_ratio < Fade_Out_Start_Ratio)
        return;

    float ratio_left = passed_ratio - Fade_Out_Start_Ratio;
    if(ratio_left < 0.0f)
        return;

    float volume = 1.0f - (ratio_left / Fade_Out_Ratio_Left);

    if(volume < 0.0f)
        volume = 0.0f;

    _sound_data.sound->settings().volume = volume;
    _sound_data.sound->apply_settings();
}



void Player::process()
{
    LDS::List<Sound_Data>::Iterator it = m_active_sounds.begin();
    while(!it.end_reached())
    {
        Sound_Data& data = *it;

        M_process_fade_out(data);

        if(data.sound->is_playing())
        {
            ++it;
            continue;
        }

        delete data.data;
        delete data.sound;

        it = m_active_sounds.erase_and_iterate_forward(it);
    }
}
