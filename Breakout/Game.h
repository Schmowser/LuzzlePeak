#ifndef GAME_H
#define GAME_H

#include <vector>
#include <tuple>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "game_level.h"
#include "ball_object.h"

// Represents the current state of the game
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

// Represents the four possible (collision) directions
enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

// Represents a match constituing of a boolean if there is one, 
// row or col index, starting col or row index and length
typedef std::tuple<GLboolean, GLuint, GLuint, GLuint> Match;

// Initial size of the player cursor
const glm::vec2 PLAYER_SIZE(150, 75);

// Game holds all game-related state and functionality.
class Game
{
public:
	// Game state
	GameState		State;
	GLboolean		Keys[1024];
	GLuint			Width, Height;
	std::vector<GameLevel>	Levels;
	GLuint					Level;

	// KeysProcessed variable to avoid key processing for multiple frames
	GLboolean		KeysProcessed[1024];

	// Constructor / Deconstructor
	Game(GLuint width, GLuint height);
	~Game();

	// Initialize game state (load all shaders / textures / levels)
	void Init();
	
	// Game loop: Check for Input, update accordingly and render changes
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();

	// Reset functions
	void resetLevel();
	void resetPlayer();

	// Check functions
	void checkGravity(std::vector<std::vector<GLuint>> &matrix);
	Match checkRowMatch(std::vector<std::vector<GLuint>> &matrix);
	Match checkColMatch(std::vector<std::vector<GLuint>> &matrix);
	void destroyBlocks(std::vector<std::vector<GLuint>> &matrix, Match rowMatch, Match colMatch);

};

#endif