#include "ControlBehaviour.h"
#include "florp/app/Window.h"
#include "florp/app/Application.h"
#include "florp/game/Transform.h"
#include "florp/game/SceneManager.h"
#include "florp/app/Timing.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/wrap.hpp>
#include <imgui.h>

template <typename T>
T wrap(const T& value, const T& min, const T& max) {
	//(((x - x_min) % (x_max - x_min)) + (x_max - x_min)) % (x_max - x_min) + x_min;
	T range = max - min;
	return glm::mod(glm::mod(value - min, range) + range, range )+ min;
}
//Base Monkey
void ControlBehaviour::Update(entt::entity entity) {
	using namespace florp::app;
	auto& transform = CurrentRegistry().get<florp::game::Transform>(entity);
	Window::Sptr window = Application::Get()->GetWindow();

	glm::vec3 translate = glm::vec3(0.0f);

	//Making the player lose
	if (death == false && explosion == false) {
		translate.z += 3.5f;
	}
	if (death == true || explosion == true) {
		translate.z += 0;
	}

	translate *= Timing::DeltaTime * mySpeed;

	if (glm::length(translate) > 0) {
		translate = glm::mat3(transform.GetLocalTransform()) * translate;
		translate += transform.GetLocalPosition();
		transform.SetPosition(translate);
	}

	//Position Updates
	glm::vec3 positionGorilla = CurrentRegistry().get< florp::game::Transform>(entity).GetLocalPosition();
	MonkeyPosZ = positionGorilla.z;
	MonkeyPosX = positionGorilla.x;

	//Attacking the Monkey
	if (window->IsKeyDown(Key::One) && MonkeyPosX == -2) {
		//Testing the distance
		if (MonkeyPosZ >= -5) {
			MonkeyDist = true;
		}
	}
	if (window->IsKeyDown(Key::Two) && MonkeyPosX == 0) {
		//Testing the distance
		if (MonkeyPosZ >= -5) {
			MonkeyDist = true;
		}
	}
	if (window->IsKeyDown(Key::Three) && MonkeyPosX == 2) {
		//Testing the distance
		if (MonkeyPosZ >= -5) {
			MonkeyDist = true;
		}
	}

	if (MonkeyPosZ >= -1) {
		death = true;
	}

	//RNG Placement
	if (MonkeyDist == true) {
		controlCount += 1;
		rng = rand() % 3;
		auto start = std::chrono::system_clock::now();
		std::vector<int> v(100000, 42);
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> diff = end - start;
		rng = diff.count() * 100000;

		//3 Lanes Positions
		if (rng % 3 == 0) { //Left
			transform.SetPosition(glm::vec3(-2, -0.5, -30));
		}
		else if (rng % 3 == 1) { //Middle
			transform.SetPosition(glm::vec3(0, -0.5, -30));
		}
		else if (rng % 3 == 2) { //Right
			transform.SetPosition(glm::vec3(2, -0.5, -30));

		}
		MonkeyDist = false;
	}
}
//Points
void ControlBehaviour::RenderGUI(entt::entity entity){
	ImGui::Begin("Score");

	char txt[16];
	sprintf(txt, "%d", controlCount);
	ImGui::Button(txt);

	if (death || explosion == true)
	{
		//This also breaks the game, sort of what we wanted to happen when player loses anyways
		ImGui::BeginMenu("You died! Better luck next time!");
	}

	ImGui::End();
}
//Chris
void ControlFastest::Update(entt::entity entity) {
	using namespace florp::app;
	auto& transform = CurrentRegistry().get<florp::game::Transform>(entity);
	Window::Sptr window = Application::Get()->GetWindow();

	glm::vec3 translate = glm::vec3(0.0f);
	//Making the player lose
	if (death == false && explosion == false) {
		translate.z += 4.0f;
	}
	if (death == true || explosion == true) {
		translate.z += 0;
	}

	translate *= Timing::DeltaTime * mySpeed;

	if (glm::length(translate) > 0) {
		translate = glm::mat3(transform.GetLocalTransform()) * translate;
		translate += transform.GetLocalPosition();
		transform.SetPosition(translate);
	}

	//Position Updates
	glm::vec3 positionGorilla = CurrentRegistry().get< florp::game::Transform>(entity).GetLocalPosition();
	ChrisPosZ = positionGorilla.z;
	ChrisPosX = positionGorilla.x;

	//Attacking the Gorilla
	if (window->IsKeyDown(Key::One) && ChrisPosX == -2) {
		//Testing the distance
		if (ChrisPosZ >= -5) {
			ChrisDist = true;
		}
	}
	if (window->IsKeyDown(Key::Two) && ChrisPosX == 0) {
		//Testing the distance
		if (ChrisPosZ >= -5) {
			ChrisDist = true;
		}
	}
	if (window->IsKeyDown(Key::Three) && ChrisPosX == 2) {
		//Testing the distance
		if (ChrisPosZ >= -5) {
			ChrisDist = true;
		}
	}

	if (ChrisPosZ >= -1) {
		death = true;
	}

	//RNG Placement
	if (ChrisDist == true) {
		controlCount += 1;
		rng = rand() % 3;
		auto start = std::chrono::system_clock::now();
		std::vector<int> v(100000, 42);
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> diff = end - start;
		rng = diff.count() * 100000;

		//3 Lanes Positions
		if (rng % 3 == 0) { //Left
			transform.SetPosition(glm::vec3(-2, -0.5, -30));
		}
		else if (rng % 3 == 1) { //Middle
			transform.SetPosition(glm::vec3(0, -0.5, -30));
		}
		else if (rng % 3 == 2) { //Right
			transform.SetPosition(glm::vec3(2, -0.5, -30));
			
		}
		ChrisDist = false;
	}
}
//Haocean
void ControlFast::Update(entt::entity entity) {
	using namespace florp::app;
	auto& transform = CurrentRegistry().get<florp::game::Transform>(entity);
	Window::Sptr window = Application::Get()->GetWindow();

	glm::vec3 translate = glm::vec3(0.0f);
	//Making the player lose
	if (death == false && explosion == false) {
		translate.z += 3.0f;
	}
	if (death == true || explosion == true) {
		translate.z += 0;
	}

	translate *= Timing::DeltaTime * mySpeed;

	if (glm::length(translate) > 0) {
		translate = glm::mat3(transform.GetLocalTransform()) * translate;
		translate += transform.GetLocalPosition();
		transform.SetPosition(translate);
	}

	//Position Updates
	glm::vec3 positionGorilla = CurrentRegistry().get< florp::game::Transform>(entity).GetLocalPosition();
	MonkeyPosZ = positionGorilla.z;
	MonkeyPosX = positionGorilla.x;

	//Attacking the Monkey
	if (window->IsKeyDown(Key::One) && MonkeyPosX == -2) {
		//Testing the distance
		if (MonkeyPosZ >= -5) {
			MonkeyDist = true;
		}
	}
	if (window->IsKeyDown(Key::Two) && MonkeyPosX == 0) {
		//Testing the distance
		if (MonkeyPosZ >= -5) {
			MonkeyDist = true;
		}
	}
	if (window->IsKeyDown(Key::Three) && MonkeyPosX == 2) {
		//Testing the distance
		if (MonkeyPosZ >= -5) {
			MonkeyDist = true;
		}
	}

	if (MonkeyPosZ >= -1) {
		death = true;
	}

	//RNG Placement
	if (MonkeyDist == true) {
		controlCount+=1;
		rng = rand() % 3;
		auto start = std::chrono::system_clock::now();
		std::vector<int> v(100000, 42);
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> diff = end - start;
		rng = diff.count() * 100000;

		//3 Lanes Positions
		if (rng % 3 == 0) { //Left
			transform.SetPosition(glm::vec3(-2, -0.5, -30));
		}
		else if (rng % 3 == 1) { //Middle
			transform.SetPosition(glm::vec3(0, -0.5, -30));
		}
		else if (rng % 3 == 2) { //Right
			transform.SetPosition(glm::vec3(2, -0.5, -30));

		}
		MonkeyDist = false;
	}
}
//Isaiah
void ControlMedium::Update(entt::entity entity) {
	using namespace florp::app;
	auto& transform = CurrentRegistry().get<florp::game::Transform>(entity);
	Window::Sptr window = Application::Get()->GetWindow();
	AudioEngine& audioEngine = AudioEngine::GetInstance();

	glm::vec3 translate = glm::vec3(0.0f);
	//Making the player lose
	if (death == false && explosion == false) {
		translate.z += 2.5f;
	}
	if (death == true || explosion == true) {
		translate.z += 0;
	}

	translate *= Timing::DeltaTime * mySpeed;

	if (glm::length(translate) > 0) {
		translate = glm::mat3(transform.GetLocalTransform()) * translate;
		translate += transform.GetLocalPosition();
		transform.SetPosition(translate);
	}

	//Position Updates
	glm::vec3 positionGorilla = CurrentRegistry().get< florp::game::Transform>(entity).GetLocalPosition();
	MonkeyPosZ = positionGorilla.z;
	MonkeyPosX = positionGorilla.x;

	//Attacking the Monkey
	if (window->IsKeyDown(Key::One) && MonkeyPosX == -2) {
		//Testing the distance
		if (MonkeyPosZ >= -5) {
			MonkeyDist = true;
		}
	}
	if (window->IsKeyDown(Key::Two) && MonkeyPosX == 0) {
		//Testing the distance
		if (MonkeyPosZ >= -5) {
			MonkeyDist = true;
		}
	}
	if (window->IsKeyDown(Key::Three) && MonkeyPosX == 2) {
		//Testing the distance
		if (MonkeyPosZ >= -5) {
			MonkeyDist = true;
		}
	}

	if (MonkeyPosZ >= -1) {
		death = true;
	}

	//RNG Placement
	if (MonkeyDist == true) {
		controlCount += 1;
		rng = rand() % 3;
		auto start = std::chrono::system_clock::now();
		std::vector<int> v(100000, 42);
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> diff = end - start;
		rng = diff.count() * 100000;

		//3 Lanes Positions
		if (rng % 3 == 0) { //Left
			transform.SetPosition(glm::vec3(-2, -0.5, -30));
		}
		else if (rng % 3 == 1) { //Middle
			transform.SetPosition(glm::vec3(0, -0.5, -30));
		}
		else if (rng % 3 == 2) { //Right
			transform.SetPosition(glm::vec3(2, -0.5, -30));

		}
		MonkeyDist = false;
	}
}
//Xavier and Eric
void ControlSlow::Update(entt::entity entity) {
	using namespace florp::app;
	auto& transform = CurrentRegistry().get<florp::game::Transform>(entity);
	Window::Sptr window = Application::Get()->GetWindow();
	glm::vec3 translate = glm::vec3(0.0f);
	//Making the player lose
	if (death == false && explosion == false) {
		translate.z += 2.0f;
	}
	if (death == true || explosion == true) {
		translate.z += 0;
	}

	translate *= Timing::DeltaTime * mySpeed;

	if (glm::length(translate) > 0) {
		translate = glm::mat3(transform.GetLocalTransform()) * translate;
		translate += transform.GetLocalPosition();
		transform.SetPosition(translate);
	}

	//Position Updates
	glm::vec3 positionGorilla = CurrentRegistry().get< florp::game::Transform>(entity).GetLocalPosition();
	MonkeyPosZ = positionGorilla.z;
	MonkeyPosX = positionGorilla.x;

	//Attacking the Monkey
	if (window->IsKeyDown(Key::One) && MonkeyPosX == -2) {
		//Testing the distance
		if (MonkeyPosZ >= -5) {
			MonkeyDist = true;
		}
	}
	if (window->IsKeyDown(Key::Two) && MonkeyPosX == 0) {
		//Testing the distance
		if (MonkeyPosZ >= -5) {
			MonkeyDist = true;
		}
	}
	if (window->IsKeyDown(Key::Three) && MonkeyPosX == 2) {
		//Testing the distance
		if (MonkeyPosZ >= -5) {
			MonkeyDist = true;
		}
	}

	if (MonkeyPosZ >= -1) {
		death = true;
	}

	//RNG Placement
	if (MonkeyDist == true) {
		controlCount += 1;
		rng = rand() % 3;
		auto start = std::chrono::system_clock::now();
		std::vector<int> v(100000, 42);
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> diff = end - start;
		rng = diff.count() * 100000;

		//3 Lanes Positions
		if (rng % 3 == 0) { //Left
			transform.SetPosition(glm::vec3(-2, -0.5, -30));
		}
		else if (rng % 3 == 1) { //Middle
			transform.SetPosition(glm::vec3(0, -0.5, -30));
		}
		else if (rng % 3 == 2) { //Right
			transform.SetPosition(glm::vec3(2, -0.5, -30));

		}
		MonkeyDist = false;
	}
}
//Bomb
void bomb::Update(entt::entity entity) {
	using namespace florp::app;
	auto& transform = CurrentRegistry().get<florp::game::Transform>(entity);
	Window::Sptr window = Application::Get()->GetWindow();

	glm::vec3 translate = glm::vec3(0.0f);
	//Making the player lose
	if (death == false && explosion == false) {
		translate.z += 2.5f;
	}
	if (death == true || explosion == true) {
		translate.z += 0;

	}

	translate *= Timing::DeltaTime * mySpeed;

	if (glm::length(translate) > 0) {
		translate = glm::mat3(transform.GetLocalTransform()) * translate;
		translate += transform.GetLocalPosition();
		transform.SetPosition(translate);
	}

	//Position Updates
	glm::vec3 positionGorilla = CurrentRegistry().get< florp::game::Transform>(entity).GetLocalPosition();
	MonkeyPosZ = positionGorilla.z;
	MonkeyPosX = positionGorilla.x;

	//Attacking the bomb
	if (window->IsKeyDown(Key::One) && MonkeyPosX == -2) {
		//Testing the distance
		if (MonkeyPosZ >= -5) {
			explosion = true;
		}
	}
	if (window->IsKeyDown(Key::Two) && MonkeyPosX == 0) {
		//Testing the distance
		if (MonkeyPosZ >= -5) {
			explosion = true;
		}
	}
	if (window->IsKeyDown(Key::Three) && MonkeyPosX == 2) {
		//Testing the distance
		if (MonkeyPosZ >= -5) {
			explosion = true;
		}
	}

	//Bomb Mechanics
	if (window->IsKeyDown(Key::Four) && MonkeyPosX == -2) {
		if (MonkeyPosZ >= -5 && MonkeyPosZ <= -2) {
			MonkeyDist = true;
		}
	}
	if (window->IsKeyDown(Key::Five) && MonkeyPosX == 0) {
		if (MonkeyPosZ >= -5 && MonkeyPosZ <= -2) {
			MonkeyDist = true;
		}
	}
	if (window->IsKeyDown(Key::Six) && MonkeyPosX == 2) {
		if (MonkeyPosZ >= -5 && MonkeyPosZ <= -2) {
			MonkeyDist = true;
		}
	}

	//Bomb Making it close, so it teleports back
	if (MonkeyPosZ >= -1) {
		MonkeyDist = true;
	}

	//RNG Placement
	if (MonkeyDist == true) {
		rng = rand() % 3;
		auto start = std::chrono::system_clock::now();
		std::vector<int> v(100000, 42);
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> diff = end - start;
		rng = diff.count() * 100000;

		//3 Lanes Positions
		if (rng % 3 == 0) { //Left
			transform.SetPosition(glm::vec3(-2, -0.5, -30));
		}
		else if (rng % 3 == 1) { //Middle
			transform.SetPosition(glm::vec3(0, -0.5, -30));
		}
		else if (rng % 3 == 2) { //Right
			transform.SetPosition(glm::vec3(2, -0.5, -30));

		}
		MonkeyDist = false;
	}

	if (explosion == true)
	{
		AudioEngine::GetInstance().SetEventPosition("Explosion", { 0,0,0 });

		//Stop all other audio if it explodes
		AudioEngine::GetInstance().StopEvent("Monkey");
		AudioEngine::GetInstance().StopEvent("Monkey(X)");
		AudioEngine::GetInstance().StopEvent("Monkey(I)");
		AudioEngine::GetInstance().StopEvent("Monkey(H)");
		//AudioEngine::GetInstance().StopEvent("Monkey(E)");
		AudioEngine::GetInstance().StopEvent("Gorilla(C)");
		AudioEngine::GetInstance().StopEvent("Bomb");
		AudioEngine::GetInstance().StopEvent("Music");
	}
	if (death == true)
	{
		AudioEngine::GetInstance().SetEventPosition("Death", { 0,0,0 });

		//If dead, stop all the sounds
		AudioEngine::GetInstance().StopEvent("Monkey");
		AudioEngine::GetInstance().StopEvent("Monkey(X)");
		AudioEngine::GetInstance().StopEvent("Monkey(I)");
		AudioEngine::GetInstance().StopEvent("Monkey(H)");
		//AudioEngine::GetInstance().StopEvent("Monkey(E)");
		AudioEngine::GetInstance().StopEvent("Gorilla(C)");
		AudioEngine::GetInstance().StopEvent("Bomb");
		AudioEngine::GetInstance().StopEvent("Music");
	}
}
