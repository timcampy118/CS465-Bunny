#include "Triangle.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

// A function clamping the input values to the lower and higher bounds
#define CLAMP(in, low, high) ((in) < (low) ? (low) : ((in) > (high) ? (high) : in))




Triangle::Triangle(glm::vec3 &v0, glm::vec3 &v1, glm::vec3 &v2)
{
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;

	c[0] = glm::vec3(1.0f, 1.0f, 1.0f);
	c[1] = glm::vec3(1.0f, 1.0f, 1.0f);
	c[2] = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Triangle::setColor()
{
	
		float r = (float)rand() / RAND_MAX;
		float g = (float)rand() / RAND_MAX;
		float b = (float)rand() / RAND_MAX;
		glm::vec3 tmpVec(r, g, b);
		c[0] = tmpVec;
		c[1] = tmpVec;
		c[2] = tmpVec;
	
	
}


void Triangle::setColorVertex()
{
	for (int x = 0; x < 3; x++)
	{
		float r = (float)rand() / RAND_MAX;
		float g = (float)rand() / RAND_MAX;
		float b = (float)rand() / RAND_MAX;
		glm::vec3 tmpVec(r, g, b);
		c[x] = tmpVec;
	}
}

void Triangle::setColorZ()
{
		c[0] = glm::vec3(v[2].z, 0.0f, 0.0);
		c[1] = glm::vec3(v[2].z, 0.0f, 0.0);
		c[2] = glm::vec3(v[2].z, 0.0f, 0.0);
	
}

// Rendering the triangle using OpenGL
void Triangle::RenderOpenGL(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix) 
{

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(modelViewMatrix));

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(projectionMatrix));

	glBegin(GL_TRIANGLES);
		glColor3f(c[0].x, c[0].y, c[0].z);
		glVertex3f(v[0].x, v[0].y, v[0].z);

		glColor3f(c[1].x, c[1].y, c[1].z);
		glVertex3f(v[1].x, v[1].y, v[1].z);

		glColor3f(c[2].x, c[2].y, c[2].z);
		glVertex3f(v[2].x, v[2].y, v[2].z);
	glEnd();
}


bool Triangle::inTriangle(float i, float j, glm::vec4 v[3])
{
	float a1 = -(v[1].y- v[0].y);
	float b1 = (v[1].x - v[0].x);
	float c1 = v[0].x*v[1].y - v[1].x*v[0].y;



	float a2 = -(v[2].y - v[1].y);
	float b2 = (v[2].x - v[1].x);
	float c2 = v[1].x*v[2].y - v[2].x*v[1].y;

	float a3 = -(v[0].y - v[2].y);
	float b3 = (v[0].x - v[2].x);
	float c3 = v[2].x*v[0].y - v[0].x*v[2].y;

	if ((a1*i + b1*j + c1 >= 0) && (a2*i + b2*j + c2 >= 0) && (a3*i + b3*j + c3 >= 0))
		return true;
	//printf("not in");
	return false;

}

// Render the triangle on CPU

void Triangle::RenderCPU(glm::mat4 & modelViewMatrix, glm::mat4 & projectionMatrix, float(&buffer)[1000][1000][3], float(&depth)[1000][1000])
{
	glm::vec4 CpuVertex[3];


	//rv=

	glm::mat4 viewport(
		1000 / 2, 0, 0, 0,
		0, 1000 / 2, 0, 0,
		0, 0, 1, 0,
		1000/2, 1000/2, 0, 1
	);

	CpuVertex[0] = viewport * projectionMatrix * modelViewMatrix * glm::vec4(v[0].x, v[0].y, v[0].z, 1);
	CpuVertex[1] = viewport * projectionMatrix * modelViewMatrix * glm::vec4(v[1].x, v[1].y, v[1].z, 1);
	CpuVertex[2] = viewport * projectionMatrix * modelViewMatrix * glm::vec4(v[2].x, v[2].y, v[2].z, 1);

	CpuVertex[0] = CpuVertex[0] / CpuVertex[0].w;
	CpuVertex[1] = CpuVertex[1] / CpuVertex[1].w;
	CpuVertex[2] = CpuVertex[2] / CpuVertex[2].w;


	float xMax = -100000;
	float yMax = -100000;
	float xMin = 100000;
	float yMin = 100000;

	for (int i = 0; i < 3; i++)
	{
		if (CpuVertex[i].y>yMax)
			yMax = CpuVertex[i].y;
		if (CpuVertex[i].y<yMin)
			yMin = CpuVertex[i].y;

		if (CpuVertex[i].x>xMax)
			xMax = CpuVertex[i].x;
		 if (CpuVertex[i].x<xMin)
			xMin = CpuVertex[i].x;
	}
	


	for (float i = xMin; i <= xMax; i++)
	{
		for (float j = yMin; j <= yMax; j++)
		{
			if (inTriangle(i, j, CpuVertex))
			{
				//printf("in tri");
				
				float alpha = ((i - CpuVertex[1].x)*(CpuVertex[2].y - CpuVertex[1].y)*-1 + (j - CpuVertex[1].y)*(CpuVertex[2].x - CpuVertex[1].x))
					/ ((CpuVertex[0].x - CpuVertex[1].x)*(CpuVertex[2].y - CpuVertex[1].y)*-1 + (CpuVertex[0].y - CpuVertex[1].y)*(CpuVertex[2].x - CpuVertex[1].x));

				float beta = ((i - CpuVertex[2].x)*(CpuVertex[0].y - CpuVertex[2].y)*-1 + (j - CpuVertex[2].y)*(CpuVertex[0].x - CpuVertex[2].x))
					/ ((CpuVertex[1].x - CpuVertex[2].x)*(CpuVertex[0].y - CpuVertex[2].y)*-1 + (CpuVertex[1].y - CpuVertex[2].y)*(CpuVertex[0].x - CpuVertex[2].x));
				float gamma = 1 - alpha - beta;

				glm::vec3 color = glm::vec3(alpha*c[0].x + beta*c[1].x + gamma*c[2].x, alpha*c[0].y + beta*c[1].y + gamma*c[2].y, alpha*c[0].z + beta*c[1].z + gamma*c[2].z);
				
				
		

				

				float tmp = alpha*CpuVertex[0].z + beta*CpuVertex[1].z + gamma*CpuVertex[2].z;
				

				if ( tmp < depth[(int)j][(int)i])
				{
					if (j > 1000 || i > 1000)
					{

					}
					else
					{
						buffer[(int)j][(int)i][0] = color.x;
						buffer[(int)j][(int)i][1] = color.y;
						buffer[(int)j][(int)i][2] = color.z;
						depth[(int)j][(int)i] = tmp;
					}
				}

			
				
/*
				buffer[i][j][0] = 1.0f;
				buffer[i][j][1] = 0.0f;
				buffer[i][j][2] = 0.0f;
				*/
				
			}
		}
	}






	

}





