#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "SpriteRenderer.h"
#include "game_object.h"

// BallObject holds the state of the Ball object inheriting
// relevant state data from GameObject. Contains some extra 
// functionality specific to Breakout's ball object that 
// were too specific within GameObject alone.

class BallObject : public GameObject		// Subclass
{
public:
	// Ball state
	GLfloat		Radius;
	GLboolean	Stuck;

	// Constructor
	BallObject();
	BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);

	// Moves the ball, keeping it constrained within the window bounds (except bottom edge); returns new position
	glm::vec2 Move(GLfloat dt, GLuint window_width);

	// Resets the ball to original state with given position and velocity
	void	Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif