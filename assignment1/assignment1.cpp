//
//  assignment1.cpp
//  Resonance Blossoms Make The Dead
//
//  Created by Panagis Alisandratos on 9/13/14.
//
// animated elements: flower, and wave
// textures used: 4 different zombie hands (place zombie_hands folder in Supporting Files group)
// elements using vertex colors: flower, wave, and ground

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#define PI 3.14159265

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path) {
    
    SDL_Surface *surface = IMG_Load(image_path);
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 surface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    SDL_FreeSurface(surface);
    
    return textureID;
}

void DrawSprite(GLuint texture, float x, float y, float scale, float rotation = 0) {
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, 0.0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    glScalef(scale, scale, 0.0);
    
    GLfloat quad[] = {-0.1f, 0.25f, -0.1f, -0.25f, 0.1f, -0.25f, 0.1f, 0.25f};
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

void DrawPinWheel(float x, float y, float angle, float scale, float gradient_val_top = 1.0, float gradient_val_bottom = 1.0) {
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    float triangle_vertices[4][6] = {
        {0.0f, 0.0f, 0.0f, -0.3f, 0.1f, -0.2f},
        {0.0f, 0.0f, 0.0f, 0.3f, -0.1f, 0.2f},
        {0.0f, 0.0f, 0.3f, 0.0f, 0.2f, 0.1f},
        {0.0f, 0.0f, -0.3f, 0.0f, -0.2f, -0.1f},
    };
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glTranslatef(x, y, 0.0); //must place before rotate!!!
    glScalef(scale, scale, 0.0);
    glRotatef(angle, 0.0, 0.0, 1.0); // placing withing loop causes strange behavior, degrees seem to accumulate

    for (float* vertex : triangle_vertices) {
        
        GLfloat triangle[] = {vertex[0],
            vertex[1],
            vertex[2],
            vertex[3],
            vertex[4],
            vertex[5]
        };

        glVertexPointer(2, GL_FLOAT, 0, triangle);
        
        //palette set to yellow/red
        GLfloat triangleColors[] = {1.0, 1.0, gradient_val_top, 1.0, gradient_val_bottom, gradient_val_bottom, 1.0, 1.0, gradient_val_top};
        glColorPointer(3, GL_FLOAT, 0, triangleColors);
        glEnableClientState(GL_COLOR_ARRAY);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableClientState(GL_COLOR_ARRAY);
    }
}

//void DrawEmptyQuad(float x, float y, float angle, float scale) {
//
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glTranslatef(x, y, 0.0);
//    glScalef(scale, scale, 0.0);
//    glRotatef(angle, 0.0, 0.0, 1.0);
//    
//    GLfloat quad[] = { -0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5 };
//    glVertexPointer(2, GL_FLOAT, 0, quad);
//    glDrawArrays(GL_LINES, 0, 4);
//    GLfloat quad1[] = { 0.5, 0.5, -0.5, 0.5, -0.5, -0.5, 0.5, -0.5 };
//    glVertexPointer(2, GL_FLOAT, 0, quad1);
//    glDrawArrays(GL_LINES, 0, 4);
//}

void DrawEmptyOctagon(float x, float y, float angle, float scale, bool rainbow = true) {
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glTranslatef(x, y, 0.0);
    glScalef(scale, scale, 0.0);
    glRotatef(angle, 0.0, 0.0, 1.0);
   
    GLfloat octagon[] = {
        -0.5, 0.2,
        -0.5, -0.2,
        -0.2, -0.5,
        0.2, -0.5,
        0.5, -0.2,
        0.5, 0.2,
        0.2, 0.5,
        -0.2, 0.5
    };
    
    glVertexPointer(2, GL_FLOAT, 0, octagon);
    
    if (rainbow) {
        GLfloat octagonColors[] = {
            1.0, 1.0, 0.0,
            1.0, 0.0, 1.0,
            1.0, 0.0, 0.0,
            1.0, 1.0, 1.0,
            0.0, 1.0, 1.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0,
            1.0, 1.0, 1.0,
        };
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(3, GL_FLOAT, 0, octagonColors);
    }
    glPolygonMode(GL_FRONT, GL_LINE);
    glDrawArrays(GL_POLYGON, 0, 8);
    glPolygonMode(GL_FRONT, GL_FILL);
    glDisableClientState(GL_COLOR_ARRAY);
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Resonance Blossums Make The Dead", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
    
	bool done = false;
	
	SDL_Event event;
    
    glViewport(0, 0, 800, 600); //set size and pixel offset of rendering area
    glMatrixMode(GL_PROJECTION); //select projection matrix
    glLoadIdentity(); //load identity matrix
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0); //multiply current selected matrix (identity matrix) with orthographic projection matrix
    glMatrixMode(GL_MODELVIEW);

    // initial values
    bool drop = true;
    float wave_timer = 50;
    float wave_scale = 0;
    float bud_y = 1.0f;
    float bud_angle = 0;
    float bud_scale = 1;
    float stem_y = -0.8f;
    float wheel_y = 0;
    float wheel_x = 0;
    float wheel_scale = 0;
    float wheel_angle = 0;
    float wheel_speed = 0;
    float wheel_max_speed = 20;
    float wheel_gradient_top = 1.0f;
    float wheel_gradient_bottom = 1.0f;
    float deg_top = 45;
    float deg_bottom = 0;
    float lastFrameTicks = 0.0f;
    float hands_y[8][2] = {
        { -float(rand() % 2 + 10)/10, -float(rand() % 20 + 70)/100 },
        { -float(rand() % 2 + 10)/10, -float(rand() % 20 + 70)/100 },
        { -float(rand() % 2 + 10)/10, -float(rand() % 20 + 70)/100 },
        { -float(rand() % 2 + 10)/10, -float(rand() % 20 + 70)/100 },
        { -float(rand() % 2 + 10)/10, -float(rand() % 20 + 70)/100 },
        { -float(rand() % 2 + 10)/10, -float(rand() % 20 + 70)/100 },
        { -float(rand() % 2 + 10)/10, -float(rand() % 20 + 70)/100 },
        { -float(rand() % 2 + 10)/10, -float(rand() % 20 + 70)/100 },
    };
    
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
        
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        //hands
        DrawSprite(LoadTexture("zombie_hands/zombie_hand_0000_Layer-4.png"), -0.2, hands_y[0][0], 0.5);
        DrawSprite(LoadTexture("zombie_hands/zombie_hand_0001_Layer-3.png"), -0.8, hands_y[1][0], 0.5);
        DrawSprite(LoadTexture("zombie_hands/zombie_hand_0002_Layer-2.png"), 0.4, hands_y[2][0], 0.5);
        DrawSprite(LoadTexture("zombie_hands/zombie_hand_0003_Layer-1.png"), 1.0, hands_y[3][0], 0.5);
        DrawSprite(LoadTexture("zombie_hands/zombie_hand_0000_Layer-4.png"), 1.2, hands_y[4][0], 0.5);
        DrawSprite(LoadTexture("zombie_hands/zombie_hand_0001_Layer-3.png"), 0.6, hands_y[5][0], 0.5);
        DrawSprite(LoadTexture("zombie_hands/zombie_hand_0002_Layer-2.png"), -0.6, hands_y[6][0], 0.5);
        DrawSprite(LoadTexture("zombie_hands/zombie_hand_0003_Layer-1.png"), -1.0, hands_y[7][0], 0.5);
        
        //ground
        glLoadIdentity();
        GLfloat ground[] = {-1.33f, -0.8f, -1.33f, -1.0f, 1.33f, -1.0f, 1.33f, -0.8f};
        glVertexPointer(2, GL_FLOAT, 0, ground);
        glEnableClientState(GL_VERTEX_ARRAY);
        
        GLfloat groundColors[] = {wheel_gradient_top, 1.0, wheel_gradient_top, 1.0, 1.0, wheel_gradient_bottom, 1.0, 1.0, wheel_gradient_bottom, wheel_gradient_top, 1.0, wheel_gradient_top};
        glColorPointer(3, GL_FLOAT, 0, groundColors);
        glEnableClientState(GL_COLOR_ARRAY);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableClientState(GL_COLOR_ARRAY);
        
        //stem
        glLoadIdentity();
        GLfloat stem[] = {0.0f, -0.8f, 0.0f, stem_y};
        glVertexPointer(2, GL_FLOAT, 0, stem);
        glEnableClientState(GL_VERTEX_ARRAY);
        glDrawArrays(GL_LINES, 0, 2);
        
        if (drop) {
            bud_y -= 0.02;
            bud_angle += 10;
            if (bud_y < -0.8) {
                bud_y = -0.8;
                bud_angle = 45;
                drop = false;
            }
        }
        else {
            if (bud_y < 0) {
                bud_y += 0.01;
                stem_y += 0.01;
            }
            else {
                bud_y = 0;
                stem_y = 0;
                //flower
                DrawPinWheel(wheel_x, wheel_y, wheel_angle, wheel_scale, wheel_gradient_top, wheel_gradient_bottom);
                if (wheel_scale < 1)
                wheel_scale += 0.01;
                if (wheel_speed < wheel_max_speed)
                wheel_speed += elapsed * 5; //acceleration
                wheel_angle += wheel_speed;
                bud_angle = 45 + wheel_angle;
                
                wave_timer -= 1;
            }
        }
        
        //bud (draw in front -- less depth)
        glLoadIdentity();
        GLfloat bud[] = {-0.02f, 0.02f, -0.02f, -0.02f, 0.02f, -0.02f, 0.02f, 0.02f};
        glVertexPointer(2, GL_FLOAT, 0, bud);
        glEnableClientState(GL_VERTEX_ARRAY);
        glTranslatef(0.0, bud_y, 0.0);
        glRotatef(bud_angle, 0.0, 0.0, 1.0);
        glScalef(bud_scale, bud_scale, 0.0);
        glDrawArrays(GL_QUADS, 0, 4);
        
        if (wave_timer <= 0) {
            // if the code used in DrawEmptyOctagon (GL_POLYGON an glPolygonMode() ) is not allowed for this hw, please comment out the function call and use DrawEmptyQuad instead!
            DrawEmptyOctagon(wheel_x, wheel_y, wheel_angle, wave_scale);
            // DrawEmptyQuad(wheel_x, wheel_y, wheel_angle, wave_scale);
            wave_scale += 0.05;
            bud_scale = 2;
            wheel_gradient_top = cos( deg_top += 4 * PI / 180);
            wheel_gradient_bottom = cos ( deg_bottom += 4 * PI / 180);
            for (float* hand_y : hands_y) {
                if (hand_y[0] < hand_y[1])
                    hand_y[0] += 0.001;
            }
        }
        
        if (wave_scale > 4) { //reset
            wave_scale = 0;
            wave_timer = 50;
            wheel_gradient_top = 1.0;
            wheel_gradient_bottom = 1.0;
            deg_top = 45;
            deg_bottom = 0;
        }
        
        if (bud_scale > 1)
            bud_scale -= 0.05;
        else
            bud_scale = 1;
        
		SDL_GL_SwapWindow(displayWindow);
	}
    
	SDL_Quit();
	return 0;
}