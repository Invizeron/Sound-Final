#include "SceneBuildLayer.h"
#include "florp/game/SceneManager.h"
#include "florp/game/RenderableComponent.h"
#include <florp\graphics\MeshData.h>
#include <florp\graphics\MeshBuilder.h>
#include <florp\graphics\ObjLoader.h>

#include <glm/gtc/matrix_transform.hpp>
#include <florp\game\Transform.h>
#include "RotateBehaviour.h"
#include "CameraComponent.h"
#include "florp/app/Application.h"
#include <ControlBehaviour.h>
#include <ShadowLight.h>
#include "PointLightComponent.h"

// Audio Behaviours
#include "AudioMovementBehaviour.h"
#include "ListenerBehaviour.h"
#include <imgui.h>

/*
 * Helper function for creating a shadow casting light
 * @param scene The scene to create the light in
 * @param entityOut An optional pointer to receive the ENTT entity that was created (set to nullptr if you don't care)
 * @param pos The position of the light in world space
 * @param target The point for the light to look at, in world space
 * @param up A unit vector indicating what axis is considered 'up'
 * @param distance The far clipping plane of the light
 * @param fov The field of view of the light, in degrees
 * @param bufferSize The size of the buffer to create for the light (default 1024x1024)
 * @param name The name to associate with the light's buffer
 */
ShadowLight& CreateShadowCaster(florp::game::Scene* scene, entt::entity* entityOut, glm::vec3 pos, glm::vec3 target, glm::vec3 up, float distance = 10.0f, float fov = 60.0f, glm::ivec2 bufferSize = { 1024, 1024 }, const char* name = nullptr)
{
	// The depth attachment is a texture, with 32 bits for depth
	RenderBufferDesc depth = RenderBufferDesc();
	depth.ShaderReadable = true;
	depth.Attachment = RenderTargetAttachment::Depth;
	depth.Format = RenderTargetType::Depth32;

	// Our shadow buffer is depth-only
	FrameBuffer::Sptr shadowBuffer = std::make_shared<FrameBuffer>(bufferSize.x, bufferSize.y);
	shadowBuffer->AddAttachment(depth);
	shadowBuffer->Validate();
	if (name != nullptr)
		shadowBuffer->SetDebugName(name);

	// Create a new entity
	entt::entity entity = scene->CreateEntity();

	// Assign and initialize a shadow light component
	ShadowLight& light = scene->Registry().assign<ShadowLight>(entity);
	light.ShadowBuffer = shadowBuffer;
	light.Projection = glm::perspective(glm::radians(fov), (float)bufferSize.x / (float)bufferSize.y, 0.25f, distance);
	light.Attenuation = 1.0f / distance;
	light.Color = glm::vec3(1.0f);

	// Assign and initialize the transformation
	florp::game::Transform& t = scene->Registry().get<florp::game::Transform>(entity);
	t.SetPosition(pos);
	t.LookAt(target, up);

	// Send out the entity ID if we passed in a place to store it
	if (entityOut != nullptr)
		*entityOut = entity;

	return light;
}

void SceneBuilder::Initialize()
{
	florp::app::Application* app = florp::app::Application::Get();
	
	using namespace florp::game;
	using namespace florp::graphics;
	
	auto* scene = SceneManager::RegisterScene("main");
	SceneManager::SetCurrentScene("main");

	//Bunch of primates
	MeshData data = ObjLoader::LoadObj("Objects/monkey.obj", glm::vec4(1.0f));//Remake this to have proper orientation
	MeshData data_Gorilla = ObjLoader::LoadObj("Objects/Gorilla-LP1.obj", glm::vec4(1.0f));
	MeshData data_Lemonk = ObjLoader::LoadObj("Objects/lemonk.obj", glm::vec4(1.0f));
	MeshData data_LeGorilleHead=ObjLoader::LoadObj("Objects/LeGorilleHead2.obj", glm::vec4(1.0f));
	MeshData data_MarMonkey = ObjLoader::LoadObj("Objects/MarlinkMonkey1.obj", glm::vec4(1.0f));
	MeshData data_Monkey = ObjLoader::LoadObj("Objects/monkey_ACNH.obj", glm::vec4(1.0f));
	//Bomb
	MeshData data_Bomb = ObjLoader::LoadObj("Objects/bomb.obj", glm::vec4(1.0f));
	//Floor Markers
	MeshData data_Floor = ObjLoader::LoadObj("untitled.obj", glm::vec4(1.0f));
	//Floor Marker for the Bomb 
	MeshData data_FloorBomb = ObjLoader::LoadObj("Objects/FloorBombMarker2.obj", glm::vec4(1.0f));

	Shader::Sptr shader = std::make_shared<Shader>();
	shader->LoadPart(ShaderStageType::VertexShader, "shaders/lighting.vs.glsl");
	shader->LoadPart(ShaderStageType::FragmentShader, "shaders/forward.fs.glsl"); 
	shader->Link();

	// Load and set up our simple test material
	Material::Sptr mat = std::make_shared<Material>(shader); 
	mat->Set("s_Albedo", Texture2D::LoadFromFile("marble.png", false, true, true));


	// The central monkey (default blender)
	{
		entt::entity eMonkey = scene->CreateEntity();
		RenderableComponent& renderable = scene->Registry().assign<RenderableComponent>(eMonkey);
		renderable.Mesh = MeshBuilder::Bake(data);
		renderable.Material = mat;
		Transform& t = scene->Registry().get<Transform>(eMonkey);
		t.SetPosition(glm::vec3(0, -0.5, -30));
		
		// Make our move toward player and sound
		scene->AddBehaviour<DefaultMonkeyAudio>(eMonkey);
		scene->AddBehaviour<ControlBehaviour>(eMonkey, glm::vec3(1.0f));

	}

	//The Gorilla (Chris)
	{
		entt::entity Gorilla = scene->CreateEntity();
		RenderableComponent& renderable = scene->Registry().assign<RenderableComponent>(Gorilla);
		renderable.Mesh = MeshBuilder::Bake(data_Gorilla);
		renderable.Material = mat;
		Transform& t = scene->Registry().get<Transform>(Gorilla);

		//Initial Position Set
		t.SetPosition(glm::vec3(2, -0.5, -30));

		// Make our move toward player and sound
		scene->AddBehaviour<AudioChris>(Gorilla);
		scene->AddBehaviour<ControlFastest>(Gorilla, glm::vec3(1.0f));
	}

	//Lemonk (Xavier)
	{
		entt::entity Lemonk = scene->CreateEntity();
		RenderableComponent& renderable = scene->Registry().assign<RenderableComponent>(Lemonk);
		renderable.Mesh = MeshBuilder::Bake(data_Lemonk);
		renderable.Material = mat;
		Transform& t = scene->Registry().get<Transform>(Lemonk);

		//Initial Position Set
		t.SetPosition(glm::vec3(-2, -0.5, -20));

		scene->AddBehaviour<AudioXavier>(Lemonk);
		scene->AddBehaviour<ControlSlow>(Lemonk, glm::vec3(1.0f));
	}

	//The Gorilla Head(Isaiah)
	{
		entt::entity LeGorilleHead = scene->CreateEntity();
		RenderableComponent& renderable = scene->Registry().assign<RenderableComponent>(LeGorilleHead);
		renderable.Mesh = MeshBuilder::Bake(data_LeGorilleHead);
		renderable.Material = mat;
		Transform& t = scene->Registry().get<Transform>(LeGorilleHead);

		//Initial Position Set
		t.SetPosition(glm::vec3(2, -0.5, -25));

		scene->AddBehaviour<AudioIsaiah>(LeGorilleHead);
		scene->AddBehaviour<ControlMedium>(LeGorilleHead, glm::vec3(1.0f));
	}

	//MarlinkMonkey (Eric) 
	{
		entt::entity MarMonkey = scene->CreateEntity();
		RenderableComponent& renderable = scene->Registry().assign<RenderableComponent>(MarMonkey);
		renderable.Mesh = MeshBuilder::Bake(data_MarMonkey);
		renderable.Material = mat;
		Transform& t = scene->Registry().get<Transform>(MarMonkey);

		//Initial Position Set 
		t.SetPosition(glm::vec3(2, -0.5, -25));

		// Make our move toward player and sound
		//Audio stays at 0,0,0 for some reason, even though all code is correct (FMOD is correct too)
		//scene->AddBehaviour<AudioEric>(MarMonkey); //Doesn't work properly
		scene->AddBehaviour<ControlSlow>(MarMonkey, glm::vec3(1.0f));
	}

	//Monkey (Haocean) 
	{
		entt::entity Monkey = scene->CreateEntity();
		RenderableComponent& renderable = scene->Registry().assign<RenderableComponent>(Monkey);
		renderable.Mesh = MeshBuilder::Bake(data_Monkey);
		renderable.Material = mat;
		Transform& t = scene->Registry().get<Transform>(Monkey);

		//Initial Position Set  
		t.SetPosition(glm::vec3(-2, -0.5, -25));
		 
		// Make our move toward player and sound
		scene->AddBehaviour<AudioHao>(Monkey);
		scene->AddBehaviour<ControlFast>(Monkey, glm::vec3(1.0f));
	}

	//Bomb 
	{
		entt::entity daBomb = scene->CreateEntity();
		RenderableComponent& renderable = scene->Registry().assign<RenderableComponent>(daBomb);
		renderable.Mesh = MeshBuilder::Bake(data_Bomb);
		renderable.Material = mat;
		Transform& t = scene->Registry().get<Transform>(daBomb);

		//Initial Position Set 
		t.SetPosition(glm::vec3(0, -0.5, -30));

		// Make our move toward player and sound
		scene->AddBehaviour<AudioBomb>(daBomb);
		scene->AddBehaviour<bomb>(daBomb, glm::vec3(1.0f));
	}
	
	//Floors 
	{
		int zPos = -5;
		int yPos = -1.5;
		for (int i = 0; i < 2; i++) {
			entt::entity Floor = scene->CreateEntity();
			RenderableComponent& renderable = scene->Registry().assign<RenderableComponent>(Floor);
			renderable.Mesh = MeshBuilder::Bake(data_Floor);
			renderable.Material = mat;
			Transform& t = scene->Registry().get<Transform>(Floor);
			//Initial Position Set 
			t.SetPosition(glm::vec3(0, yPos, zPos));
			zPos += 4;
			yPos = -1.0;
		}
	}

	//Floor marker for bomb 
	{
		entt::entity FBM = scene->CreateEntity();
		RenderableComponent& renderable = scene->Registry().assign<RenderableComponent>(FBM);
		renderable.Mesh = MeshBuilder::Bake(data_FloorBomb);
		renderable.Material = mat;
		Transform& t = scene->Registry().get<Transform>(FBM);

		//Initial Position Set 
		t.SetPosition(glm::vec3(2, -0.5, -2));
	}

	// Creates our main camera
	{
		// The color buffer should be marked as shader readable, so that we generate a texture for it
		RenderBufferDesc mainColor = RenderBufferDesc();
		mainColor.ShaderReadable = true;
		mainColor.Attachment = RenderTargetAttachment::Color0;
		mainColor.Format = RenderTargetType::ColorRgb8;

		// The normal buffer
		RenderBufferDesc normalBuffer = RenderBufferDesc(); // NEW
		normalBuffer.ShaderReadable = true;
		normalBuffer.Attachment = RenderTargetAttachment::Color1;
		normalBuffer.Format = RenderTargetType::ColorRgb10; // Note: this format is 10 bits per component
		
		// The depth attachment does not need to be a texture (and would cause issues since the format is DepthStencil)
		RenderBufferDesc depth = RenderBufferDesc();
		depth.ShaderReadable = true;
		depth.Attachment = RenderTargetAttachment::Depth;
		depth.Format = RenderTargetType::Depth32;

		// Our main frame buffer needs a color output, and a depth output
		FrameBuffer::Sptr buffer = std::make_shared<FrameBuffer>(app->GetWindow()->GetWidth(), app->GetWindow()->GetHeight(), 4);
		buffer->AddAttachment(mainColor);
		buffer->AddAttachment(normalBuffer);
		buffer->AddAttachment(depth);
		buffer->Validate();
		buffer->SetDebugName("MainBuffer");

		// We'll create an entity, and attach a camera component to it
		entt::entity camera = scene->CreateEntity();
		CameraComponent& cam = scene->Registry().assign<CameraComponent>(camera);
		cam.BackBuffer = buffer;
		cam.FrontBuffer = buffer->Clone();
		cam.IsMainCamera = true;
		cam.Projection = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 1000.0f);

		scene->AddBehaviour<ListenerBehaviour>(camera);
	}	

	// Our floor plane
	{ 
		// Building the mesh
		MeshData data = MeshBuilder::Begin();
		MeshBuilder::AddAlignedCube(data, glm::vec3(0.0f, -1.0f, 0.0), glm::vec3(100.0f, 0.1f, 100.0f));
		Mesh::Sptr mesh = MeshBuilder::Bake(data);

		// Creating the entity and attaching the renderable
		entt::entity entity = scene->CreateEntity();
		RenderableComponent& renderable = scene->Registry().assign<RenderableComponent>(entity);
		renderable.Mesh = MeshBuilder::Bake(data);
		renderable.Material = mat;
	}
}
