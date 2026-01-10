#include <Voice_Package_Queue.h>

#include <L_Debug/L_Debug.h>

#include <Devices/Input_Device.h>

#include <Common_Constants.h>
#include <Package_Header.h>

using namespace LVC;


Voice_Package_Queue::Voice_Package_Queue()
{

}

Voice_Package_Queue::~Voice_Package_Queue()
{
    clear();
}



void Voice_Package_Queue::add_data(const LNet::Package& _voice_package)
{
    L_DEBUG_FUNC_NOARG([&]()
    {
        Package_Header header = _voice_package.parse_header<Package_Header>();
        L_ASSERT(header.command_type == Command_Type::Sound_Data);
    });

    float delay = (float)(m_queue.size() + 1) * Voice_Package_Duration;
    if(delay > Max_Delay)
        return;

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

    LSound::Sound_Data* sound_data = new LSound::Sound_Data;
    sound_data->set_raw_data(raw_sound_data);

    m_queue.push_back(sound_data);
}

void Voice_Package_Queue::clear()
{
    while(m_queue.size() != 0)
    {
        delete m_queue.back();
        m_queue.pop_back();
    }

    m_queue.clear();
}



LSound::Sound_Data* Voice_Package_Queue::extract_top_data()
{
    if(m_queue.size() == 0)
        return nullptr;

    LSound::Sound_Data* result = m_queue.front();
    m_queue.pop_front();
    return result;
}
