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

	m_shader = new GameEngine::Shader("2dshader.vs", "2dshader.fs");

	m_shader->use();

	// Simple quad
	float vertices[] = {
        	0.5f,  0.5f, 0.0f,  // top right
        	0.5f, -0.5f, 0.0f,  // bottom right
        	-0.5f, -0.5f, 0.0f,  // bottom left
        	-0.5f,  0.5f, 0.0f   // top left 
    		};
    	unsigned int indices[] = {  // note that we start from 0!
       		0, 1, 3,  // first Triangle
        	1, 2, 3   // second Triangle
    	};

	
	glm::mat4 projection = glm::ortho(0.0f, (GLfloat)m_width, (GLfloat)m_height, 0.0f, -1.0f, 1.0f);

	m_shader->setMat4("orthoMatrix", projection);
   
	m_quad = new GameEngine::Quad(vertices, indices, sizeof(vertices), sizeof(indices), glm::vec2(5), glm::vec3(),100.0f);
	


}

void Game::start(){
	loop();
}

void Game::loop() {
   	
	
	while (!m_window->shouldClose())
	{
		processInput();
		m_timer->start();
	
 		m_window->clear();
    
       		m_quad->draw(m_shader);
		m_client->send("hello");
      
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
