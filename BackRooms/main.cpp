#include "Engine.hpp"
#include "Room.h"
#include "Light.h"

int main()
{
	UIManager UI;
	//1024 768   :   800 600
	if (!UI.create(900, 650, "The backrooms", 120, "fog")) //"fog"  "sobel_color"
		return -1;
	//UI.setClearColor(10,0,8);
	UI.setClearColor(255,255,255);

	Audio::init();

	std::shared_ptr<GameObject> stage = UI.stage;
	
	std::shared_ptr<GameObject> world1 = std::shared_ptr<GameObject>(new GameObject());
	stage->addChild(world1);
	
	std::vector<std::shared_ptr<Entity>> entities;
	std::vector<std::shared_ptr<Holdable>> items;


	std::shared_ptr<TextureShader> shader0 = TextureShader::loadShader("texture");

	std::shared_ptr<TextureMesh> wall = TextureMesh::loadFromFile("Wall");
	std::shared_ptr<TextureMesh> wall_2 = TextureMesh::loadFromFile("Wall_2");
	std::shared_ptr<TextureMesh> wall_4 = TextureMesh::loadFromFile("Wall_4");
	std::shared_ptr<TextureMesh> floor = TextureMesh::loadFromFile("Floor");
	std::shared_ptr<TextureMesh> roof = TextureMesh::loadFromFile("roof");
	std::shared_ptr<TextureMesh> light = TextureMesh::loadFromFile("light");

	std::shared_ptr<Texture> wallTexture = Texture::loadFromFile("backrooms_wall.jpg");
	std::shared_ptr<Texture> floorTexture = Texture::loadFromFile("backrooms_floor.jpg");
	std::shared_ptr<Texture> lightTexture = Texture::loadFromFile("backrooms_light.jpg");
	std::shared_ptr<Texture> roofTexture = Texture::loadFromFile("backrooms_roof.jpg");

//	std::shared_ptr<GameObjectTexture> roomObj(new GameObjectTexture());
//	roomObj->mesh = room;
//	roomObj->shader = shader0;
//	roomObj->texture = wallText;
//	world1->addChild(roomObj);

	//room stuff

	std::shared_ptr<RoomManager> roomManager = std::shared_ptr<RoomManager>(new RoomManager());
	roomManager->update();
	

	Camera camera(90, UI.aspectRatio, 0.1f, 100.0f);

	//16, 16, 100
//	std::shared_ptr<Terrain> land(new Terrain(1024, glm::vec3(15, 100, 15), 40, 1.0, 2.0, glm::vec3(1.0, 1.0, 1.0), 0.0, 2));
//	land->shader = shader0;
//	world1->addChild(land);
//	land->addNew(glm::vec2(0, 0));
//	//land->generateTerrain();

	std::shared_ptr<OctTree> tree(NULL);// new OctTree());
	//tree->build(world1, 2, 2);


	std::shared_ptr<Player> player(new Player(0.4, std::shared_ptr<Mesh>(NULL), &UI));
	player->setJump(10.0);
	player->setJumpTime(0.5);
	player->collider.radius = 0.6;
	entities.push_back(player);
	double ballSpeed = 100;
//	land->setTarget(player);


	//LIGHTING
	glm::vec4 lightAxis(0, 1, 0, 0);

	double lightAngleX = 60;
	double lightAngleZ = 0;
	double lightAngleY = 120;
	glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(lightAngleX * TO_RAD, lightAngleY * TO_RAD, lightAngleZ * TO_RAD)));
	lightAxis = rotation * lightAxis;


	bool lockToggle = true;

	do
	{
		if (UI.isKeyPressed(GLFW_KEY_2))
		{
			if (lockToggle)
			{
				if (UI.getMouseLockState())
					UI.unlockMouse();
				else
					UI.lockMouse();
			}
			lockToggle = false;
		}
		else
		{
			lockToggle = true;
		}


		player->move(tree);
		//player->interact(items);

		Audio::configureListener(glm::vec3(player->transform.x, player->transform.y, player->transform.z), player->lookRotation);

		shader0->setLight(lightAxis);

		if (UI.isKeyPressed(GLFW_KEY_LEFT_CONTROL))
			player->updateCamera(camera, -0.1);
		else
			player->updateCamera(camera, 0.9);

		//Holdable::move(items, land);
		Entity::entityBounds(entities);

		UI.display(camera);
	} while (UI.isWindowOpen());

	UI.delay(0.1);

	return 1;
}