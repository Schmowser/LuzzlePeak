/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game.h"
#include "game_board.h"
#include "resource_manager.h"
#include "SpriteRenderer.h"


// Game-related State data
SpriteRenderer  *Renderer;

// Player object
GameObject		*Player;

// GameBoard object
GameBoard		*Board;


Game::Game(GLuint width, GLuint height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
	delete Renderer;
	delete Player;
	delete Board;
}

void Game::Init()
{
	// Load shaders
	ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");

	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	// Load textures
	ResourceManager::LoadTexture("textures/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("textures/blue-pixilart.png", GL_TRUE, "blue");
	ResourceManager::LoadTexture("textures/green-pixilart.png", GL_FALSE, "green");
	ResourceManager::LoadTexture("textures/purple-pixilart.png", GL_TRUE, "purple");
	ResourceManager::LoadTexture("textures/cursor-pixilart.png", GL_TRUE, "cursor");
	// Load levels
	GameLevel one; one.Load("levels/samplelevel.txt", this->Width, this->Height);
	this->Levels.push_back(one);

	// Set render-specific controls
	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	//Renderer = new SpriteRenderer(vertexShader, fragmentShader);

	// Set player data
	glm::vec2 playerPos = glm::vec2(60, 740);
	glm::vec2 playerPosIdx = glm::vec2(0, 9);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("cursor"), playerPosIdx);

	// Set GameBoard data
	glm::vec2 boardPos = glm::vec2(60, 740);
	glm::vec2 boardSize = glm::vec2(10, 7);
	Board = new GameBoard(playerPos, boardSize);
	Board->Load("levels/samplelevel.txt");
	
}

void Game::Update(GLfloat dt)
{
	GLboolean update_necessary = GL_TRUE;
	// GRAVITY CHECK: Check whether a block has to fall down
	// This is the case if adjacent BlockMatrix entries under the block equal 0
	checkGravity(Board->BlockMatrix);

	// MATCH CHECK I: Check whether there is a 3-Match in a row
	Match rowMatch = checkRowMatch(Board->BlockMatrix);

	// MATCH CHECK II: Check whether there is a 3-Match in a col
	Match colMatch = checkColMatch(Board->BlockMatrix);

	// Destroy matched blocks
	if (std::get<0>(rowMatch) || std::get<0>(colMatch))
		destroyBlocks(Board->BlockMatrix, rowMatch, colMatch);
}

// =========================
//	ProcessInput routine checks for Input
//
//	W, A, S, D: Movement || Space: Swap blocks
// =========================


void Game::ProcessInput(GLfloat dt)
{
	if (this->State == GAME_ACTIVE)
	{
		// Move cursor left
		if (this->Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 75 && !this->KeysProcessed[GLFW_KEY_A])
			{
				Player->Position.x -= 75;
				Player->PosIdx.x--;
				this->KeysProcessed[GLFW_KEY_A] = GL_TRUE;
			}
		}

		// Move right
		if (this->Keys[GLFW_KEY_D])
		{
			if (Player->Position.x < 60 + 5*75 && !this->KeysProcessed[GLFW_KEY_D])
			{
				Player->Position.x += 75;
				Player->PosIdx.x++;
				this->KeysProcessed[GLFW_KEY_D] = GL_TRUE;
			}
		}

		// Move up
		if (this->Keys[GLFW_KEY_W])
		{
			if (Player->Position.y > 700 - 8*75 && !this->KeysProcessed[GLFW_KEY_W])
			{
				Player->Position.y -= 75;
				Player->PosIdx.y--;
				this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
			}
		}

		// Move down
		if (this->Keys[GLFW_KEY_S])
		{
			if (Player->Position.y <= 700 && !this->KeysProcessed[GLFW_KEY_S])
			{
				Player->Position.y += 75;
				Player->PosIdx.y++;
				this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
			}
		}

		// Swap blocks
		if (this->Keys[GLFW_KEY_SPACE])
		{
			if (!this->KeysProcessed[GLFW_KEY_SPACE])
			{
				Board->Swap(Player->PosIdx.y, Player->PosIdx.x);
				this->KeysProcessed[GLFW_KEY_SPACE] = GL_TRUE;
			}
		}
	}
}

void Game::Render()
{

	if (this->State == GAME_ACTIVE)
	{
		// Draw background
		Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);

		// Draw level
		//this->Levels[this->Level].Draw(*Renderer);

		// Draw GameBoard
		Board->Draw(*Renderer);
		
		// Draw player
		Player->Draw(*Renderer);
	}
}

void Game::resetLevel()
{
	//if (this->Level == 0)

	//else if (this->Level == 1)

	//else if (this->Level == 2)

	//else if (this->Level == 3)

	this->Levels[0].Load("levels/spaceinvaders.txt", this->Width, this->Height * 0.5);
}

void Game::resetPlayer()
{
	// Reset player/ball stats
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
}


// =======================
//	GRAVITY CHECK
// =======================

void Game::checkGravity(std::vector<std::vector<GLuint>> &matrix)
{
	GLuint rowSize = matrix.size();
	GLuint colSize = matrix[0].size();

	// distance( begin(temp), find_if( begin(temp), end(temp), [](auto x) { return x != 0; })); (needs <algorithm>)

	// For each column, starting from the bottom we check for zeros
	// If there are zeros, for every row we get the entry from the row above
	// Do this for every row until we reach the top row

	GLuint zeroCount = 0;

	for (GLuint col = 0; col < colSize; col++)
	{
		// Make sure zeroCount is zero for every column
		zeroCount = 0;
		for (GLuint row = rowSize - 1; row > 0;)
		{
			if (matrix[row][col] == 0)
			{
				// We count how many zeroes are in one column
				zeroCount++;
				row--;
			}
			else
			{
				// If zeroCount is non-zero, we get the entries from above. The top entries will be assigned a value of zero.
				if (zeroCount > 0)
				{
					for (GLuint k = row + zeroCount; k > 0; k--)
						matrix[k][col] = (k >= zeroCount) ? matrix[k - zeroCount][col] : 0;
					zeroCount = 0;
				}
				else
				{
					row--;
				}
			}
		}
	}
}

// ===========================
//	MATCH CHECK: Part I (Rows)
// ===========================

Match Game::checkRowMatch(std::vector<std::vector<GLuint>> &matrix)
{
	GLboolean match = GL_FALSE;			// Is there a match?
	GLuint match_row = 0;				// If so, in which row?
	GLuint match_col = 0;				// And where does it start?
	GLuint match_length = 0;			// And how long is it?

	// Create output Match object
	Match output_match = std::make_tuple(GL_FALSE, 0, 0, 0);

	// currentBlockType stores the type of match we are currently looking for
	GLuint currentBlockType = -1;

	GLuint rowSize = matrix.size();
	GLuint colSize = matrix[0].size();

	for (GLuint row = 0; row < rowSize; row++)
	{
		// New row sets currentBlockType and match_length to a default value
		currentBlockType = -1;
		match_length = 0;

		for (GLuint col = 0; col < colSize; col++)
		{
			// If current entry is non-zero, there could be a match
			if (matrix[row][col] != 0)
			{

				// If current block is of same type as the left one, increment match_length
				if (matrix[row][col] == currentBlockType)
					match_length++;
				else
					// Otherwise: Set the match parameters to initial state
				{
					match = GL_FALSE;
					match_length = 1;
					match_row = row;
					match_col = col;
					currentBlockType = matrix[row][col];
				}

				// Check whether there was a match longer than two
				if (match_length >= 3)
				{
					match = GL_TRUE;
					output_match = std::make_tuple(match, match_row, match_col, match_length);
				}

			}
			else
			{
				// If there is a zero, we set match_length back to 0
				match = GL_FALSE;
				match_length = 0;
			}
		}
	}

	return output_match;	// { MATCH ? , rowIdx, colIdx, length }
}

// ===========================
//	MATCH CHECK: Part II (Columns)
// ===========================

Match Game::checkColMatch(std::vector<std::vector<GLuint>> &matrix)
{
	GLboolean match = GL_FALSE;			// Is there a match?
	GLuint match_row = 0;				// Where does it start?
	GLuint match_col = 0;				// In which col?
	GLuint match_length = 0;			// And how long is it?

										// Create output Match object
	Match output_match = std::make_tuple(GL_FALSE, 0, 0, 0);

	// currentBlockType stores the type of match we are currently looking for
	GLuint currentBlockType = -1;

	GLuint rowSize = matrix.size();
	GLuint colSize = matrix[0].size();

	for (GLuint col = 0; col < colSize; col++)
	{
		// New row sets currentBlockType and match_length to a default value
		currentBlockType = -1;
		match_length = 0;

		for (GLuint row = 0; row < rowSize; row++)
		{
			// If current entry is non-zero, there could be a match
			if (matrix[row][col] != 0)
			{

				// If current block is of same type as the left one, increment match_length
				if (matrix[row][col] == currentBlockType)
					match_length++;
				else
					// Otherwise: Set the match parameters to initial state
				{
					match = GL_FALSE;
					match_length = 1;
					match_row = row;
					match_col = col;
					currentBlockType = matrix[row][col];
				}

				// Check whether there was a match longer than two
				if (match_length >= 3)
				{
					match = GL_TRUE;
					output_match = std::make_tuple(match, match_row, match_col, match_length);
				}

			}
			else
			{
				// If there is a zero, we set match_length back to 0
				match = GL_FALSE;
				match_length = 0;
			}
		}
	}

	return output_match;	// { MATCH ? , rowIdx, colIdx, length }
}

void Game::destroyBlocks(std::vector<std::vector<GLuint>> &matrix, Match rowMatch, Match colMatch)
{
	// Destroying rowMatch
	if (std::get<0>(rowMatch))
	{
		GLuint rowMatchRow = std::get<1>(rowMatch);
		GLuint rowMatchCol = std::get<2>(rowMatch);
		for (GLuint k = 0; k < std::get<3>(rowMatch); k++)
			matrix[rowMatchRow][rowMatchCol + k] = 0;
	}

	// Destroying colMatch
	if (std::get<0>(colMatch))
	{
		GLuint colMatchRow = std::get<1>(colMatch);
		GLuint colMatchCol = std::get<2>(colMatch);
		for (GLuint k = 0; k < std::get<3>(colMatch); k++)
			matrix[colMatchRow + k][colMatchCol] = 0;
	}
}

// Calculates which direction a vector is facing (N_orth, E_ast, S_outh, W_est)
Direction VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f,1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f,-1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};
	GLfloat max = 0.0f;
	GLuint best_match = -1;
	for (int i = 0; i < 4; i++)
	{
		GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}