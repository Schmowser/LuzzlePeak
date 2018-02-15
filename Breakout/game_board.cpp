#include "game_board.h"
#include "game_object.h"

#include <fstream>
#include <sstream>

GameBoard::GameBoard()
	: Position(0, 0), Size(1, 1) {};

GameBoard::GameBoard(glm::vec2 pos, glm::vec2 size)
	: Position(pos), Size(size) {};

void GameBoard::Load(const GLchar *file)
{
	// Clear old data
	this->BlockMatrix.clear();

	// Load from file
	GLuint tileCode;
	GameBoard board;
	std::string line;
	std::ifstream fstream(file);

	// If file can be read
	if (fstream)
	{
		while (std::getline(fstream, line))		// Read each line from level file
		{
			std::istringstream sstream(line);
			std::vector<GLuint> row;
			while (sstream >> tileCode)			// Read each word seperated by spaces
				row.push_back(tileCode);
			this->BlockMatrix.push_back(row);
		}
	}
}

void GameBoard::Draw(SpriteRenderer &renderer)
{
	for (GLuint row = 0; row < Size.x; row++)		// For every row / col: Create a game object according to blockType
	{
		for (GLuint col = 0; col < Size.y; col++)
		{
			GLuint blockType = this->BlockMatrix[row][col];
			if (blockType > 0)
			{
				glm::vec2 pos(Position.x + 75 * col, Position.y - 75 * (Size.x - row - 1));		// Because y-Position is mirrored we consider -
				glm::vec2 size(75, 75);
				if (blockType == 1)
				{
					GameObject obj(pos, size, ResourceManager::GetTexture("blue"), glm::vec3(1.0f, 1.0f, 1.0f));
					obj.Draw(renderer);
				}
				else
				{
					GameObject obj(pos, size, ResourceManager::GetTexture("purple"), glm::vec3(1.0f, 1.0f, 1.0f));
					obj.Draw(renderer);
				}
			}
		}
	}
}

void GameBoard::Swap(GLuint rowIdx, GLuint colIdx)
{
	GLuint bufferInt = this->BlockMatrix[rowIdx][colIdx];
	this->BlockMatrix[rowIdx][colIdx] = this->BlockMatrix[rowIdx][colIdx + 1];
	this->BlockMatrix[rowIdx][colIdx + 1] = bufferInt;
}
