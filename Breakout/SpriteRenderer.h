#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"

class SpriteRenderer
{
public:
	// Constructor
	SpriteRenderer(Shader &shader);
	//SpriteRenderer(int &vertexShader, int &fragmentShader);
	// Deconstructor
	~SpriteRenderer();
	// Renders a defined quad textured with give sprite
	void DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
	void DrawTriangle();

private:
	// Render state
	Shader shader;
	//int vertexShader, fragmentShader;
	GLuint quadVAO;

	// Initialization and Configuration of quad's buffer and vertex attributes
	void initRenderData();
};

#endif