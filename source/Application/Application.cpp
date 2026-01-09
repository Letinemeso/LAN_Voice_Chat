#include <Application/Application.h>

#include <thread>
#include <chrono>

#include <LV_Registration.h>

#include <Sound_Engine.h>
#include <Devices/Input_Device.h>
#include <Sound.h>

using namespace Shardis;


Application::Application()
{
    LV::register_types(m_object_constructor);

    LSound::Sound_Engine::instance();

    M_register_messages();
    M_register_types();
    M_on_components_initialized();
}

Application::~Application()
{

}



void Application::M_register_messages()
{

}

void Application::M_register_types()
{

}

void Application::M_on_components_initialized()
{

}



void Application::run()
{
    constexpr float Duration = 2.0f;

    LSound::Input_Device_Settings device_settings = LSound::Input_Device_Settings::voip_standard(Duration);
    // device_settings.raw_volume_multiplier = 2.0f;

    LSound::Input_Device input_device(device_settings);

    std::string pause_crutch;
    std::cin >> pause_crutch;

    std::cout << "\nstarting capture" << std::endl;

    input_device.start_capture();
    std::this_thread::sleep_for(std::chrono::seconds((unsigned int)Duration));
    input_device.stop_capture();

    LSound::Sound_Data* sound_data = input_device.extract_capture();
    L_ASSERT(sound_data);

    std::cout << "\nplaying capture" << std::endl;


    LSound::Sound sound;
    sound.set_sound_data(sound_data);

    LSound::Sound::Settings settings;
    // settings.volume = 2.0f;
    sound.set_settings(settings);
    sound.play();

    std::this_thread::sleep_for(std::chrono::seconds((unsigned int)Duration));

    LSound::Sound_Engine::instance().set_listener_volume_multiplier(50.0f);
    // settings.volume = 2.0f;
    sound.set_settings(settings);
    sound.play();

    std::this_thread::sleep_for(std::chrono::seconds((unsigned int)Duration));

    delete sound_data;
}
