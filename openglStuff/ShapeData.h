#pragma once
#include <GL\glew.h>
#include <glm.hpp>

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 color;
};

struct ShapeData
{
	ShapeData() : verts(0), numVerts(0), indices(0), numIndicies(0) {}
	Vertex* verts;
	GLuint numVerts;
	GLushort* indices;
	GLuint numIndicies;
	GLsizeiptr vertexBufferSize() const
	{
		return numVerts * sizeof(Vertex);
	}
	GLsizeiptr indexBufferSize() const
	{
		return numIndicies * sizeof(GLushort);
	}
	void Cleanup()
	{
		delete[] verts;
		delete[] indices;
		numIndicies = 0;
	}
};
