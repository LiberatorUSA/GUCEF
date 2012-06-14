/*  Berkelium GLUT Embedding
 *  glut_input.cpp
 *
 *  Copyright (c) 2010, Ewen Cheslack-Postava
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

// And some global constants
#define WIDTH 1024
#define HEIGHT 1024
#define USE_TRANSPARENCY true

unsigned int win_width = WIDTH;
unsigned int win_height = HEIGHT;

void loadURL(std::string url) {
    if (bk_texture_window == NULL)
        return;

    bk_texture_window->clear();

    bk_texture_window->getWindow()->navigateTo(url.data(), url.length());

    glutPostRedisplay();
}

void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );

    glColor3f(1.f, 1.f, 1.f);

    bk_texture_window->bind();

    glBegin(GL_QUADS);
    glTexCoord2f(0.f, 0.f); glVertex3f(-1.f, -1.f, 0.f);
    glTexCoord2f(0.f, 1.f); glVertex3f(-1.f,  1.f, 0.f);
    glTexCoord2f(1.f, 1.f); glVertex3f( 1.f,  1.f, 0.f);
    glTexCoord2f(1.f, 0.f); glVertex3f( 1.f, -1.f, 0.f);
    glEnd();

    bk_texture_window->bind();

    glutSwapBuffers();
}

void reshape( int w, int h )
{
    win_width = w;
    win_height = h;

    glClearColor( .3, .3, .3, 1 );

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glOrtho( -1.f, 1.f, 1.f, -1.f, -1.f, 1.f );
    glViewport( 0, 0, w, h );

    glutPostRedisplay();
}

void keyboard( unsigned char key_, int x, int y )
{
    unsigned int key = key_;
    if (key == 27) { // ESC
        delete bk_texture_window;
        Berkelium::destroy();
        exit(0);
    }
    if (key == ']') { // unicode test
        key = 0x6708; // Chinese character [moon].
    }
    if (key == '[') { // surrogate test
        // The bug that occurs when backspacing this happens in standalone chrome too.
        key = 0x201d1; // Chinese character [person + father], surrogate pair.
    }

    // Some keys that come through this are actually special keys, despite being
    // represented as ASCII characters.
    if (key == '\b' || key == '\r' || key == '\n' || key == ' ' || key == 127 ||
        key >= 'a' && key <= 'z' || key >= 'A' && key <= 'Z') {
        bool pressed = true;
        int wvmods = mapGLUTModsToBerkeliumMods(glutGetModifiers());
        int vk_code = key == 127 ? BK_KEYCODE_DELETE : tolower(key);
        int scancode = 0;

        bk_texture_window->getWindow()->keyEvent(pressed, wvmods, vk_code, scancode);
    }
    if (!isASCIISpecialToBerkelium(key)) {
        // Regular text can be sent via textEvent
        wchar_t outchars[2];
        outchars[0] = key;
        outchars[1] = 0;
        bk_texture_window->getWindow()->textEvent(outchars,1);
        if (key == ';') {
            wchar_t outchars[20] = L"Hello World! 123456";
            bk_texture_window->getWindow()->textEvent(outchars,19);
        }
    }

    glutPostRedisplay();
}

void special_keyboard(int key, int x, int y) {
// Sample code for using wheel:
/*
    if (key == GLUT_KEY_LEFT)
        bk_texture_window->getWindow()->mouseWheel(20, 0);
    else if (key == GLUT_KEY_RIGHT)
        bk_texture_window->getWindow()->mouseWheel(-20, 0);
    else if (key == GLUT_KEY_UP)
        bk_texture_window->getWindow()->mouseWheel(0, 20);
    else if (key == GLUT_KEY_DOWN)
        bk_texture_window->getWindow()->mouseWheel(0, -20);
    else
*/
    {
        bool pressed = true;
        int wvmods = mapGLUTModsToBerkeliumMods(glutGetModifiers());
        int vk_code = mapGLUTKeyToBerkeliumKey(key);
        int scancode = 0;

        bk_texture_window->getWindow()->keyEvent(pressed, wvmods, vk_code, scancode);
    }

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    unsigned int tex_coord_x = mapGLUTCoordToTexCoord(x, win_width, WIDTH);
    unsigned int tex_coord_y = mapGLUTCoordToTexCoord(y, win_height, HEIGHT);

    // Make sure Berkelium knows the mouse has moved over the where the event is happening
    bk_texture_window->getWindow()->mouseMoved(tex_coord_x, tex_coord_y);

// GLUT can generate these buttons, but doesn't define them
#define _GLUT_SCROLL_UP_BUTTON 3
#define _GLUT_SCROLL_DOWN_BUTTON 4

    // And figure out precisely what to inject
    if (button == GLUT_LEFT_BUTTON || button == GLUT_MIDDLE_BUTTON || button == GLUT_RIGHT_BUTTON) {
        bk_texture_window->getWindow()->mouseButton(
            button,
            (state == GLUT_DOWN)
        );
    }
    else if (button == _GLUT_SCROLL_UP_BUTTON) {
        bk_texture_window->getWindow()->mouseWheel(0, 20);
    }
    else if (button == _GLUT_SCROLL_DOWN_BUTTON) {
        bk_texture_window->getWindow()->mouseWheel(0, -20);
    }
}

void motion(int x, int y) {
    unsigned int tex_coord_x = mapGLUTCoordToTexCoord(x, win_width, WIDTH);
    unsigned int tex_coord_y = mapGLUTCoordToTexCoord(y, win_height, HEIGHT);
    bk_texture_window->getWindow()->mouseMoved(tex_coord_x, tex_coord_y);
}

void passive_motion(int x, int y) {
    unsigned int tex_coord_x = mapGLUTCoordToTexCoord(x, win_width, WIDTH);
    unsigned int tex_coord_y = mapGLUTCoordToTexCoord(y, win_height, HEIGHT);
    bk_texture_window->getWindow()->mouseMoved(tex_coord_x, tex_coord_y);
}

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

    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passive_motion);

    glutIdleFunc(idle);

    // Initialize Berkelium and create a window
    if (!Berkelium::init(
#ifndef _WIN32
            argc > 2 ? Berkelium::FileString::point_to(argv[2], strlen(argv[2])) :
#endif
                Berkelium::FileString::empty())) {
        std::cout << "Failed to initialize berkelium!" << std::endl;
        return 1;
    }
    bk_texture_window = new GLTextureWindow(WIDTH, HEIGHT, USE_TRANSPARENCY);
    bk_texture_window->window()->addBindOnStartLoading(
        Berkelium::WideString::point_to(L"glutCallback"),
        Berkelium::Script::Variant::bindFunction(Berkelium::WideString::point_to(L"glutCB"), false));
    bk_texture_window->window()->addBindOnStartLoading(
        Berkelium::WideString::point_to(L"glutCallbackSync"),
        Berkelium::Script::Variant::bindFunction(Berkelium::WideString::point_to(L"glutCB"), true));
    bk_texture_window->window()->addBindOnStartLoading(
        Berkelium::WideString::point_to(L"glutObjectTest"),
        Berkelium::Script::Variant::emptyObject());
    bk_texture_window->window()->addBindOnStartLoading(
        Berkelium::WideString::point_to(L"glutObjectTest.someArray"),
        Berkelium::Script::Variant::emptyArray());
    bk_texture_window->window()->addBindOnStartLoading(
        Berkelium::WideString::point_to(L"glutObjectTest.someArray[0]"),
        Berkelium::Script::Variant::bindFunction(Berkelium::WideString::point_to(L"arrayFunc"), false));
    bk_texture_window->window()->addBindOnStartLoading(
        Berkelium::WideString::point_to(L"glutObjectTest.someArray[1]"),
        Berkelium::Script::Variant::bindFunction(Berkelium::WideString::point_to(L"arrayFunc"), true));
    bk_texture_window->window()->addBindOnStartLoading(
        Berkelium::WideString::point_to(L"glutObjectTest.nullValue"),
        Berkelium::Script::Variant());
    bk_texture_window->window()->addBindOnStartLoading(
        Berkelium::WideString::point_to(L"glutObjectTest.stringValue"),
        Berkelium::Script::Variant("Hello, World!"));
    bk_texture_window->window()->focus();

    if (argc < 2) {
        loadURL("http://www.google.com");
    } else {
        loadURL(argv[1]);
    }

    // Start the main rendering loop
    glutMainLoop();

    return 0;
}
