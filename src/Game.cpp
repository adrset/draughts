#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include "Game.h"

#include <vector>
#include <cstdlib>
#include <fstream>
#include <thread>

#include <iostream>
#include <unistd.h>



Game::Game(int width, int height, std::string title, int fps, std::string room): m_width{width}, m_height(height), m_title(title), m_room(room)
{
    for(int i=0; i<4; i++)
        m_selectedFields[i] = 0;
    srand(time(NULL));
    //m_server = new Network::ListenerServer();
    m_client = new network::Client();
    m_window = new ge::Window(m_width, m_height, m_title);

    m_timer = new ge::Timer(fps);

    lastX = m_width / 2.0f;
    lastY = m_height / 2.0f;

    m_instanceShader = new ge::Shader("instance.vs", "instance.fs");
    m_shader = new ge::Shader("2dshader.vs", "2dshader.fs");
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

    for(int i =0; i<8; i++) {
        for(int j =0; j<8; j++) {
            pos.push_back(glm::vec3(offset*i,offset*j,0));
            col.push_back(glm::vec3((i+j)%2));
        }
    }

    m_board = new ge::QuadField(vertices, indices, sizeof(vertices), sizeof(indices), pos, col, offset);
    for(int i =0; i<3; i++) {
        for(int j =0; j<4; j++) {
            m_draughtsOpposite.push_back(new ge::TexturedQuad(vertices, indices, sizeof(vertices), sizeof(indices), glm::vec2(200*j + (i == 1 ? 100: 0),100*i), glm::vec3(0.13, 0.7, 0.12), offset, "red.png"));
        }
    }

    for(int i =5; i<8; i++) {
        for(int j =0; j<4; j++) {
            m_draughts.push_back(new ge::TexturedQuad(vertices, indices, sizeof(vertices), sizeof(indices), glm::vec2(200*j + (i == 5 || i == 7 ? 100: 0),100*i), glm::vec3(0.13, 0.7, 0.12), offset, "white.png"));
        }
    }


    m_texturedQuad = new ge::TexturedQuad(vertices, indices, sizeof(vertices), sizeof(indices), glm::vec2(100,0), glm::vec3(0.3, 0.7, 0.1), offset, "white.png");




}

std::ostream& operator<<(std::ostream& stream, glm::vec2& vec) {

    stream << "[" << vec.x << ", " << vec.y << "]";
    return stream;
}

std::ostream& operator<<(std::ostream& stream, glm::vec3& vec) {

    stream << "[" <<vec.x << ", " << vec.y << ", " << vec.z << "]";
    return stream;

}

void Game::start() {
    network::Data recv;

    std::stringstream str;
    while(recv.empty) {
        recv = m_client->send("GET_SID", 30000);

    }
    str<<recv.response;
    str>>m_sessionID;
    std::cout<<"Assigned ID"<<m_sessionID<<std::endl;

    std::string send = "CREATE_ROOM ";
    send += m_room;
    char* s = new char[send.length()];
    strcpy(s,send.c_str());
    recv.empty = true;
    while(recv.empty) {
        recv = m_client->send(s, 30000);
    }
    delete s;
    if(strncmp(recv.response, "NOK",3) == 0) { // room exists!
        std::cout<<"Room with given name exists!"<<std::endl<<"Trying to join it..."<<std::endl;
    }

    send = "JOIN ";
    send += m_room;
    send += " ";
    send += std::to_string(m_sessionID);
    s = new char[send.length()];
    strcpy(s,send.c_str());
    recv.empty = true;
    while(recv.empty) {
        recv = m_client->send(s, 30000);
    }
    delete s;
    if(strncmp(recv.response, "NOK",3) == 0) { // room exists!
        std::cout<<"Room is full! Connect to different one!"<<std::endl;

    } else {
        std::cout<<"Connected!"<<recv.response<<std::endl;
        m_window->setTitle("Draughts - room: " + m_room);
        m_window->showWindow();
        loop();
    }

}

void Game::loop() {


    int loop=0;
    while (!m_window->shouldClose())
    {
        processInput();
        m_timer->start();
        //Networking
        if(loop* 10 > m_fps ) {
            networkLogic();
            loop = 0;
        }

        m_window->clear();
        glm::vec2 pos = ge::InputManager::getMouseCoords().xy;
        const std::vector <glm::vec3>& positions = m_board->getPositions();
        const float& scale = m_board->getScale();
        for (unsigned int i = 0; i< positions.size(); i++) {
            if(pos.x > positions[i].x && pos.y > positions[i].y && pos.x < positions[i].x + scale && pos.y < positions[i].y + scale) {
                if(ge::InputManager::isMouseKeyDown(GLFW_MOUSE_BUTTON_1) && m_selected == 0) {
                    m_selectedFields[0] = ((int)pos.x /100) + 1;
                    m_selectedFields[1] = ((int)pos.y /100) + 1;
                    m_selected = 1;
                    m_board->setColor(glm::vec3(0.6), i);
                    m_selectedFields[4] = i;
                } else if(ge::InputManager::isMouseKeyDown(GLFW_MOUSE_BUTTON_1) && m_selected == 1) {
                    int x =  ((int)pos.x /100) + 1;
                    int y = ((int)pos.y /100) + 1;

                    if(m_selectedFields[0] != x || m_selectedFields[1] != y) {
                        m_selectedFields[2] = x;
                        m_selectedFields[3] = y;
                        m_selectedFields[5] = i;
                        m_selected = 2;
                        m_board->setColor(glm::vec3(0.6,0.4,0.1), i);
                    }
                } else if(m_selected == 2) {
                    if(ge::InputManager::isKeyPressed(GLFW_KEY_ENTER)) {
                        m_selected = 3;
                    }
                } 

            }

        }

        m_board->update(m_instanceShader); // draws quads

        for(int i =0; i<12; i++) {
            m_draughtsOpposite[i]->draw(m_shader);
            m_draughts[i]->draw(m_shader);
        }



        m_window->swapBuffers();

        glfwPollEvents();

        waitAndShoutFPS();
        loop++;
    }
    std::string strg = "DC ";
    strg += std::to_string(m_sessionID);
    m_client->send(strg.c_str());
    m_client->close();
    cleanUp();

}

void Game::networkLogic() {

    network::Data recv;
    if(m_selected == 3) {
        std::string strg = "MOV ";
        strg += std::to_string(m_sessionID);
        for(int i=0; i<4; i++) {
            strg += " ";
            strg += std::to_string(m_selectedFields[i]);
        }
        strg += m_room;

        recv = m_client->send(strg.c_str());

        if(!recv.empty) {

            //std::cout<<recv.response<<std::endl;
            m_selected = 0;
            m_board->setOldColor(m_selectedFields[4]);
            m_board->setOldColor(m_selectedFields[5]);
        }
    } else {
        std::string s = "GET_BOARD ";
        s+= m_room;
        recv = m_client->send(s.c_str());
        if(!recv.empty) {

            std::stringstream str;
            str<<recv.response;
            int x;
            if(strncmp(recv.response, "No rooms!", 3) == 0) { // 3 is enough :)
                std::cout<<"Server probalby died! No rooms!"<<std::endl<<"Press enter to exit."<<std::endl;
                getchar();
                glfwSetWindowShouldClose(m_window->getWindowID(), true);
            } else if(strncmp(recv.response, "END", 3) == 0) {
				std::cout<<"Match finished!"<<std::endl;
                getchar();
                glfwSetWindowShouldClose(m_window->getWindowID(), true);
            
            }else{
                int i=0;
                while(str>>x) {
                    m_boardData[i/8][i%8] = x ;
                    i++;
                }

                int k = 0;
                int m = 0;
                for(int i =0; i<12; i++) {
                    m_draughts[i]->setPosition(glm::vec2(-100));
                    m_draughtsOpposite[i]->setPosition(glm::vec2(-100));
                }
                for(int i =0; i<8; i++) {
                    for(int j =0; j<8; j++) {
                        if(m_boardData[i][j]-2 < 0) {// white
                            m_draughts[k++]->setPosition(glm::vec2(m_draughts[i]->getScale() * j,m_draughts[i]->getScale() * i));
                        } else if(m_boardData[i][j]-2 >0) {
                            m_draughtsOpposite[m++]->setPosition(glm::vec2(m_draughtsOpposite[i]->getScale() * j,m_draughtsOpposite[i]->getScale() * i));
                        } else {

                        }
                    }
                }
            }
        } else {
            std::cout<<"No connection!       "<<std::endl;
        }
    }

}

void Game::waitAndShoutFPS() {

    m_fps = 1.0/m_timer->end();
    //std::cout<< m_fps << std::endl;
    m_timer->wait();

}

void Game::gameLogic() {

}

void Game::processInput()
{
    if(ge::InputManager::isKeyPressed(GLFW_KEY_ESCAPE))
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
