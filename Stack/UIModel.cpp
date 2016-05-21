#include <fstream>
#include "UIModel.h"
#include "MyVertex.h"
#include "ConstVars.h"


const float UIModel::LETTERWIDTH = 0.5f;
const float UIModel::LETTERHEIGHT = 1.0f;

UIModel::UIModel() : ModelClass(), m_uiPosx(0), m_uiPosy(0), m_width(LETTERWIDTH), m_height(LETTERHEIGHT)
{
}

void UIModel::SetToPolygon(std::string filename)
{
	std::ifstream fin;


	struct VertexInfo {
		int pos;
		int tex;
		int nor;
	};

	struct Face {
		VertexInfo verts[3];
	};

	std::vector<Vector3> vertices;
	std::vector<Vector2> texcoords;
	std::vector<Vector3> normals;
	std::vector<Face> faces;

	// Initialize the counts.
	int vertexCount = 0;
	int textureCount = 0;
	int normalCount = 0;
	int faceCount = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return;
	}
	while (!fin.eof())
	{
		char input;
		fin.get(input);
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ')
			{
				float x;
				float y;
				float z;
				fin >> x >> y >> z;

				vertices.push_back(Vector3{ x,y,z });
				vertexCount++;
			}
			else if (input == 't')
			{
				float x;
				float y;
				fin >> x >> y;
				texcoords.push_back(Vector2{ x,y });
				textureCount++;
			}
			else if (input == 'n')
			{
				float x;
				float y;
				float z;
				fin >> x >> y >> z;

				normals.push_back(Vector3{ x,y,z });
				normalCount++;
			}
		}
		else if (input == 'f')
		{
			fin.get(input);


			if (input == ' ')
			{
				if (textureCount == 0)
				{
					char input1, input2;

					VertexInfo v1;
					VertexInfo v2;
					VertexInfo v3;

					fin >> v1.pos >> input1 >> input2 >> v1.nor
						>> v2.pos >> input1 >> input2 >> v2.nor
						>> v3.pos >> input1 >> input2 >> v3.nor;

					Face f;
					f.verts[0] = v1;
					f.verts[1] = v2;
					f.verts[2] = v3;

					faces.emplace_back(f);
					faceCount++;
				}
				else
				{
					char input1, input2;

					VertexInfo v1;
					VertexInfo v2;
					VertexInfo v3;

					fin >> v1.pos >> input1 >> v1.tex >> input2 >> v1.nor
						>> v2.pos >> input1 >> v2.tex >> input2 >> v2.nor
						>> v3.pos >> input1 >> v3.tex >> input2 >> v3.nor;

					Face f;
					f.verts[0] = v1;
					f.verts[1] = v2;
					f.verts[2] = v3;

					faces.emplace_back(f);
					faceCount++;
				}
			}

		}
	}
	fin.close();

	int count = 0;
	for (auto& face : faces)
	{
		for (int i = 0; i < 3; i++)
		{

			XMFLOAT3	pos =
			{
				vertices[face.verts[i].pos - 1].x,
				vertices[face.verts[i].pos - 1].y,
				vertices[face.verts[i].pos - 1].z,
			};

			XMFLOAT4	color =
			{
				1,1,1,1
			};

			XMFLOAT3	normal =
			{
				normals[face.verts[i].nor - 1].x,
				normals[face.verts[i].nor - 1].y,
				normals[face.verts[i].nor - 1].z
			};
			XMFLOAT2	tex;
			if (texcoords.empty())
			{
				tex =
				{
					0.0f,
					0.0f
				};
			}
			else
			{
				tex =
				{
					texcoords[face.verts[i].tex - 1].x,
					texcoords[face.verts[i].tex - 1].y
				};
			}

			struct MyVertex vertex =
			{
				pos,
				color,
				normal,
				tex
			};

			m_vertices.push_back(vertex);

			m_indices.push_back(count * 3 + i);
		}
		count++;
	}

}


UIModel::~UIModel()
{
}


void UIModel::SetToNumber(int n)
{
	m_vertices.clear();
	m_indices.clear();

	XMFLOAT3 standardNormal = { 0.0f, 1.0f, 0.0f };
	XMFLOAT3 pos[4];
	pos[0] = { -m_width / 2 ,0.0f, +m_height / 2, };
	pos[1] = { +m_width / 2 ,0.0f, +m_height / 2, };
	pos[2] = { +m_width / 2 ,0.0f, -m_height / 2, };
	pos[3] = { -m_width / 2 ,0.0f, -m_height / 2, };
	
	XMFLOAT3 normal = { 0.0f, 1.0f, 0.0f };
	
	XMFLOAT4 rgba;
	switch (n)
	{
	case 0:
		//red
		rgba = { 1.0f, 0.0f, 0.0f, 1.0f };
		break;
	case 1:
		//orange
		rgba = { 1.0f, 165.0f/255.0f, 0.0f, 1.0f };
		break;
	case 2:
		//yellow
		rgba = { 1.0f, 1.0f, 0.0f, 1.0f };
		break;
	case 3:
		//green
		rgba = { 0.0f, 1.0f, 0.0f, 1.0f };
		break;
	case 4:
		//skyblue
		rgba = { 0.0f, 1.0f, 1.0f, 1.0f };
		break;
	case 5:
		//blue
		rgba = { 0.0f, 0.0f, 1.0f, 1.0f };
		break;
	case 6:
		//dark blue
		rgba = { 0.0f, 0.0f, 0.6f, 1.0f };
		break;
	case 7:
		//purple
		rgba = { 0.3f, 0.1f, 0.6f, 1.0f };
		break;
	case 8:
		//black
		rgba = { 0.0f, 0.0f, 0.0f, 1.0f };
		break;
	case 9:
		//white
		rgba = { 1.0f, 1.0f, 1.0f, 1.0f };
		break;
	default:
		break;
	}
	
	MyVertex v1 = { pos[0], rgba, normal,{ 0.0f, 0.0f } };
	MyVertex v2 = { pos[1], rgba, normal,{ 1.0f, 0.0f } };
	MyVertex v3 = { pos[2], rgba, normal,{ 1.0f, 1.0f } };
	MyVertex v4 = { pos[3], rgba, normal,{ 0.0f, 1.0f } };

	AddRectangle(v1, v2, v3, v4);
}


void UIModel::SetUIXY(float x, float y)
{
	m_uiPosx = x;
	m_uiPosy = y;
}

float UIModel::GetUIX()
{
	return m_uiPosx;
}

float UIModel::GetUIY()
{
	return m_uiPosy;
}

void UIModel::SetUIPosition(Camera & camera)
{
	auto cameraPos = camera.GetPosition();
	auto normalviewVec = camera.GetNormalNiewDIr();
	float nearPlaneDist = 1.0f;

	XMFLOAT3 centerPos = {
		cameraPos.x + normalviewVec.x / nearPlaneDist,
		cameraPos.y + normalviewVec.y / nearPlaneDist,
		cameraPos.z + normalviewVec.z / nearPlaneDist, };

	float deltaX = (m_uiPosx * camera.GetViewSizeWidth() - camera.GetViewSizeWidth() / 2.0f);
	float deltaY = (m_uiPosy * camera.GetViewSizeHeight() - camera.GetViewSizeHeight() / 2.0f);

	XMVECTOR v = { centerPos.x, centerPos.y, centerPos.z};

	//xz평면으로 돌리는 행렬
	XMMATRIX mat = XMMatrixRotationZ(-m_zRot);
	mat *= XMMatrixRotationY(-m_yRot);
	mat *= XMMatrixRotationX(-m_xRot);

	//XZ평면에서 움직임
	mat *= XMMatrixTranslation(deltaX, 0.0f, deltaY);

	//다시 원래 각도로 돌리는 행렬
	mat *= XMMatrixRotationX(m_xRot);
	mat *= XMMatrixRotationY(m_yRot);
	mat *= XMMatrixRotationZ(m_zRot);

	//위치이동
	XMFLOAT3 resultPos;
	auto resultVec = XMVector3Transform(v, mat);
	XMStoreFloat3(&resultPos, resultVec);

	SetPosition(resultPos);
}

