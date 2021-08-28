#pragma once

#include <stdlib.h>
#include <math.h>

#include <glm/glm.hpp>


class Triangle {
	private:
		glm::vec3 v[3];		// Triangle vertices
		glm::vec3 c[3];		// Vertex color

	public:
		// Constructor
		void RenderCPU(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix, float(&buffer)[1000][1000][3], float(&depth)[1000][1000]);

		Triangle(glm::vec3 &v0, glm::vec3 &v1, glm::vec3 &v2);

		void setColor();

		bool inTriangle(float x, float y, glm::vec4 CpuVertex[3]);

		void setColorVertex();

		void setColorZ();

		// Rendering the triangle using OpenGL
		void RenderOpenGL(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix);

	
};
