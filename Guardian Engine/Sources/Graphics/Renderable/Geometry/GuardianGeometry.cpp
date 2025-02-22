#include "GuardianGeometry.h"
#include "../../Renderer/GuardianRenderer.h"

namespace guardian
{
	GuardianGeometry::GuardianGeometry() : GuardianRenderableBase<GuardianGeometry>()
	{
		
	}

	GuardianGeometry::GuardianGeometry(const GuardianGeometry& other) : GuardianRenderableBase<GuardianGeometry>(other)
	{

	}

	GuardianGeometry::~GuardianGeometry()
	{
		for (auto& applicable : this->ApplicableList)
		{
			applicable.reset();
		}
		this->ApplicableList.clear();
	}

	void GuardianGeometry::InitializeGeometry(std::shared_ptr<GuardianGraphics> graphics, const GuardianGeometryType& type)
	{
		if (!this->IsStaticApplicablesInitialized())
		{
			this->AddStaticApplicable(GuardianRasterizerState::CreateNewRasterizerState(graphics, GE_FILL_WIREFRAME, GE_CULL_NONE));
			this->AddStaticApplicable(GuardianTopology::CreateNewTopology(GE_TOPOLOGY_TRIANGLELIST));

			auto vs = GuardianVertexShader::CreateNewVertexShader(graphics, "../Guardian Engine/Shaders/GeometryVertexShader.hlsl");
			this->AddStaticApplicable(vs);
			this->AddStaticApplicable(
				GuardianPixelShader::CreateNewPixelShader(graphics, "../Guardian Engine/Shaders/GeometryPixelShader.hlsl"));

			D3D11_INPUT_ELEMENT_DESC id[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};
			this->AddStaticApplicable(GuardianInputLayout::CreateNewInputLayout(graphics, vs, id, ARRAYSIZE(id)));
		}

		std::vector<GeometryVertex> Vertices;
		std::vector<UINT> Indices;

		if (type == GE_GEOMETRY_SPHERE)
		{
			this->GenerateSphere(Vertices, Indices);
		}
		else if (type == GE_GEOMETRY_BOX)
		{
			this->GenerateBox(Vertices, Indices);
		}
		else if (type == GE_GEOMETRY_CAPSULE)
		{
			this->GenerateCapsule(Vertices, Indices);
		}

		this->AddVertexBuffer(GuardianVertexBuffer::CreateNewVertexBuffer(graphics, (void*)Vertices.data(), 
			(UINT)Vertices.size(), (UINT)sizeof(GeometryVertex)));
		this->AddIndexBuffer(GuardianIndexBuffer::CreateNewIndexBuffer(graphics, Indices));
		
		this->AddTransformConstantBuffer(GuardianTransformConstantBuffer::CreateNewTransformConstantBuffer(graphics));
	}

	void GuardianGeometry::UpdateGeometry(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
	{
		this->RenderingTransformConstantBuffer->UpdateData(GuardianTransformProperties(worldMatrix, viewMatrix, projectionMatrix));
	}

	void GuardianGeometry::Update()
	{

	}

	const bool GuardianGeometry::IsInitialized() const noexcept
	{
		return this->RenderingVertexBuffer->GetVertexBufferObject();
	}

	void GuardianGeometry::GenerateSphere(std::vector<GeometryVertex>& vertices, std::vector<UINT>& indices)
	{
		GVector3 greenColor(0.0f, 1.0f, 0.0f);
		float radius = 1.0f;
		float stackCount = 20.0f;
		float sliceCount = 20.0f;
		// 顶部顶点
		vertices.push_back({ GVector3(0.0f, radius, 0.0f), greenColor });
		// 中间层顶点
		float phiStep = DirectX::XM_PI / stackCount;
		float thetaStep = 2.0f * DirectX::XM_PI / sliceCount;
		for (UINT i = 1; i < stackCount; ++i)
		{
			float phi = i * phiStep;
			for (UINT j = 0; j <= sliceCount; ++j)
			{
				float theta = j * thetaStep;
				float x = radius * sinf(phi) * cosf(theta);
				float y = radius * cosf(phi);
				float z = radius * sinf(phi) * sinf(theta);
				vertices.push_back({ GVector3(x, y, z), greenColor });
			}
		}
		// 底部顶点
		vertices.push_back({ GVector3(0.0f, -radius, 0.0f), greenColor });
		// 顶部扇区索引
		for (UINT i = 1; i <= sliceCount; ++i)
		{
			indices.push_back(0);
			indices.push_back(i + 1);
			indices.push_back(i);
		}
		// 中间层索引
		UINT baseIndex = 1;
		UINT ringVertexCount = (UINT)sliceCount + 1;
		for (UINT i = 0; i < stackCount - 2; ++i)
		{
			for (UINT j = 0; j < sliceCount; ++j)
			{
				indices.push_back(baseIndex + i * ringVertexCount + j);
				indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
			}
		}
		// 底部扇区索引
		UINT southPoleIndex = static_cast<UINT>(vertices.size()) - 1;
		baseIndex = southPoleIndex - ringVertexCount;
		for (UINT i = 0; i < sliceCount; ++i)
		{
			indices.push_back(southPoleIndex);
			indices.push_back(baseIndex + i);
			indices.push_back(baseIndex + i + 1);
		}
	}

	void GuardianGeometry::GenerateBox(std::vector<GeometryVertex>& vertices, std::vector<UINT>& indices)
	{
		GVector3 greenColor(0.0f, 1.0f, 0.0f);
		float width = 1.0f;
		float height = 1.0f;
		float depth = 1.0f;

		float w2 = width / 2.0f;
		float h2 = height / 2.0f;
		float d2 = depth / 2.0f;
		// 顶点数据
		vertices =
		{
			// 前面
			{ GVector3(-w2, -h2, -d2), greenColor },
			{ GVector3(-w2, +h2, -d2), greenColor },
			{ GVector3(+w2, +h2, -d2), greenColor },
			{ GVector3(+w2, -h2, -d2), greenColor },
			// 后面
			{ GVector3(-w2, -h2, +d2), greenColor },
			{ GVector3(-w2, +h2, +d2), greenColor },
			{ GVector3(+w2, +h2, +d2), greenColor },
			{ GVector3(+w2, -h2, +d2), greenColor }
		};
		// 索引数据，将顶点连接成三角形
		indices =
		{
			// 前面
			0, 1, 2,
			0, 2, 3,
			// 后面
			4, 6, 5,
			4, 7, 6,
			// 顶面
			1, 5, 6,
			1, 6, 2,
			// 底面
			0, 3, 7,
			0, 7, 4,
			// 左面
			4, 5, 1,
			4, 1, 0,
			// 右面
			3, 2, 6,
			3, 6, 7
		};
	}

	void GuardianGeometry::GenerateCapsule(std::vector<GeometryVertex>& vertices, std::vector<UINT>& indices)
	{
		float stacks = 8.0f;
		float slices = 16.0f;
		float radius = 1.0f;
		float halfHeight = 0.5f;

		for (UINT i = 0; i < slices; ++i)
		{
			float theta = XM_2PI * i / slices;
			GeometryVertex v;
			v.Position = GVector3(
				radius * cos(theta),
				halfHeight,
				radius * sin(theta)
			);
			v.Color = GVector3(0.0f, 1.0f, 0.0f);
			vertices.push_back(v);
		}

		// 底部圆形顶点（y = -halfHeight）
		for (UINT i = 0; i < slices; ++i)
		{
			float theta = XM_2PI * i / slices;
			GeometryVertex v;
			v.Position = GVector3(
				radius * cos(theta),
				-halfHeight,
				radius * sin(theta)
			);
			v.Color = GVector3(0.0f, 1.0f, 0.0f);
			vertices.push_back(v);
		}

		for (UINT j = 1; j < stacks; ++j) 
		{
			float phi = XM_PIDIV2 * j / stacks;
			for (UINT i = 0; i < slices; ++i)
			{
				float theta = XM_2PI * i / slices;
				GeometryVertex v;
				v.Position = GVector3(
					radius * sin(phi) * cos(theta),
					halfHeight + radius * cos(phi),
					radius * sin(phi) * sin(theta)
				);
				v.Color = GVector3(0.0f, 1.0f, 0.0f);
				vertices.push_back(v);
			}
		}

		// 生成底部半球顶点 -----------------------------------------------
		for (UINT j = 1; j < stacks; ++j)
		{
			float phi = XM_PIDIV2 + XM_PIDIV2 * j / stacks;
			for (UINT i = 0; i < slices; ++i)
			{
				float theta = XM_2PI * i / slices;
				GeometryVertex v;
				v.Position = GVector3(
					radius * sin(phi) * cos(theta),
					-halfHeight + radius * cos(phi),
					radius * sin(phi) * sin(theta)
				);
				v.Color = GVector3(0.0f, 1.0f, 0.0f);
				vertices.push_back(v);
			}
		}

		// 生成索引数据 ---------------------------------------------------

		// 圆柱体侧面索引
		for (UINT i = 0; i < (UINT)slices; ++i)
		{
			UINT top = i;
			UINT bottom = (UINT)slices + i;
			UINT nextTop = (i + 1) % (UINT)slices;
			UINT nextBottom = (UINT)slices + (i + 1) % (UINT)slices;

			// 侧面四边形分解为两个三角形
			indices.insert(indices.end(), { top, bottom, nextTop });
			indices.insert(indices.end(), { bottom, nextBottom, nextTop });
		}

		// 顶部半球索引
		UINT topStart = 2 * (UINT)slices;
		for (UINT j = 0; j < (UINT)stacks - 1; ++j)
		{
			for (UINT i = 0; i < slices; ++i)
			{
				UINT current = topStart + j * (UINT)slices + i;
				UINT next = topStart + j * (UINT)slices + (i + 1) % (UINT)slices;
				UINT below = topStart + (j + 1) * (UINT)slices + i;
				UINT belowNext = topStart + (j + 1) * (UINT)slices + (i + 1) % (UINT)slices;

				indices.insert(indices.end(), { current, below, next });
				indices.insert(indices.end(), { below, belowNext, next });
			}
		}

		// 底部半球索引（注意三角形顺序）
		UINT bottomStart = 2 * (UINT)slices + ((UINT)stacks - 1) * (UINT)slices;
		for (UINT j = 0; j < (UINT)stacks - 1; ++j)
		{
			for (UINT i = 0; i < (UINT)slices; ++i)
			{
				UINT current = bottomStart + j * (UINT)slices + i;
				UINT next = bottomStart + j * (UINT)slices + (i + 1) % (UINT)slices;
				UINT below = bottomStart + (j + 1) * (UINT)slices + i;
				UINT belowNext = bottomStart + (j + 1) * (UINT)slices + (i + 1) % (UINT)slices;

				// 反转三角形顺序以保证正面朝向
				indices.insert(indices.end(), { current, next, below });
				indices.insert(indices.end(), { next, belowNext, below });
			}
		}
	}
}
