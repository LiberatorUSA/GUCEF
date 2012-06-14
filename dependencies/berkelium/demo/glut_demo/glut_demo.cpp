/*  Berkelium GLUT Embedding
 *  glut_demo.cpp
 *
 *  Copyright (c) 2009, Ewen Cheslack-Postava
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of Sirikata nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "berkelium/Berkelium.hpp"
#include "berkelium/Window.hpp"
#include "berkelium/WindowDelegate.hpp"
#include "berkelium/Context.hpp"

#include <glut_util/glut_util.hpp>

#include <string>
#include <cmath>
#include <cstring>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#ifdef _WIN32
#include <time.h>
#include <windows.h>
#else
#include <sys/select.h>
#endif

using namespace Berkelium;

// Some global data:
GLTextureWindow* bk_texture_window = NULL;

// Current angle for animations
float angle = 0;

// And some global constants
#define WIDTH 512
#define HEIGHT 512
#define USE_TRANSPARENCY true

void loadURL(std::string url) {
    if (bk_texture_window == NULL)
        return;

    bk_texture_window->clear();

    // And navigate to a new one
    bk_texture_window->getWindow()->navigateTo(url.data(), url.length());
}

void loadRandomPage() {
#define NUM_SITES 4
    static std::string sites[NUM_SITES] = {
        std::string("http://berkelium.org"),
        std::string("http://google.com"),
        std::string("http://xkcd.com"),
        std::string("http://slashdot.org")
    };

    unsigned int x = rand() % NUM_SITES;
    loadURL(sites[x]);
}

void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );

    glPushMatrix();

    float scale = sinf( angle ) * 0.25f + 1.f;
    glScalef( scale, scale, 1.f );

    glRotatef(angle*2.f, 0.f, 0.f, 1.f);

    glColor3f(1.f, 1.f, 1.f);

    bk_texture_window->bind();

    glBegin(GL_QUADS);
    glTexCoord2f(0.f, 0.f); glVertex3f(-1.f, -1.f, 0.f);
    glTexCoord2f(0.f, 1.f); glVertex3f(-1.f,  1.f, 0.f);
    glTexCoord2f(1.f, 1.f); glVertex3f( 1.f,  1.f, 0.f);
    glTexCoord2f(1.f, 0.f); glVertex3f( 1.f, -1.f, 0.f);
    glEnd();

    bk_texture_window->bind();

    glPopMatrix();

    glutSwapBuffers();
}

void reshape( int w, int h )
{
    glClearColor( .3, .3, .3, 1 );

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glOrtho( -1.f, 1.f, 1.f, -1.f, -1.f, 1.f );
    glViewport( 0, 0, w, h );

    glutPostRedisplay();
}

void keyboard( unsigned char key, int x, int y )
{
    if (key == 27 || key == 'q') { // ESC
        delete bk_texture_window;
        Berkelium::destroy();
        exit(0);
    }
    else if (key == 'n' || key == ' ') {
        loadRandomPage();
    }

    glutPostRedisplay();
}

void special_keyboard(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT)
        bk_texture_window->getWindow()->mouseWheel(20, 0);
    else if (key == GLUT_KEY_RIGHT)
        bk_texture_window->getWindow()->mouseWheel(-20, 0);
    else if (key == GLUT_KEY_UP)
        bk_texture_window->getWindow()->mouseWheel(0, 20);
    else if (key == GLUT_KEY_DOWN)
        bk_texture_window->getWindow()->mouseWheel(0, -20);

    glutPostRedisplay();
}

// FIXME we're using idle and sleep because the GLUT and Chromium message loops
// seem to conflict when using GLUT timers
void idle() {
    {
#ifdef _WIN32
        Sleep(30);
#else
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 30000;
        select(0,NULL,NULL,NULL, &tv);
#endif
    }

    Berkelium::update();

    angle = angle + .1f;
    if (angle > 360.f)
        angle = 0.f;

    glutPostRedisplay();
}

int main (int argc, char** argv) {

    // Create a GLUT window and setup callbacks
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
    glutInitWindowSize( WIDTH, HEIGHT );
    glutCreateWindow( "Berkelium Demo" );
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special_keyboard);
    glutIdleFunc(idle);

    // Initialize Berkelium and create a window
    if (!Berkelium::init(FileString::empty())) {
        std::cout << "Failed to initialize berkelium!" << std::endl;
        return 1;
    }
    bk_texture_window = new GLTextureWindow(WIDTH, HEIGHT, USE_TRANSPARENCY);
    bk_texture_window->window()->focus();

    if (argc < 2) {
        loadRandomPage();
    } else {
        loadURL(argv[1]);
    }

    // Start the main rendering loop
    glutMainLoop();

    return 0;
}
