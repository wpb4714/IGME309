#include "MyMesh.h"
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	float halfHeight = a_fHeight / 2.0f;

	//Calculate cone base
	std::vector<vector3> baseVertices;
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		float angle = glm::radians(static_cast<float>(i) / static_cast<float>(a_nSubdivisions) * 360.0f);
		float x = a_fRadius * cos(angle);
		float z = a_fRadius * sin(angle);
		baseVertices.push_back(vector3(x, -halfHeight, z));
	}
	//Calculate cone sides
	std::vector<vector3> sideVertices;
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		float angle = glm::radians(static_cast<float>(i) / static_cast<float>(a_nSubdivisions) * 360.0f);
		float x = a_fRadius * cos(angle);
		float z = a_fRadius * sin(angle);
		sideVertices.push_back(vector3(x, -halfHeight, z));
		sideVertices.push_back(vector3(0.0f, halfHeight, 0.0f));
		int nextIndex = (i + 1) % a_nSubdivisions;
		x = a_fRadius * cos(glm::radians(static_cast<float>(nextIndex) / static_cast<float>(a_nSubdivisions) * 360.0f));
		z = a_fRadius * sin(glm::radians(static_cast<float>(nextIndex) / static_cast<float>(a_nSubdivisions) * 360.0f));
		sideVertices.push_back(vector3(x, -halfHeight, z));
	}

	//Add base
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		int nextIndex = (i + 1) % a_nSubdivisions;
		AddTri(baseVertices[i], baseVertices[nextIndex], vector3(0.0f, -halfHeight, 0.0f));
	}
	//Add sides
	for (int i = 0; i < a_nSubdivisions * 3; i++)
	{
		AddVertexPosition(sideVertices[i]);
		AddVertexColor(a_v3Color);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	/*
	Currently, this displays perfectly in orthographic view but looks really wonky and warped in perspective,
	and I don't know why
	*/

	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	float halfHeight = a_fHeight / 2.0f;

	//Calculate cylinder caps
	std::vector<vector3> topVertices;
	std::vector<vector3> bottomVertices;
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		float angle = glm::radians(static_cast<float>(i) / static_cast<float>(a_nSubdivisions) * 360.0f);
		float x = a_fRadius * cos(angle);
		float z = a_fRadius * sin(angle);
		topVertices.push_back(vector3(x, halfHeight, z));
		bottomVertices.push_back(vector3(x, -halfHeight, z));
	}

	//Add caps
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		int nextIndex = (i + 1) % a_nSubdivisions;
		AddTri(topVertices[i], topVertices[nextIndex], vector3(0.0f, halfHeight, 0.0f));
		AddTri(bottomVertices[i], vector3(0.0f, -halfHeight, 0.0f), bottomVertices[nextIndex]);
	}

	//Calculate side vertices
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		int nextIndex = (i + 1) % a_nSubdivisions;
		vector3 topVertex1 = topVertices[i];
		vector3 topVertex2 = topVertices[nextIndex];
		vector3 bottomVertex1 = bottomVertices[i];
		vector3 bottomVertex2 = bottomVertices[nextIndex];

		//Add sides
		AddQuad(bottomVertex1, bottomVertex2, topVertex1, topVertex2);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
        a_fOuterRadius = 0.01f;

    if (a_fInnerRadius < 0.005f)
        a_fInnerRadius = 0.005f;

    if (a_fInnerRadius > a_fOuterRadius)
        std::swap(a_fInnerRadius, a_fOuterRadius);

    if (a_fHeight < 0.01f)
        a_fHeight = 0.01f;

    if (a_nSubdivisions < 3)
        a_nSubdivisions = 3;
    if (a_nSubdivisions > 360)
        a_nSubdivisions = 360;

    Release();
    Init();

    float halfHeight = a_fHeight / 2.0f;

    //Calculate outer and inner circles
    std::vector<vector3> outerVertices;
    std::vector<vector3> innerVertices;
	std::vector<vector3> botOuterVertices;
	std::vector<vector3> botInnerVertices;
    for (int i = 0; i < a_nSubdivisions; i++)
    {
        float angle = glm::radians(static_cast<float>(i) / static_cast<float>(a_nSubdivisions) * 360.0f);
        float xOuter = a_fOuterRadius * cos(angle);
        float zOuter = a_fOuterRadius * sin(angle);
        float xInner = a_fInnerRadius * cos(angle);
        float zInner = a_fInnerRadius * sin(angle);
        outerVertices.push_back(vector3(xOuter, halfHeight, zOuter));
        innerVertices.push_back(vector3(xInner, halfHeight, zInner));
		botOuterVertices.push_back(vector3(xOuter, -halfHeight, zOuter));
		botInnerVertices.push_back(vector3(xInner, -halfHeight, zInner));
    }

    //Add sides
    for (int i = 0; i < a_nSubdivisions; i++)
    {
        int nextIndex = (i + 1) % a_nSubdivisions;

		//Top cap
        vector3 outerVertex1 = outerVertices[i];
        vector3 outerVertex2 = outerVertices[nextIndex];
        vector3 innerVertex1 = innerVertices[i];
        vector3 innerVertex2 = innerVertices[nextIndex];

		//Bottom cap
		vector3 botOuterVertex1 = botOuterVertices[i];
		vector3 botOuterVertex2 = botOuterVertices[nextIndex];
		vector3 botInnerVertex1 = botInnerVertices[i];
		vector3 botInnerVertex2 = botInnerVertices[nextIndex];

        AddQuad(innerVertex1, innerVertex2, outerVertex1, outerVertex2); //top cap
		AddQuad(botInnerVertex2, botInnerVertex1, botOuterVertex2, botOuterVertex1); //bottom cap
		AddQuad(botOuterVertex2, botOuterVertex1, outerVertex2, outerVertex1); //outer sides
		AddQuad(botInnerVertex1, botInnerVertex2, innerVertex1, innerVertex2); //inner sides
    }

    // Adding information about color
    CompleteMesh(a_v3Color);
    CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	//Calculate torus vertices
	std::vector<vector3> torusVertices;

	for (int i = 0; i < a_nSubdivisionsA; i++)
	{
		float phi = glm::radians(static_cast<float>(i) / static_cast<float>(a_nSubdivisionsA) * 360.0f);
		for (int j = 0; j < a_nSubdivisionsB; j++)
		{
			float theta = glm::radians(static_cast<float>(j) / static_cast<float>(a_nSubdivisionsB) * 360.0f);
			float x = (a_fOuterRadius + a_fInnerRadius * cos(theta)) * cos(phi);
			float y = (a_fOuterRadius + a_fInnerRadius * cos(theta)) * sin(phi);
			float z = a_fInnerRadius * sin(theta);
			torusVertices.push_back(vector3(x, y, z));
		}
	}

	//Create quads
	for (int i = 0; i < a_nSubdivisionsA; i++)
	{
		int nextI = (i + 1) % a_nSubdivisionsA;

		for (int j = 0; j < a_nSubdivisionsB; j++)
		{
			int nextJ = (j + 1) % a_nSubdivisionsB;

			//Calculate indices for quad vertices
			int index1 = i * a_nSubdivisionsB + j;
			int index2 = i * a_nSubdivisionsB + nextJ;
			int index3 = nextI * a_nSubdivisionsB + j;
			int index4 = nextI * a_nSubdivisionsB + nextJ;

			//Add vertices
			vector3 v1 = torusVertices[index1];
			vector3 v2 = torusVertices[index2];
			vector3 v3 = torusVertices[index3];
			vector3 v4 = torusVertices[index4];

			//Create quad
			AddQuad(v1, v3, v2, v4);
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	// Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 8)
		a_nSubdivisions = 8;

	Release();
	Init();

	//Calculate sphere vertices
	std::vector<vector3> sphereVertices;
	for (int i = 0; i <= a_nSubdivisions; i++)
	{
		float phi = glm::pi<float>() * static_cast<float>(i) / static_cast<float>(a_nSubdivisions); //vertical angle
		for (int j = 0; j <= a_nSubdivisions; j++)
		{
			float theta = 2.0f * glm::pi<float>() * static_cast<float>(j) / static_cast<float>(a_nSubdivisions); //horizontal angle
			float x = a_fRadius * sin(phi) * cos(theta);
			float y = a_fRadius * cos(phi);
			float z = a_fRadius * sin(phi) * sin(theta);
			sphereVertices.push_back(vector3(x, y, z));
		}
	}

	//Create quads for the sphere
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		for (int j = 0; j < a_nSubdivisions; j++)
		{
			int currentIndex = i * (a_nSubdivisions + 1) + j;
			int nextIndex = currentIndex + a_nSubdivisions + 1;

			//Add vertices for the quad
			vector3 v1 = sphereVertices[currentIndex];
			vector3 v2 = sphereVertices[currentIndex + 1];
			vector3 v3 = sphereVertices[nextIndex];
			vector3 v4 = sphereVertices[nextIndex + 1];

			//Add quad to mesh
			AddQuad(v1, v2, v3, v4);
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}