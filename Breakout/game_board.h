#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "SpriteRenderer.h"
#include "resource_manager.h"

// GameBoard holds data of the current state of the set of 
// Blocks 

class GameBoard
{
public:
	// Object state
	glm::vec2	Position, Size;
	std::vector<std::vector<GLuint>> BlockMatrix;

	// Constructor
	GameBoard();
	GameBoard(glm::vec2 pos, glm::vec2 size);

	// Fill BlockMatrix from file
	void		Load(const GLchar *file);

	// Render GameBoard
	void		Draw(SpriteRenderer &renderer);

	// Swap two Blocks
	void		Swap(GLuint rowIdx, GLuint colIdx);


//private:
//	// Draw GameBoard
//	virtual void	Draw();
};

#endif