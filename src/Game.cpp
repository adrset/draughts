#include "Game.h"

#include <vector>
#include <cstdlib>
#include <fstream>

#include <iostream>
#include <unistd.h>

 // new int c++11 - uniform initialization var{ a}
Game::Game(int width, int height, std::string title, float t, float cf, int n, int size,int fps): m_temp(t), m_cFactor(cf), m_n(n), m_size(size), m_width{width}, m_height(height), m_title(title)
{
	GameEngine::Window::init();

	m_window = new GameEngine::Window(m_width, m_height, m_title);
	m_graphWindow = new GameEngine::Window(m_width, m_height, "Graph", m_window->getWindowID());
	m_window2 = new GameEngine::Window(m_width, m_height, "Interface", m_window->getWindowID());



		// NanoVG init
	m_window2->makeContextCurrent();
	/*m_vg = nvgCreateGL3( NVG_STENCIL_STROKES | NVG_DEBUG);
	if (m_vg == NULL) {
		printf("Could not init nanovg.\n");
	}

	fontNormal = nvgCreateFont(m_vg, "sans", "Roboto-Regular.ttf");
		if (fontNormal == -1) {
			printf("Could not add font italic.\n");
	}*/
	m_graphWindow->makeContextCurrent();

	m_window2->changePosition(0.5*(m_window2->getWindowPosition().vWidth + m_window2->getWindowPosition().width), 0.5*(m_window2->getWindowPosition().vHeight - m_window2->getWindowPosition().width));

	m_graphWindow->changePosition(0.5*(m_window2->getWindowPosition().vWidth - 3 * m_window2->getWindowPosition().width), 0.5*(m_window2->getWindowPosition().vHeight - m_window2->getWindowPosition().width));

	m_graph = new GameEngine::Graph();

	m_window->makeContextCurrent();

	m_timer = new GameEngine::Timer(fps);

	lastX = m_width / 2.0f;
	lastY = m_height / 2.0f;


	// build and compile our shader zprogram
	// ------------------------------------
	m_shader = new GameEngine::Shader("2dshader.vs", "2dshader.fs");
	m_graphShader = new GameEngine::Shader("graph.vs", "graph.fs");

	m_graphShader->use();
	m_graphShader->setVec2("min", glm::vec2());
    m_graphShader->setVec2("max", glm::vec2());
	// set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices[] = {
       1.0f,  1.0f, 0.0f,  // top right
       1.0f, -1.0f, 0.0f,  // bottom right
      -1.0f, -1.0f, 0.0f,  // bottom left
      -1.0,  1.0f, 0.0f   // top left
  };
  unsigned int indices[] = {  // note that we start from 0!
      0, 1, 3,  // first Triangle
      1, 2, 3   // second Triangle
  };

  srand(time(NULL));
  m_shader->use();

	m_projection = glm::ortho(0.0f, (GLfloat)m_width, (GLfloat)m_height, 0.0f, -10.0f, 10.00f);
	m_shader->setMat4("orthoMatrix", m_projection);

	std::vector<glm::vec3> pos;
	std::vector<glm::vec3> col;

  m_potts = new Potts::MainMatrix(m_size, m_temp, m_cFactor, m_n, 0 );    //MainMatrix(int newMatrixSize, float simTemperature, float couplingFactor, int maxSpin, int minSpin );
  m_data = new Potts::InfoPack(m_potts);

  int offset = m_width / m_size;

	for(int i=0;i< m_size; i++){
		for(int j=0;j< m_size; j++){
            float color = 0.3 * (float)m_potts->getSpin(i,j);
			pos.push_back(glm::vec3(offset*i, offset*j, 0));
			col.push_back(glm::vec3(color, 0.2, 0.2));
		}
	}

	m_field = new GameEngine::QuadField(vertices, indices, sizeof(vertices), sizeof(indices), pos, col, ((float)m_width) / ((float)m_size));


}

void Game::start(){
	loop();
}

void Game::loop() {
	float i=0;
	//NVGpaint bg;
	float x=120;
	/*m_buttons.push_back(GameEngine::Button(200, 20, 100, 30, m_window2->getWindowID(), "Zmniejsz T", nvgRGBA(21,31,12,255), nvgRGBA(42,121,3,255), 10, 20.0f ,[&](void) {// [=] pass all variables from outside by val
		m_potts->adjustTemperature(-0.0001f);
	}));

	m_buttons.push_back(GameEngine::Button(200, 50, 100, 30, m_window2->getWindowID(), "Zwieksz T", nvgRGBA(121,31,12,255), nvgRGBA(231,121,3,255),10,20.0f ,[&](void) {// [=] pass all variables from outside by val
		m_potts->adjustTemperature(0.0001f);
	}));
	m_buttons.push_back(GameEngine::Button(200, 80, 100, 30, m_window2->getWindowID(), "Zwieksz X", nvgRGBA(221,31,12,255), nvgRGBA(231,121,3,255),10,20.0f ,[&](void) {// [=] pass all variables from outside by val
		x+= 1;
	}));
	m_buttons.push_back(GameEngine::Button(200, 110, 100, 30, m_window2->getWindowID(), "Zmniejsz X", nvgRGBA(121,12,123,255), nvgRGBA(231,121,3,255),10,20.0f ,[&](void) {// [=] pass all variables from outside by val
		x-= 1;
	}));
	*/
	
	while (!m_window->shouldClose() && !m_window2->shouldClose() && !m_graphWindow->shouldClose())
	{

		// First window drawing
		m_timer->start();
		processInput();
		m_window->makeContextCurrent();

		m_window->clear();

  		m_shader->setMat4("orthoMatrix", m_projection);

		m_field->update(m_shader);

  		gameLogic();

		// GUI drawing
		/*m_window2->makeContextCurrent();
		m_window2->clear();

		nvgBeginFrame(m_vg, m_window2->getInfo().width, m_window2->getInfo().height, m_window2->getFramebufferPixelRatio());

		////// GUI drawing
		for(GameEngine::Button b: m_buttons){
			b.draw(m_vg);
			b.isClicked();
		}
	
		bg = nvgLinearGradient(m_vg, 0,0,0,800, nvgRGBA(255,255,255,16), nvgRGBA(0,0,0,16));

		nvgFillPaint(m_vg, bg);
		nvgRect(m_vg, 0,0, 800,800);
		nvgFill(m_vg);

		nvgFillColor(m_vg, nvgRGBA(255,255,255,255));
		nvgFontSize(m_vg, 42.0f);
		nvgFontFace(m_vg, "sans");
		nvgTextAlign(m_vg,NVG_ALIGN_LEFT);
		nvgText(m_vg, 0.f,42.0f, ("FPS: " + std::to_string(m_fps)).c_str(), NULL);
		nvgText(m_vg, 0.f,72.0f, ("T: " + std::to_string(m_potts->getTemperature())).c_str(), NULL);

		nvgEndFrame(m_vg);
		*/
		// Graph

		m_graphWindow->makeContextCurrent();
		m_graphWindow->clear();
  		m_graph->draw(m_graphShader);


		//m_window->swapBuffers();
		m_window2->swapBuffers();
		m_graphWindow->swapBuffers();
		glfwPollEvents();

		waitAndShoutFPS();


	}

	cleanUp();

}

void Game::waitAndShoutFPS(){
		m_fps = 1.0/m_timer->end();

		m_timer->wait();

}

void Game::gameLogic(){
	//POTTS INTENSIFIES!!
		for(int i=0;i<m_size * m_size;i++){
        	m_potts->MetropolisStep();
		}

		m_data->calcStateHistogram(false);
		m_data->consolePrintData();

        for(int i=0;i< m_size; i++){
            for(int j=0;j< m_size; j++){
                int color = m_potts->getSpin(i,j);
                m_field->setColor(i*m_size+j, glm::normalize(glm::vec3(m_potts->getColor(color).r,m_potts->getColor(color).g,m_potts->getColor(color).b)));
            }
        }
}

void Game::processInput()
{
	if(GameEngine::InputManager::isKeyPressed(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(m_window->getWindowID(), true);
	if(GameEngine::InputManager::isKeyPressed(GLFW_KEY_1))
		m_potts->adjustTemperature(0.01f);
	if(GameEngine::InputManager::isKeyPressed(GLFW_KEY_2))
		m_potts->adjustTemperature(-0.01f);
}


void Game::cleanUp() {
	m_window->closeWindow();
}


Game::~Game()
{
	delete m_window;
	delete m_shader;
	delete m_graphShader;
	delete m_window2;
	delete m_graphWindow;
	delete m_timer;
	delete m_graph;
	std::cout << "Closing game." << std::endl;
}
