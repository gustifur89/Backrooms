#pragma once
#include "Engine.hpp"
#include "RoomSet.h"
#include "Light.h"
#include <queue>

class Room : public GameObject
{
public:
	Room();
	~Room();
	//Bounds bounds;
	int lowX, highX, lowY, highY;


//	std::vector<glm::vec3> doors;
//	std::vector<glm::vec2> lights;
//	int width, height;
//	double scale;

	static std::vector<std::vector<glm::vec4>> getMap(std::vector<glm::vec3> doors, int lowX, int highX, int lowY, int highY);
	static void addPillars(std::vector<std::vector<glm::vec4>> & map, std::vector<glm::vec3> pillars);
	static void offSetWalls(std::vector<std::vector<glm::vec4>> & map, double offSet);

	void makeWalls(std::vector<std::vector<glm::vec4>> & map, double scale);
	void addWall(glm::vec2 start, glm::vec2 end, double scale);
	void setPosition(glm::vec3 pos);

	static std::shared_ptr<Room> makeRoom(glm::vec3 pos, std::vector<glm::vec3> doors, std::vector<glm::vec2> lights, int lowX, int highX, int lowY, int highY, double scale);
};


class RoomManager
{
private:
	class Grid
	{
	private:
		std::map<std::pair<int, int>, int> grid;
	public:
		int get(int x, int y)
		{
			std::pair<int, int> point(x, y);
			if (grid.find(point) == grid.end())
			{
				return 0;
			}
			else
			{
				return grid.find(point)->second;
			}
		}

		void set(int x, int y, int value)
		{
			std::pair<int, int> point(x, y);
			if (value == 0)
			{
				grid.erase(point);
			}
			else
			{
				grid[point] = value;
			}
		}

	};

	class Node
	{
	public:
		enum SIDE
		{
			HIGH_X,
			LOW_X,
			HIGH_Y,
			LOW_Y
		};

		glm::vec3 door;
		SIDE side;
		std::shared_ptr<Room> room1;
		std::shared_ptr<Room> room2;

		bool operator==(const Node& rhs) const 
		{
			return this->door == rhs.door;
		}

	};

	std::vector<Node> nodes;
	Grid grid;

	void addGridRoom(std::shared_ptr<Room> room);
	void removeGridRoom(std::shared_ptr<Room> room);

	void startRoom();
	void makeRoomFromNode(Node node);

public:
	RoomManager();
	~RoomManager();

	void update();
	void setSeed(long seed);

};

