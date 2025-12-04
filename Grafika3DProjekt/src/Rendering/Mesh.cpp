#include "Mesh.h"

// Constructor
Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	EBO = 0;
	indexCount = 0;
}


// Method used by desctructor
void Mesh::ClearMesh()
{
	if (EBO != 0) {
		glDeleteBuffers(1, &EBO);
		EBO = 0;
	}
	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (VAO != 0) {
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	indexCount = 0;
}

// Destructor
Mesh::~Mesh()
{
	ClearMesh();
}


// Method used to create mesh
void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices, unsigned int floatsPerVertex = 8){
	// Store index count
	indexCount = numOfIndices;

	// Generate and bind VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate and bind VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	// Generate and bind EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	// Set vertex attribute pointers
	// Calculate stride
	GLsizei stride = floatsPerVertex * sizeof(GLfloat);
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Texture 
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Tangent (Normal mapping)
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

	// Bitangent (Normal mapping)
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(11 * sizeof(GLfloat)));
	glEnableVertexAttribArray(4);

	// Unbind VAO
	glBindVertexArray(0);

}

// Method used to render mesh
void Mesh::RenderMesh() {

	// Bind VAO
	glBindVertexArray(VAO);

	// Draw elements
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	// Unbind VAO 
	glBindVertexArray(0);

}

