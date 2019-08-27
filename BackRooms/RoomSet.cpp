#include "RoomSet.h"

std::shared_ptr<TextureShader> RoomSet::shader0 = std::shared_ptr<TextureShader>(NULL);

std::shared_ptr<TextureMesh> RoomSet::wall = std::shared_ptr<TextureMesh>(NULL);
std::shared_ptr<TextureMesh> RoomSet::wall_2 = std::shared_ptr<TextureMesh>(NULL);
std::shared_ptr<TextureMesh> RoomSet::wall_4 = std::shared_ptr<TextureMesh>(NULL);
std::shared_ptr<TextureMesh> RoomSet::floor = std::shared_ptr<TextureMesh>(NULL);
std::shared_ptr<TextureMesh> RoomSet::roof = std::shared_ptr<TextureMesh>(NULL);
std::shared_ptr<TextureMesh> RoomSet::light = std::shared_ptr<TextureMesh>(NULL);

std::shared_ptr<Texture> RoomSet::wallTexture = std::shared_ptr<Texture>(NULL);
std::shared_ptr<Texture> RoomSet::floorTexture = std::shared_ptr<Texture>(NULL);
std::shared_ptr<Texture> RoomSet::lightTexture = std::shared_ptr<Texture>(NULL);
std::shared_ptr<Texture> RoomSet::roofTexture = std::shared_ptr<Texture>(NULL);

std::shared_ptr<Sound> RoomSet::buzzing = std::shared_ptr<Sound>(NULL);
