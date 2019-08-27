#include "Room.h"

Room::Room()
{
}

Room::~Room()
{
}

std::vector<std::vector<glm::vec4>> Room::getMap(std::vector<glm::vec3> doors, int lowX, int highX, int lowY, int highY)
{
	std::vector<std::vector<glm::vec4>> map;

	std::vector<std::pair<glm::vec4, bool>> X;
	std::vector<std::pair<glm::vec4, bool>> Z;
	std::vector<std::pair<glm::vec4, bool>> PX;
	std::vector<std::pair<glm::vec4, bool>> PZ;

	std::vector<std::pair<glm::vec4, bool>> points;
	points.push_back(std::pair<glm::vec4,bool>(glm::vec4(lowX, lowY, 1, 1), false));
	points.push_back(std::pair<glm::vec4,bool>(glm::vec4(highX, lowY, -1, 1), false));
	points.push_back(std::pair<glm::vec4,bool>(glm::vec4(lowX, highY, 1, -1), false));
	points.push_back(std::pair<glm::vec4,bool>(glm::vec4(highX, highY, -1, -1), false));

	//add the door vertexes
	for (int i = 0; i < doors.size(); i++)
	{
		double halfSize = 0.5 * doors[i].z;
		if (doors[i].x == lowX)
		{
			//add it
			points.push_back(std::pair<glm::vec4, bool>(glm::vec4(doors[i].x, doors[i].y - halfSize, 1, 0), true));
			points.push_back(std::pair<glm::vec4, bool>(glm::vec4(doors[i].x, doors[i].y + halfSize, 1, 0), true));
		}
		if (doors[i].x == highX)
		{
			points.push_back(std::pair<glm::vec4, bool>(glm::vec4(doors[i].x, doors[i].y - halfSize, -1, 0), true));
			points.push_back(std::pair<glm::vec4, bool>(glm::vec4(doors[i].x, doors[i].y + halfSize, -1, 0), true));
		}
		if (doors[i].y == lowY)
		{
			points.push_back(std::pair<glm::vec4, bool>(glm::vec4(doors[i].x - halfSize, doors[i].y, 0, 1), true));
			points.push_back(std::pair<glm::vec4, bool>(glm::vec4(doors[i].x + halfSize, doors[i].y, 0, 1), true));
		}
		if (doors[i].y == highY)
		{
			points.push_back(std::pair<glm::vec4, bool>(glm::vec4(doors[i].x - halfSize, doors[i].y, 0, -1), true));
			points.push_back(std::pair<glm::vec4, bool>(glm::vec4(doors[i].x + halfSize, doors[i].y, 0, -1), true));
		}
	}

	for (int i = 0; i < points.size(); i++)
	{
		if (points[i].first.x == lowX)
			X.push_back(points[i]);
		if (points[i].first.x == highX)
			PX.push_back(points[i]);
		if (points[i].first.y == lowY)
			Z.push_back(points[i]);
		if (points[i].first.y == highY)
			PZ.push_back(points[i]);
	}

	//order them In CCW order
	for (int i = 0; i < X.size(); i++)
	{
		for (int j = i; j < X.size(); j++)
		{
			if (X[j].first.y < X[i].first.y)
			{
				//swap
				std::pair<glm::vec4, bool> temp = X[j];
				X[j] = X[i];
				X[i] = temp;
			}
		}
	}

	for (int i = 0; i < PZ.size(); i++)
	{
		for (int j = i; j < PZ.size(); j++)
		{
			if (PZ[j].first.x < PZ[i].first.x)
			{
				//swap
				std::pair<glm::vec4, bool> temp = PZ[j];
				PZ[j] = PZ[i];
				PZ[i] = temp;
			}
		}
	}

	for (int i = 0; i < PX.size(); i++)
	{
		for (int j = i; j < PX.size(); j++)
		{
			if (PX[j].first.y > PX[i].first.y)
			{
				//swap
				std::pair<glm::vec4, bool> temp = PX[j];
				PX[j] = PX[i];
				PX[i] = temp;
			}
		}
	}

	for (int i = 0; i < Z.size(); i++)
	{
		for (int j = i; j < Z.size(); j++)
		{
			if (Z[j].first.x > Z[i].first.x)
			{
				//swap
				std::pair<glm::vec4, bool> temp = Z[j];
				Z[j] = Z[i];
				Z[i] = temp;
			}
		}
	}

	std::queue<std::pair<glm::vec4, bool>> vertQue;

	//enqueue
	for (int i = 0; i < X.size(); i++)
	{
		vertQue.push(X[i]);
	}

	for (int i = 0; i < PZ.size(); i++)
	{
		vertQue.push(PZ[i]);
	}

	for (int i = 0; i < PX.size(); i++)
	{
		vertQue.push(PX[i]);
	}

	for (int i = 0; i < Z.size(); i++)
	{
		vertQue.push(Z[i]);
	}

	if (doors.size() == 0)
	{
		//somethign else
		map.push_back(std::vector<glm::vec4>());
		while (vertQue.size() > 0)
		{
			//pop and move
			std::pair<glm::vec4, bool> temp = vertQue.front();
			vertQue.pop();
			map[map.size() - 1].push_back(temp.first);
		}
	}
	else
	{
		//we need to start at a door and then loop around. So we will cycle the queue untile we get a door and then go one more.
		while (true)
		{
			std::pair<glm::vec4, bool> temp = vertQue.front();
			vertQue.pop();
			vertQue.push(temp);
			if (temp.second == true)
				break;
		}

		//now go around
		//start at first vert and go around CCW

		bool prevTag = 1;
		while (vertQue.size() > 0)
		{
			//pop and move
			std::pair<glm::vec4, bool> temp = vertQue.front();
			vertQue.pop();

			if (temp.second == true && prevTag == true)
			{
				//new
				map.push_back(std::vector<glm::vec4>());
			}
			map[map.size() - 1].push_back(temp.first);
			prevTag = temp.second;
		}
	}

	return map;
}

void Room::addPillars(std::vector<std::vector<glm::vec4>> & map, std::vector<glm::vec3> pillars)
{
	for (int i = 0; i < pillars.size(); i++)
	{
		double halfSize = pillars[i].z * 0.5;

		double lowX = pillars[i].x - halfSize;
		double highX = pillars[i].x + halfSize;
		double lowY = pillars[i].y - halfSize;
		double highY = pillars[i].y + halfSize;

		glm::vec4 LL(lowX, lowY,	0,0);
		glm::vec4 HL(highX, lowY,	0,0);
		glm::vec4 HH(highX, highY,	0,0);
		glm::vec4 LH(lowX, highY,	0,0);

		map.push_back(std::vector<glm::vec4>());
		map[map.size() - 1].push_back(LL);
		map[map.size() - 1].push_back(HL);
		map[map.size() - 1].push_back(HH);
		map[map.size() - 1].push_back(LH);
		map[map.size() - 1].push_back(LL);
	}
}

void Room::offSetWalls(std::vector<std::vector<glm::vec4>>& map, double offSet)
{
	for (int i = 0; i < map.size(); i++)
	{
		std::vector<glm::vec4> newList;
		newList.push_back(map[i][0]);
		for (int j = 0; j < map[i].size(); j++)
		{
			newList.push_back(map[i][j] + (float)offSet * glm::vec4(map[i][j].z, map[i][j].w, 0, 0));
		}
		newList.push_back(map[i][map[i].size() - 1]);
		map[i] = newList;
	}
}

void Room::makeWalls(std::vector<std::vector<glm::vec4>> & map, double scale)
{
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size() - 1; j++)
		{
			this->addWall(glm::vec2(map[i][j].x, map[i][j].y), glm::vec2(map[i][j + 1].x, map[i][j + 1].y), scale);
		}
	}
}

void Room::addWall(glm::vec2 start, glm::vec2 end, double scale)
{
	glm::vec2 path = end - start;
	double distance = glm::length(path);
	glm::vec2 dir = glm::normalize(path);

	float disForWhole = floor(distance);
	float disForHalf = 0.5 * floor((distance - disForWhole) / 0.5);
	float disForQuart = distance - disForWhole - disForHalf;

	glm::vec2 startHalf = start + disForWhole * dir;
	glm::vec2 startQuart = startHalf + disForHalf * dir;

	//angle is 90 from the direction of the start -> end vector
	double angle = TO_DEG * atan2(path.x , path.y) - 90;

	//add all the whole walls
	for (double i = 0.0; i < disForWhole; i += 1.0)
	{
		float t = ((i + 0.5));
		glm::vec2 pos = start + t * dir;

		std::shared_ptr<GameObjectTexture> obj(new GameObjectTexture());
		obj->transform.x = (pos.x) * scale;
		obj->transform.y = 0;
		obj->transform.z = (pos.y) * scale;;

		obj->transform.scaleX = scale;
		obj->transform.scaleZ = scale;
		obj->transform.scaleY = scale;

		obj->transform.angleY = angle;

		obj->mesh = RoomSet::wall;
		obj->shader = RoomSet::shader0;
		obj->texture = RoomSet::wallTexture;
		this->addChild(obj);
	}

	//add all the half walls
	for (double i = 0.0; i < disForHalf; i += 0.5)
	{
		float t = ((i + 0.25));
		glm::vec2 pos = startHalf + t * dir;

		std::shared_ptr<GameObjectTexture> obj(new GameObjectTexture());
		obj->transform.x = (pos.x) * scale;
		obj->transform.y = 0;
		obj->transform.z = (pos.y) * scale;;

		obj->transform.scaleX = scale;
		obj->transform.scaleZ = scale;
		obj->transform.scaleY = scale;

		obj->transform.angleY = angle;

		obj->mesh = RoomSet::wall_2;
		obj->shader = RoomSet::shader0;
		obj->texture = RoomSet::wallTexture;
		this->addChild(obj);
	}

	//add all the quarter walls
	for (double i = 0.0; i < disForQuart; i += 0.25)
	{
		float t = ((i + 0.125));
		glm::vec2 pos = startQuart + t * dir;

		std::shared_ptr<GameObjectTexture> obj(new GameObjectTexture());
		obj->transform.x = (pos.x) * scale;
		obj->transform.y = 0;
		obj->transform.z = (pos.y) * scale;;

		obj->transform.scaleX = scale;
		obj->transform.scaleZ = scale;
		obj->transform.scaleY = scale;

		obj->transform.angleY = angle;

		obj->mesh = RoomSet::wall_4;
		obj->shader = RoomSet::shader0;
		obj->texture = RoomSet::wallTexture;
		this->addChild(obj);
	}

}

void Room::setPosition(glm::vec3 pos)
{
	for (int i = 0; i < this->children.size(); i++)
	{
		children[i]->transform.x += pos.x;
		children[i]->transform.y += pos.y;
		children[i]->transform.z += pos.z;
	}
}

std::shared_ptr<Room> Room::makeRoom(glm::vec3 pos, std::vector<glm::vec3> doors, std::vector<glm::vec2> lights, int lowX, int highX, int lowY, int highY, double scale)
{
	std::shared_ptr<Room> room = std::shared_ptr<Room>(new Room());
	room->lowX = lowX;
	room->highX = highX;
	room->lowY = lowY;
	room->highY = highY;

	//lights
	for (int i = 0; i < lights.size(); i++)
	{
		std::shared_ptr<Light> obj(new Light(RoomSet::buzzing));

		int x = lights[i].x;
		int z = lights[i].y;

		obj->transform.x = (x + 0.5) * scale;
		obj->transform.y = 1.0 * scale;
		obj->transform.z = (z + 0.5) * scale;
		obj->mesh = RoomSet::light;
		obj->shader = RoomSet::shader0;
		obj->texture = RoomSet::lightTexture;
		room->addChild(obj);
		obj->buzzing->setLoop(true);
		obj->buzzing->setAttenuation(1.0);
		obj->buzzing->setMinDistance(10.0);
		obj->play();

		obj->buzzing->setAttenuation(0.5);
		obj->buzzing->setMinDistance(15.0);
		obj->buzzing->setLoop(true);
		obj->play();
	}

	//floor
	for (int x = lowX; x < highX; x++)
	{
		for (int z = lowY; z < highY; z++)
		{
			std::shared_ptr<GameObjectTexture> obj(new GameObjectTexture());
			obj->transform.x = (x + 0.5) * scale;
			obj->transform.y = 0;
			obj->transform.z = (z + 0.5) * scale;

			obj->transform.scaleX = scale;
			obj->transform.scaleZ = scale;
			obj->transform.scaleY = scale;

			obj->mesh = RoomSet::floor;
			obj->shader = RoomSet::shader0;
			obj->texture = RoomSet::floorTexture;

			room->addChild(obj);
		}
	}

	//roof
	for (int x = lowX; x < highX; x++)
	{
		for (int z = lowY; z < highY; z++)
		{
			std::shared_ptr<GameObjectTexture> obj(new GameObjectTexture());
			obj->transform.x = (x + 0.5) * scale;
			obj->transform.y = 1.0 * scale;
			obj->transform.z = (z + 0.5) * scale;

			obj->transform.scaleX = scale;
			obj->transform.scaleZ = scale;
			obj->transform.scaleY = scale;

			obj->mesh = RoomSet::roof;
			obj->shader = RoomSet::shader0;
			obj->texture = RoomSet::roofTexture;

			room->addChild(obj);
		}
	}

	//walls
	/*
	for (int x = 0; x < width; x++)
	{
		std::shared_ptr<GameObjectTexture> obj(new GameObjectTexture());
		obj->transform.x = (x + 0.5) * scale;
		obj->transform.y = 0;
		obj->transform.z = 0;

		obj->transform.scaleX = scale;
		obj->transform.scaleZ = scale;
		obj->transform.scaleY = scale;

		obj->transform.angleY = 180;

		obj->mesh = RoomSet::wall;
		obj->shader = RoomSet::shader0;
		obj->texture = RoomSet::wallTexture;
		room->addChild(obj);
	}
	for (int x = 0; x < width; x++)
	{
		std::shared_ptr<GameObjectTexture> obj(new GameObjectTexture());
		obj->transform.x = (x + 0.5) * scale;
		obj->transform.y = 0;
		obj->transform.z = (depth)* scale;

		obj->transform.scaleX = scale;
		obj->transform.scaleZ = scale;
		obj->transform.scaleY = scale;

		obj->transform.angleY = 0;

		obj->mesh = RoomSet::wall;
		obj->shader = RoomSet::shader0;
		obj->texture = RoomSet::wallTexture;
		room->addChild(obj);
	}
	for (int z = 0; z < depth; z++)
	{
		std::shared_ptr<GameObjectTexture> obj(new GameObjectTexture());
		obj->transform.x = 0;
		obj->transform.y = 0;
		obj->transform.z = (z + 0.5) * scale;

		obj->transform.scaleX = scale;
		obj->transform.scaleZ = scale;
		obj->transform.scaleY = scale;

		obj->transform.angleY = -90;

		obj->mesh = RoomSet::wall;
		obj->shader = RoomSet::shader0;
		obj->texture = RoomSet::wallTexture;
		room->addChild(obj);
	}
	for (int z = 0; z < depth; z++)
	{
		std::shared_ptr<GameObjectTexture> obj(new GameObjectTexture());
		obj->transform.x = (width)* scale;
		obj->transform.y = 0;
		obj->transform.z = (z + 0.5) * scale;

		obj->transform.scaleX = scale;
		obj->transform.scaleZ = scale;
		obj->transform.scaleY = scale;

		obj->transform.angleY = 90;

		obj->mesh = RoomSet::wall;
		obj->shader = RoomSet::shader0;
		obj->texture = RoomSet::wallTexture;
		room->addChild(obj);
	}
	*/

	std::vector<glm::vec3> pillars;

	pillars.push_back(glm::vec3(0,0, 0.5));

	std::vector<std::vector<glm::vec4>> map = Room::getMap(doors, lowX, highX, lowY, highY);
	Room::offSetWalls(map, 0.25);
	Room::addPillars(map, pillars);

	room->makeWalls(map, scale);

	//room->bounds = Bounds(pos, pos + glm::vec3(width, 0, depth));

	return room;
}

// =================== RoomManager ==========================

void RoomManager::addGridRoom(std::shared_ptr<Room> room)
{
	for (int x = room->lowX; x <= room->highX; x++)
	{
		for (int y = room->lowY; y <= room->highY; y++)
		{
			grid.set(x, y, 1);
		}
	}
}

void RoomManager::removeGridRoom(std::shared_ptr<Room> room)
{
	for (int x = room->lowX; x <= room->highX; x++)
	{
		for (int y = room->lowY; y <= room->highY; y++)
		{
			grid.set(x, y, 0);
		}
	}
}

void RoomManager::startRoom()
{
	//start room
	//make a regular sized room with a couple doors
}

void RoomManager::makeRoomFromNode(Node node)
{
	//node side is the side of the new room
	glm::vec2 start(node.door.x, node.door.y);
	int downX = -1;
	int upX = -1;
	int downY = -1;
	int upY = -1;

	if (node.side == Node::SIDE::LOW_X)
	{
		downX = 0;
	}
	if (node.side == Node::SIDE::HIGH_X)
	{
		upX = 0;
	}
	if (node.side == Node::SIDE::LOW_Y)
	{
		downY = 0;
	}
	if (node.side == Node::SIDE::HIGH_Y)
	{
		upY = 0;
	}






	//std::pair<int,int>



}

RoomManager::RoomManager()
{
	RoomSet::shader0 = TextureShader::loadShader("texture");

	RoomSet::wall = TextureMesh::loadFromFile("Wall");
	RoomSet::wall_2 = TextureMesh::loadFromFile("Wall_2");
	RoomSet::wall_4 = TextureMesh::loadFromFile("Wall_4");
	RoomSet::floor = TextureMesh::loadFromFile("Floor");
	RoomSet::roof = TextureMesh::loadFromFile("roof");
	RoomSet::light = TextureMesh::loadFromFile("light");

	RoomSet::wallTexture = Texture::loadFromFile("backrooms_wall.jpg");
	RoomSet::floorTexture = Texture::loadFromFile("backrooms_floor.jpg");
	RoomSet::lightTexture = Texture::loadFromFile("backrooms_light.jpg");
	RoomSet::roofTexture = Texture::loadFromFile("backrooms_roof.jpg");

	RoomSet::buzzing = Sound::loadFromFile("buzz");
}

RoomManager::~RoomManager()
{

}

void RoomManager::update()
{
	//Clear rooms out of bounds. Readd opened nodes
	//Clear nodes removed
	if (nodes.size() == 0)
	{
		startRoom();
	}
	else
	{
		//add new
		std::vector<Node> workingNodes;
		for (int i = 0; i < nodes.size(); i++)
		{
			//if it's in range
			workingNodes.push_back(nodes[i]);
		}

		if (workingNodes.size() != 0)
		{
			//now with working nodes.. pick a random one
			Node nNode = workingNodes[rand() % workingNodes.size()];

		}

	}
}

void RoomManager::setSeed(long seed)
{
	srand(seed);
}

/*
std::vector<glm::vec3> doors;
std::vector<glm::vec2> lights;
lights.push_back(glm::vec2(0, 0));
lights.push_back(glm::vec2(3, 2));
lights.push_back(glm::vec2(2, 5));

doors.push_back(glm::vec3(4, 2, 1));
doors.push_back(glm::vec3(2, 6, 1));

std::shared_ptr<Room> room0 = Room::makeRoom(doors, lights, 4, 6, 2.5);

world1->addChild(room0);

std::vector<glm::vec3> doors2;
doors2.push_back(glm::vec3(0, 2, 1));
//doors2.push_back(glm::vec3(2, 0, 1));

std::shared_ptr<Room> room1 = Room::makeRoom(doors2, lights, 4, 6, 2.5);
room1->setPosition(glm::vec3(4 * 2.5, 0, 0));

world1->addChild(room1);
*/
