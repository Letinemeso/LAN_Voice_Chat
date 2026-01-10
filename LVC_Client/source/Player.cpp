#include <Player.h>

using namespace LVC;


Player::Player()
{

}

Player::~Player()
{
    delete m_sound_data;
}



void Player::M_on_sound_end()
{
    if(!m_sound.sound_data())
        return;

    if(m_sound.is_playing())
        return;

    m_sound.set_sound_data(nullptr);
    delete m_sound_data;
    m_sound_data = nullptr;
}

void Player::M_start_next_sound_if_needed()
{
    if(m_sound.sound_data())
        return;

    LSound::Sound_Data* next_sound_data = m_packages_queue->extract_top_data();
    if(!next_sound_data)
        return;

    m_sound_data = next_sound_data;
    m_sound.set_sound_data(next_sound_data);
    m_sound.play();
}



void Player::process()
{
    L_ASSERT(m_packages_queue);

    M_on_sound_end();
    M_start_next_sound_if_needed();
}
