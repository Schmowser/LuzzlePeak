#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "SpriteRenderer.h"
#include "resource_manager.h"


// GameLevel holds all tiles as part of a Breaout Level and
// hosts functionality to load/render levels from harddisk

class GameLevel
{
public:
	// Level state
	std::vector<GameObject> Blocks;

	// Constructor
	GameLevel()	{}

	// Load level from file
	void		Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);

	// Render level
	void		Draw(SpriteRenderer &renderer);

	// Check if the level is completed (all non-solid tiles are destroyed)
	GLboolean	IsCompleted();

private:
	// Initialize level from tile data
	void		init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);

};

#endif