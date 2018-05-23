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

	m_instanceShader = new GameEngine::Shader("instance.vs", "instance.fs");
	m_shader = new GameEngine::Shader("2dshader.vs", "2dshader.fs");
	m_projection = glm::ortho(0.0f, (GLfloat)m_width, (GLfloat)m_height, 0.0f, -10.0f, 10.0f);

	m_shader->use();
	m_shader->setMat4("orthoMatrix", m_projection);

	m_instanceShader->use();
	m_instanceShader->setMat4("orthoMatrix", m_projection);

	// Simple quad
	float vertices[] = {
       1.0f,  1.0f, 0.0f,  1.0f,  1.0f, // top right
       1.0f, -1.0f, 0.0f,  1.0f,  0.0f,// bottom right
      -1.0f, -1.0f, 0.0f,  0.0f,  0.0f,// bottom left
      -1.0f,  1.0f, 0.0f,  0.0f,  1.0f // top left
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
	for(int i =0;i<3;i++){
		for(int j =0;j<4;j++){
			m_draughtsOpposite.push_back(new GameEngine::TexturedQuad(vertices, indices, sizeof(vertices), sizeof(indices), glm::vec2(200*j + (i == 1 ? 100: 0),100*i), glm::vec3(0.13, 0.7, 0.12), offset, "red.png"));
		}
	}

for(int i =5;i<8;i++){
		for(int j =0;j<4;j++){
			m_draughts.push_back(new GameEngine::TexturedQuad(vertices, indices, sizeof(vertices), sizeof(indices), glm::vec2(200*j + (i == 5 || i == 7 ? 100: 0),100*i), glm::vec3(0.13, 0.7, 0.12), offset, "red.png"));
		}
	}

	
	m_texturedQuad = new GameEngine::TexturedQuad(vertices, indices, sizeof(vertices), sizeof(indices), glm::vec2(100,0), glm::vec3(0.3, 0.7, 0.1), offset, "red.png");
   
	


}

std::ostream& operator<<(std::ostream& stream, glm::vec2& vec){

	stream << "[" << vec.x << ", " << vec.y << "]";
	return stream;
}

std::ostream& operator<<(std::ostream& stream, glm::vec3& vec){

	stream << "[" <<vec.x << ", " << vec.y << ", " << vec.z << "]";
	return stream;

}

void Game::start(){
	loop();
}

void Game::loop() {

	
	int loop=0;
	while (!m_window->shouldClose())
	{	
		processInput();
		m_timer->start();
	
 		m_window->clear();
		glm::vec2 pos = GameEngine::InputManager::getMouseCoords().xy;
		const std::vector <glm::vec3>& positions = m_board->getPositions();
		const float& scale = m_board->getScale();
		for (unsigned int i = 0; i< positions.size();i++){
			if(pos.x > positions[i].x && pos.y > positions[i].y && pos.x < positions[i].x + scale && pos.y < positions[i].y + scale){
				m_board->setColor(glm::vec3(0.6), i);
			}else{
				m_board->setOldColor(i);
			}
			
		}
		
 		m_board->update(m_instanceShader); // draws quads

		for(int i =0;i<12;i++){
			m_draughtsOpposite[i]->draw(m_shader);
			m_draughts[i]->draw(m_shader);
		}


		//Networking
		if(loop > m_fps ){
			networkLogic();
			loop = 0;	
		}
		m_window->swapBuffers();

		glfwPollEvents();
		
		waitAndShoutFPS();
		loop++;
	}
	m_client->close();
	cleanUp();

}

void Game::networkLogic(){

	Network::data recv;
	if(m_sessionID == 0 ){
		recv = m_client->send("GET_SID", 30000);
		if(!recv.empty){
			std::stringstream str;
			str<<recv.response;
			str>>m_sessionID;
			std::cout<<"Assigned ID"<<m_sessionID<<std::endl;
		
		}else{
			std::cout<<"No connection!       "<<std::endl;
		}
	}else if(GameEngine::InputManager::isKeyPressed(GLFW_KEY_R)){
		std::string strg = "MOV ";
		strg += std::to_string(m_sessionID);
		strg += " 1 3 2 4";

		char *cstr = new char[strg.length() + 1];
		strcpy(cstr, strg.c_str());

		recv = m_client->send(cstr);

		delete [] cstr;
		if(!recv.empty){
			
			std::cout<<recv.response<<std::endl;
		}
	}else{

	recv = m_client->send("GET_BOARD");
		if(!recv.empty){
			
			std::stringstream str;
			str<<recv.response;
			int x;
			
			int i=0;
			while(str>>x){
				m_boardData[i/8][i%8] = x ;
				i++;
			}	
			
			std::cout<<"RESPONSE"<<std::endl;
			for(int i =0;i<8;i++){
				for(int j =0;j<8;j++){
					std::cout<< m_boardData[i][j]<<"*";
				}
				std::cout<<std::endl;
			}
			
		}else{
			std::cout<<"No connection!       "<<std::endl;
		}
      }

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
	
	delete m_board;
	//delete m_texturedQuad;
	m_window->closeWindow();
}


Game::~Game()
{
	delete m_window;
	delete m_shader;
	delete m_instanceShader;
	delete m_timer;
	
	std::cout << "Closing game." << std::endl;
}
