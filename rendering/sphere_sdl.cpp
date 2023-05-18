#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
// #include <GL/glu.h> // Include the OpenGL Utility Library (GLU)
#include <cmath>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

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

void renderSphere()
{
    const float radius = 0.5f;
    const int slices = 50;
    const int stacks = 50;

    for (int i = 0; i < slices; ++i)
    {
        float theta1 = (static_cast<float>(i) / slices) * 2 * M_PI;
        float theta2 = (static_cast<float>(i + 1) / slices) * 2 * M_PI;

        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= stacks; ++j)
        {
            float phi = (static_cast<float>(j) / stacks) * M_PI;
            float x, y, z;

            // Vertex 1
            x = radius * std::sin(theta1) * std::sin(phi);
            y = radius * std::cos(phi);
            z = radius * std::cos(theta1) * std::sin(phi);
            glVertex3f(x, y, z);

            // Vertex 2
            x = radius * std::sin(theta2) * std::sin(phi);
            y = radius * std::cos(phi);
            z = radius * std::cos(theta2) * std::sin(phi);
            glVertex3f(x, y, z);
        }
        glEnd();
    }
}

int main(int argc, char* argv[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("SDL initialization failed: %s", SDL_GetError());
        return -1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("3D Sphere", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    if (!window)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Create an OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext)
    {
        SDL_Log("Failed to create OpenGL context: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set up the perspective projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    setupProjection();

    // Enter the main loop
    bool quit = false;
    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                quit = true;
        }

        // Clear the buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up the modelview matrix
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -2.0f);  // Move the sphere away from the camera

        // Render the sphere
        renderSphere();

        // Swap the buffers
        SDL_GL_SwapWindow(window);
    }

    // Clean up
    SDL_GL_DeleteContext(glContext);

}
