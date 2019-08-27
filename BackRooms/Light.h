#pragma once
#include "Engine.hpp"

class Light : public GameObjectTexture
{
public:
	Light(std::shared_ptr<Sound> sound);
	~Light();

	void play();

	std::shared_ptr<SoundEmitter> buzzing;
};

