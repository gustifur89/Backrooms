#include "Light.h"



Light::Light(std::shared_ptr<Sound> sound)
{
	buzzing = std::shared_ptr<SoundEmitter>(new SoundEmitter(sound));
}


Light::~Light()
{

}

void Light::play()
{
	buzzing->transform = this->transform;
	buzzing->play();
}
