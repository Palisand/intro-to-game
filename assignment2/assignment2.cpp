//
//  pong.cpp
//  NYUCodebase
//
//  Created by Panagis Alisandratos on 9/17/14.
//

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#define PI 3.14159265

SDL_Window* displayWindow;

GLuint LoadTexture(const char* image_path, GLenum image_format = GL_BGRA) {
    
    SDL_Surface *surface = IMG_Load(image_path);
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, image_format, GL_UNSIGNED_BYTE, surface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    SDL_FreeSurface(surface);
    
    return textureID;
}

class Entity { //put in header (entity.h, entity.cpp)
    
public:
    
    Entity() : x_scale(1), y_scale(1) {}
    
    void Draw() {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(x, y, 0.0);
        glRotatef(angle, 0.0, 0.0, 1.0);
        glScalef(x_scale, y_scale, 0.0);
        
        GLfloat quad[] = {
            -width * 0.5f, height * 0.5f,
            -width * 0.5f, -height * 0.5f,
            width * 0.5f, -height * 0.5f,
            width * 0.5f, height * 0.5f
        };
        glVertexPointer(2, GL_FLOAT, 0, quad);
        glEnableClientState(GL_VERTEX_ARRAY);
        
        GLfloat quadUVs[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
        glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glDrawArrays(GL_QUADS, 0, 4);
        glDisable(GL_TEXTURE_2D);
    }
    
    void SetSize(float height, float width) {
        this->height = height;
        this->width = width;
    }
    
    float x;
    float y;
    float x_scale;
    float y_scale;
    float angle;
    
    int textureID;
    
    float width;
    float height;
    float speed;
    
    float dir_x;
    float dir_y;
    
};

//global vars of death
Entity ball, paddle_left, paddle_right;
Entity top_bumper, bottom_bumper, net;
float screen_red = 0.0f;
float screen_blue = 0.0f;
float screen_green = 0.0f;
bool screen_fade = false;
const Uint8* keys = SDL_GetKeyboardState(nullptr);

void Setup() {
    SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("This is Pong... Sorta", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
    
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}

void SetValues(bool right_won = true) {
    ball.x = 0.0f;
    ball.y = 0.0f;
    ball.dir_x = right_won ? 1.0f : -1.0f;
    ball.dir_y = 0.0f;
    paddle_left.x = -1.0f;
    paddle_left.y = 0.0f;
    paddle_right.x = 1.0f;
    paddle_right.y = 0.0f;
    top_bumper.y = 1.0f;
    top_bumper.x = 0.0f;
    bottom_bumper.y = -1.0f;
    bottom_bumper.x = 0.0f;
}

bool ProcessEvents() {
	SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            return false;
        }
    }
    
    //PADDLE MOVEMENT
    
    if (keys[SDL_SCANCODE_W])
        paddle_left.dir_y = 1.0f;
    else if (keys[SDL_SCANCODE_S])
        paddle_left.dir_y = -1.0f;
    else
        paddle_left.dir_y = 0.0f;
    
    if (keys[SDL_SCANCODE_UP])
        paddle_right.dir_y = 1.0f;
    else if (keys[SDL_SCANCODE_DOWN])
        paddle_right.dir_y = -1.0f;
    else
        paddle_right.dir_y = 0.0f;
    
    
    //COLLISIONS... ugh collisions...
    
    //ball collide with right paddle
    if (((ball.x + ball.width * 0.5) > (paddle_right.x - paddle_right.width * 0.5)) &&
        ((ball.x + ball.width * 0.5) < (paddle_right.x)) && //for top hit check if collide before x on paddle far side
        ((ball.y + ball.height * 0.5) > (paddle_right.y - paddle_right.height * 0.5)) &&
        ((ball.y - ball.height * 0.5) < (paddle_right.y + paddle_right.height * 0.5))) {
        ball.x = (paddle_right.x - paddle_right.width * 0.5) - (ball.width * 0.5);
        ball.dir_x = -ball.dir_x;
        float speed = (ball.y - paddle_right.y) / (paddle_right.height * 0.5);
        ball.dir_y = speed;
        screen_red = float(rand() % 10) / 10;
        screen_green = float(rand() % 10) / 10;
        screen_blue = float(rand() % 10) / 10;
    }
    
    //ball collide with left paddle
    if (((ball.x - ball.width * 0.5) < (paddle_left.x + paddle_left.width * 0.5)) &&
        ((ball.x - ball.width * 0.5) > (paddle_left.x)) &&
        ((ball.y + ball.height * 0.5) > (paddle_left.y - paddle_left.height * 0.5)) &&
        ((ball.y - ball.height * 0.5) < (paddle_left.y + paddle_left.height * 0.5))) {
        ball.x = (paddle_left.x + paddle_left.width * 0.5) + (ball.width * 0.5);
        ball.dir_x = -ball.dir_x;
        float speed = (ball.y - paddle_left.y) / (paddle_left.height * 0.5);
        ball.dir_y = speed;
        screen_red = float(rand() % 10) / 10;
        screen_green = float(rand() % 10) / 10;
        screen_blue = float(rand() % 10) / 10;
    }
    
    //ball collide with top bumper
    if ((ball.y + ball.height * 0.5) > (top_bumper.y - top_bumper.height * 0.5)) {
        ball.y = (top_bumper.y - top_bumper.height * 0.5) - (ball.height * 0.5);
        ball.dir_y = -ball.dir_y;
    }
    
    //ball collide with bottom bumper
    if ((ball.y - ball.height * 0.5) < (bottom_bumper.y + bottom_bumper.height * 0.5)) {
        ball.y = (bottom_bumper.y + bottom_bumper.height * 0.5) + (ball.height * 0.5);
        ball.dir_y = -ball.dir_y;
    }
    
    //right paddle collide with top bumper
    if ((paddle_right.y + paddle_right.height * 0.5) > (top_bumper.y - top_bumper.height * 0.5))
        paddle_right.y = (top_bumper.y - top_bumper.height * 0.5) - (paddle_right.height * 0.5);
    //right paddle collide with bottom bumper
    if ((paddle_right.y - paddle_right.height * 0.5) < (bottom_bumper.y + top_bumper.height * 0.5))
        paddle_right.y = (bottom_bumper.y + bottom_bumper.height * 0.5) + (paddle_right.height * 0.5);
    
    //left paddle collide with top bumper
    if ((paddle_left.y + paddle_left.height * 0.5) > (top_bumper.y - top_bumper.height * 0.5))
        paddle_left.y = (top_bumper.y - top_bumper.height * 0.5) - (paddle_left.height * 0.5);
    //left paddle collide with bottom bumper
    if ((paddle_left.y - paddle_left.height * 0.5) < (bottom_bumper.y + top_bumper.height * 0.5))
        paddle_left.y = (bottom_bumper.y + bottom_bumper.height * 0.5) + (paddle_left.height * 0.5);
    
    //SCORING
    if (ball.x > (paddle_right.x + paddle_right.width)) {
        SetValues(false);
        screen_red = 1.0f;
        screen_green = 1.0f;
        screen_blue = 1.0f;
        screen_fade = true;
    }
    if (ball.x < (paddle_left.x - paddle_left.width)) {
        SetValues();
        screen_red = 1.0f;
        screen_green = 1.0f;
        screen_blue = 1.0f;
        screen_fade = true;
    }
    
    if (screen_fade) {
        screen_red -= 0.02;
        screen_green -= 0.02;
        screen_blue -= 0.02;
        if (screen_red <= 0.0)
            screen_fade = false;
    }
    
    return true;
}

void Update(float elapsed) {
    paddle_left.y += paddle_left.dir_y * elapsed;
    paddle_right.y += paddle_right.dir_y * elapsed;
    ball.x += ball.dir_x * elapsed;
    ball.y += ball.dir_y * elapsed;
}

void Render() {
    glClearColor(screen_red, screen_green, screen_blue, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    ball.SetSize(0.1, 0.1);
    ball.Draw();
    paddle_left.SetSize(0.3, 0.1);
    paddle_left.Draw();
    paddle_right.SetSize(0.3, 0.1);
    paddle_right.Draw();
    
    top_bumper.SetSize(0.1, 2);
    top_bumper.Draw();
    bottom_bumper.SetSize(0.1, 2);
    bottom_bumper.Draw();
    
    net.SetSize(1.5, 0.02);
    net.Draw();
    
    SDL_GL_SwapWindow(displayWindow);
}

void Cleanup() {
    // ???
}

int main(int argc, char *argv[]) {
    Setup();
    
    float lastFrameTicks = 0.0f;
    
    SetValues();
    
	while (ProcessEvents()) {
        
        float ticks = (float)SDL_GetTicks() / 1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        Update(elapsed);
        
        Render();
    }
    
	SDL_Quit();
	return 0;
}