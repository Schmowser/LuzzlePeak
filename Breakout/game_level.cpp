#include "game_level.h"

#include <fstream>
#include <sstream>


void GameLevel::Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight)
{
	// Clear old data
	this->Blocks.clear();

	// Load from file
	GLuint tileCode;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<GLuint>> tileData;

	// If file can be read
	if (fstream)
	{
		while (std::getline(fstream, line))		// Read each line from level file
		{
			std::istringstream sstream(line);
			std::vector<GLuint> row;
			while (sstream >> tileCode)			// Read each word seperated by spaces
				row.push_back(tileCode);
			tileData.push_back(row);
		}
		if (tileData.size() > 0)
			this->init(tileData, levelWidth, levelHeight);
	}

}

void GameLevel::Draw(SpriteRenderer &renderer)
{
	//for (GameObject &tile : this->Blocks)		// For every game object "tile" in the <GameObject>-vector Bricks...
	//{
	//	if (!tile.Destroyed)
	//		tile.Draw(renderer);
	//}
}

GLboolean GameLevel::IsCompleted()
{
	//for (GameObject &tile : this->Blocks)
	//	if (!tile.IsSolid && !tile.Destroyed)
	//		return GL_FALSE;					// returns GL_FALSE if at least one tile is still to be processed or not solid
	return GL_TRUE;								// returns GL_TRUE if all tiles are either destroyed or solid
}

void GameLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
{
	// Calculate dimensions
	GLuint height = tileData.size();		// tileData is a matrix consisting of a vector of vectors. The size of tileData is height.
	GLuint width = tileData[0].size();		// The size of the first vector is width.
	GLfloat unit_width = 75, unit_height = 75;

	// Initialize level tiles based on tileData
	for (GLuint y = 0; y < height; ++y)
	{
		for (GLuint x = 0; x < width; ++x)
		{
			// Check block type from level data (2D level array)
			if (tileData[y][x] == 1)		// SOLID
			{
				glm::vec2 pos(60 + unit_width * x, 65 + unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(pos, size, ResourceManager::GetTexture("blue"), glm::vec3(1.0f, 1.0f, 1.0f));
				//obj.IsSolid = GL_TRUE;
				this->Blocks.push_back(obj);
			}
			else if (tileData[y][x] > 1)	// NON_SOLID - determin its color based on level data
			{
				//glm::vec3 color = glm::vec3(1.0f);	// original: white
				//if (tileData[y][x] == 2)
				//	color = glm::vec3(0.2f, 0.6f, 1.0f);
				//else if (tileData[y][x] == 3)
				//	color = glm::vec3(0.0f, 0.7f, 0.0f);
				//else if (tileData[y][x] == 4)
				//	color = glm::vec3(0.8f, 0.8f, 0.4f);
				//else if (tileData[y][x] == 5)
				//	color = glm::vec3(1.0f, 0.5f, 0.0f);

				glm::vec2 pos(60 + unit_width * x, 65 + unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				this->Blocks.push_back(GameObject(pos, size, ResourceManager::GetTexture("purple"), glm::vec3(1.0f, 1.0f, 1.0f)));
			}
		}
	}

}