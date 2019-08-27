#pragma once
#include "Engine.hpp"

class RoomSet
{
public:
	static std::shared_ptr<TextureShader> shader0;

	static std::shared_ptr<TextureMesh> wall;
	static std::shared_ptr<TextureMesh> wall_2;
	static std::shared_ptr<TextureMesh> wall_4;
	static std::shared_ptr<TextureMesh> floor;
	static std::shared_ptr<TextureMesh> roof;
	static std::shared_ptr<TextureMesh> light;

	static std::shared_ptr<Texture> wallTexture;
	static std::shared_ptr<Texture> floorTexture;
	static std::shared_ptr<Texture> lightTexture;
	static std::shared_ptr<Texture> roofTexture;

	static std::shared_ptr<Sound> buzzing;
};
