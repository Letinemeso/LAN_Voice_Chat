#include <Recorder.h>

#include <Package_Header.h>

using namespace LVC;


Recorder::Recorder()
{
    m_stopwatch.start();
}

Recorder::~Recorder()
{

}



void Recorder::M_send_voice_package_if_needed()
{
    L_ASSERT(m_network_manager->connected());

    if(!m_input_device.is_capturing())
        return;
    if(m_stopwatch.duration() < Voice_Package_Duration)
        return;

    m_input_device.stop_capture();

    LSound::Sound_Data* sound_data = m_input_device.extract_capture();
    L_ASSERT(sound_data);

    LNet::Package package;

    Package_Header header;
    header.command_type = Command_Type::Sound_Data;

    package.append_header(header);
    package.append_data(sound_data->raw_data().data, sound_data->raw_data().size);

    m_network_manager->send_package(package);

    delete sound_data;
}

void Recorder::M_start_recording_if_needed()
{
    L_ASSERT(m_network_manager->connected());

    if(m_input_device.is_capturing())
        return;

    m_stopwatch.start();
    m_input_device.start_capture();
}



void Recorder::process()
{
    L_ASSERT(m_network_manager);

    if(!m_network_manager->connected())
        return;

    M_send_voice_package_if_needed();
    M_start_recording_if_needed();
}
