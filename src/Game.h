#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ge/Window.h>
#include <ge/Shader.h>
#include <ge/Timer.h>
#include <ge/RandomString.h>
#include <ge/QuadField.h>
#include <ge/TexturedQuad.h>
#include <ge/Button.h>
#include <ge/Graph.h>
//#include <Network/ListenerServer.h>
#include <network/Client.h>
class Game
{
    public:
        Game(int width, int height, std::string title,std::string ip, int port, std::string room = "alfa", int fps = 60);
        ~Game();
        void cleanUp();
        void start();
        void gameLogic();
        void waitAndShoutFPS();

    private:
		std::string m_colorString;
		int m_currentPlayer;
		std::string m_pass;
        void networkLogic();
        int m_selected = 0;
        int m_boardData[8][8];
        //Network::ListenerServer* m_server;
        network::Client* m_client;
        glm::mat4 m_projection;
        ge::Shader* m_instanceShader;
        ge::Shader* m_shader;
        std::vector<ge::TexturedQuad*> m_draughtsOpposite;
        std::vector<ge::TexturedQuad*> m_draughts;
        ge::TexturedQuad* m_texturedQuad;
        ge::QuadField* m_board;
        int m_fps = 60;
        int m_selectedFields[6];
        unsigned int m_sessionID = 0;
        void processInput();
        int m_width;
        int m_height;
        std::string m_room;
        std::string m_title;
		int m_color;
        ge::Window* m_window;

        ge::Timer* m_timer;
        float lastX;
        float lastY;
        void loop();
};

