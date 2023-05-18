#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <cmath>
#include <iostream>


const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int lastMouseX = 0;  // Last recorded mouse X position
int lastMouseY = 0;  // Last recorded mouse Y position
bool mousePressed = false;  // Flag to indicate if mouse button is pressed

void handleMouseEvent(SDL_Event& event)
{
    std::cout << "Mouse pressed \n"<< std::endl;
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            // Record the initial mouse position and set the flag to indicate button press
            lastMouseX = event.motion.x;
            lastMouseY = event.motion.y;
            mousePressed = true;
        }
    }
    else if (event.type == SDL_MOUSEBUTTONUP)
    {
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            // Reset the flag to indicate button release
            mousePressed = false;
        }
    }
    else if (event.type == SDL_MOUSEMOTION)
    {
        if (mousePressed)
        {
            // Calculate the mouse movement delta
            int deltaX = event.motion.x - lastMouseX;
            int deltaY = event.motion.y - lastMouseY;

            // Adjust the camera rotation based on mouse movement
            float rotationSpeed = 0.2f;
            glRotatef(deltaX * rotationSpeed, 0.0f, 1.0f, 0.0f);  // Rotate around the y-axis
            glRotatef(deltaY * rotationSpeed, 1.0f, 0.0f, 0.0f);  // Rotate around the x-axis

            // Update the last recorded mouse position
            lastMouseX = event.motion.x;
            lastMouseY = event.motion.y;
        }
    }
}


void renderSphere(float x, float y, float z, float radius, int slices, int stacks)
{
    const float PI = 3.14159265358979323846;
    const float TWO_PI = 2.0 * PI;

    float theta, phi;
    float x1, y1, z1;
    float x2, y2, z2;
    float nx, ny, nz;
    float u, v;
    float du = TWO_PI / slices;
    float dv = PI / stacks;

    for (int i = 0; i < stacks; ++i)
    {
        phi = i * dv;
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= slices; ++j)
        {
            theta = j * du;

            x1 = radius * sin(phi) * cos(theta);
            y1 = radius * sin(phi) * sin(theta);
            z1 = radius * cos(phi);
            nx = x1 / radius;
            ny = y1 / radius;
            nz = z1 / radius;
            u = static_cast<float>(j) / slices;
            v = static_cast<float>(i) / stacks;

            glNormal3f(nx, ny, nz);
            glTexCoord2f(u, v);
            glVertex3f(x + x1, y + y1, z + z1);

            x2 = radius * sin(phi + dv) * cos(theta);
            y2 = radius * sin(phi + dv) * sin(theta);
            z2 = radius * cos(phi + dv);
            nx = x2 / radius;
            ny = y2 / radius;
            nz = z2 / radius;
            u = static_cast<float>(j) / slices;
            v = static_cast<float>(i + 1) / stacks;

            glNormal3f(nx, ny, nz);
            glTexCoord2f(u, v);
            glVertex3f(x + x2, y + y2, z + z2);
        }
        glEnd();
    }
}

void renderPlane(float size)
{
    glBegin(GL_QUADS);
    glVertex3f(-size, 0.0f, -size);
    glVertex3f(-size, 0.0f, size);
    glVertex3f(size, 0.0f, size);
    glVertex3f(size, 0.0f, -size);
    glEnd();
}

void setupProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspectRatio = static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT;
    float fov = 45.0f;  // Field of view (in degrees)
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    float top = nearPlane * std::tan(0.5f * fov * M_PI / 180.0f);
    float right = top * aspectRatio;
    glFrustum(-right, right, -top, top, nearPlane, farPlane);
}


void renderScene(SDL_Window* window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float fovY = 45.0f;                                          // Field of view in the y-direction
    float aspectRatio = static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT; // Aspect ratio of the window
    float zNear = 0.1f;                                          // Near clipping plane
    float zFar = 100.0f;                                         // Far clipping plane

    // Calculate the projection matrix
    float f = 1.0f / tan(fovY * 0.5f * 3.14159265358979323846f / 180.0f);
    float matrixProjection[16] = {
        f / aspectRatio, 0.0f, 0.0f, 0.0f,
        0.0f, f, 0.0f, 0.0f,
        0.0f, 0.0f, (zFar + zNear) / (zNear - zFar), -1.0f,
        0.0f, 0.0f, (2.0f * zFar * zNear) / (zNear - zFar), 0.0f
    };
    glMultMatrixf(matrixProjection);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);

    // Render multiple spheres
    renderSphere(0.0f, 0.0f, 0.0f, 1.0f, 100, 100);
    renderSphere(2.0f, 0.0f, -2.0f, 0.5f, 100, 100);
    renderSphere(-2.0f, 0.0f, -2.0f, 0.8f, 100, 100);
    renderPlane(1000.0f);

    // Render a plane
    // renderPlane(5.0f);

    glFlush();
    SDL_GL_SwapWindow(window);
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("3D Scene", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    setupProjection();

    bool quit = false;
    SDL_Event event;
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                quit = true;
            else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEMOTION)
                handleMouseEvent(event);
        }

        renderScene(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
