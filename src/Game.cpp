#include "Game.h"

#include <vector>
#include <cstdlib>
#include <fstream>
#include <thread>

#include <iostream>
#include <unistd.h>



Game::Game(int width, int height, std::string title, int fps): m_width{width}, m_height(height), m_title(title)
{

	//m_server = new Network::ListenerServer();
	m_client = new Network::Client();
	m_window = new GameEngine::Window(m_width, m_height, m_title);
	
	m_timer = new GameEngine::Timer(fps);

	lastX = m_width / 2.0f;
	lastY = m_height / 2.0f;

	m_shader = new GameEngine::Shader("instance.vs", "instance.fs");

	m_shader->use();

	// Simple quad
	float vertices[] = {
        	1.0f,  1.0f, 0.0f,  // top right
        	1.0f, -1.0f, 0.0f,  // bottom right
        	-1.0f, -1.0f, 0.0f,  // bottom left
        	-1.0f,  1.0f, 0.0f   // top left 
    		};
    	unsigned int indices[] = {  // note that we start from 0!
       		0, 1, 3,  // first Triangle
        	1, 2, 3   // second Triangle
    	};
	float offset = (float)m_width/8.0f;
	std::vector<glm::vec3> col;
	std::vector<glm::vec3> pos;
	
	for(int i =0;i<8;i++){
		for(int j =0;j<8;j++){
			 pos.push_back(glm::vec3(offset*i,offset*j,0));
			 col.push_back(glm::vec3((i+j)%2));
		}
	}

	m_board = new GameEngine::QuadField(vertices, indices, sizeof(vertices), sizeof(indices), pos, col, offset);
	
	
	m_projection = glm::ortho(0.0f, (GLfloat)m_width, (GLfloat)m_height, 0.0f, -10.0f, 10.0f);

	m_shader->setMat4("orthoMatrix", m_projection);
   
	


}

void Game::start(){
	loop();
}

void Game::loop() {

	
	
	while (!m_window->shouldClose())
	{	
		m_shader->use();
		m_shader->setMat4("orthoMatrix", m_projection);
		processInput();
		m_timer->start();
	
 		m_window->clear();
    		
 		m_board->update(m_shader); // draws quads
		
		m_client->send("?");
      
		m_window->swapBuffers();

		glfwPollEvents();
		/*Network::data d = m_server->listen();
		if(d.response != nullptr){
			
			if(strncmp(d.response, "MOVE ", 5) == 0){
				
				if(strncmp(d.response+5, "UP ", 2) == 0){
					// NO view matrix so move in reverse :(
					printf( "response: %s \n", d.response );
					m_quad->move(glm::vec2(0,-10));
				}else if(strncmp(d.response+5, "DOWN ", 4) == 0){
					printf( "response: %s \n", d.response );
					m_quad->move(glm::vec2(0,+10));
				}else if(strncmp(d.response+5, "LEFT ", 4) == 0){
					printf( "response: %s \n", d.response );
					m_quad->move(glm::vec2(-10,0));
				}else if(strncmp(d.response+5, "RIGHT ", 5) == 0){
					printf( "response: %s \n", d.response );
					m_quad->move(glm::vec2(10,0));
				}

			}

		}*/
		waitAndShoutFPS();

	}
	//m_server->close();
	m_client->close();
	cleanUp();

}

void Game::waitAndShoutFPS(){
		
m_fps = 1.0/m_timer->end();
//std::cout<< m_fps << std::endl;
		m_timer->wait();

}

void Game::gameLogic(){
	
}

void Game::processInput()
{
	if(GameEngine::InputManager::isKeyPressed(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(m_window->getWindowID(), true);
if(GameEngine::InputManager::isKeyPressed(GLFW_KEY_E))
		m_quad->move(glm::vec2(0,1));
}


void Game::cleanUp() {
	m_window->closeWindow();
}


Game::~Game()
{
	delete m_window;
	delete m_shader;
	delete m_timer;
	std::cout << "Closing game." << std::endl;
}
