#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GameEngine/Window.h>
#include <GameEngine/Shader.h>
#include <GameEngine/Timer.h>
#include <GameEngine/QuadField.h>
#include <GameEngine/TexturedQuad.h>
#include <GameEngine/Button.h>
#include <GameEngine/Graph.h>
//#include <Network/ListenerServer.h>
#include <Network/Client.h>
class Game
{
public:
	Game(int width, int height, std::string title, int fps = 60);
	~Game();
	void cleanUp();
	void start();
	void gameLogic();
	void waitAndShoutFPS();

private:
	int m_boardData[8][8];
	//Network::ListenerServer* m_server;
	Network::Client* m_client;
	glm::mat4 m_projection;
	GameEngine::Shader* m_instanceShader;
	GameEngine::Shader* m_shader;
	std::vector<GameEngine::TexturedQuad*> m_draughtsOpposite;
	std::vector<GameEngine::TexturedQuad*> m_draughts;
	GameEngine::TexturedQuad* m_texturedQuad;
	GameEngine::QuadField* m_board;
	int m_fps = 60;
	
	void processInput();
	int m_width;
	int m_height;

	std::string m_title;
 
	GameEngine::Window* m_window;

	GameEngine::Timer* m_timer;
	float lastX;
	float lastY;
	void loop();
};

