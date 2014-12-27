#include <GL/glew.h>
#include <GL/glfw.h>
#include <math.h>
#include <vector>

#include "Player.h"
#include "ChunkManager.h"
#include "Shader.h"

#define _maxspeed 6
#define _gravity 20.0
#define _maxJumpHeight  1.0
#define _terminalVelocity  50


void init(void);
void enable(void);
void lighting(void);
void gameloop(void);
void shutdown(int);

void update(double);
void render(void);

void lookAround(void);
Vector3D interpolate(double);
Vector3D collide(Vector3D, double);

int OK = 0;
int ERROR = 1;

float _jumpSpeed = sqrt(2 * _gravity * _maxJumpHeight);

// Player is 1u wide, 2u tall, 1u long.
// Player camera is 1.75u from bottom of player


// TODO camera radius
int cameraRadius = 10.0f;

// Sensitivity
float _mouseSensitivity = 0.1;

/* Axes display list */
static GLuint axes_list;

Player player;
ChunkManager chunkManager;
Shader shader;

Vector3D _faces[] = {
    Vector3D(0, 1, 0),
    Vector3D(0,-1, 0),
    Vector3D(-1, 0, 0),
    Vector3D(1, 0, 0),
    Vector3D(0, 0, 1),
    Vector3D(0, 0,-1),
};


int main(int argc, char** argv)
{
    init();
    enable();
    lighting();

    gameloop();

    shutdown(OK);
}

void initDebugInfo()
{
    if (DEBUG) 
    {
        /* Create a display list for drawing axes */
        axes_list = glGenLists(1);
        glNewList(axes_list, GL_COMPILE);
     
        glColor4ub(255, 0, 0, 255);
        glBegin(GL_LINE_STRIP);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.75f, 0.25f, 0.0f);
        glVertex3f(0.75f, -0.25f, 0.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.75f, 0.0f, 0.25f);
        glVertex3f(0.75f, 0.0f, -0.25f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glEnd();

        glColor4ub(0, 255, 0, 255);
        glBegin(GL_LINE_STRIP);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.75f, 0.25f);
        glVertex3f(0.0f, 0.75f, -0.25f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.25f, 0.75f, 0.0f);
        glVertex3f(-0.25f, 0.75f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glEnd();

        glColor4ub(0, 0, 255, 255);
        glBegin(GL_LINE_STRIP);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.25f, 0.0f, 0.75f);
        glVertex3f(-0.25f, 0.0f, 0.75f);
        glVertex3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.25f, 0.75f);
        glVertex3f(0.0f, -0.25f, 0.75f);
        glVertex3f(0.0f, 0.0f, 1.0f);
        glEnd();
     
        glColor4ub(255, 255, 0, 255);
        glRasterPos3f(1.1f, 0.0f, 0.0f);
     
        glEndList();
    }
}

/**
 * Initialize some parameters
 */
void init()
{
    if (glfwInit() != GL_TRUE)
        shutdown(ERROR);

    // Desktop parameters
    GLFWvidmode desktop;
    glfwGetDesktopMode(&desktop);

    // Create window
    if (glfwOpenWindow(desktop.Width, desktop.Height, 8, 8, 8, 8, 24, 8, GLFW_FULLSCREEN) != GL_TRUE)
        shutdown(ERROR);

    // Initialize mouse
    glfwDisable(GLFW_MOUSE_CURSOR);
    glfwSetMousePos(0,0);
    
    // Initialize GLEW
	glewInit();
	if (!glewIsSupported("GL_VERSION_2_0"))
		exit(1);
    printf("OpenGL version supported by this platform : %s\n", glGetString(GL_VERSION));

    glViewport(0, 0, (GLsizei)desktop.Width, (GLsizei)desktop.Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set the perspective (angle of sight, perspective, depth)
    gluPerspective(60, (GLfloat)desktop.Width/ (GLfloat)desktop.Height, 0.1, 100.0); 
    glMatrixMode(GL_MODELVIEW);

    // Initialize debug info
    initDebugInfo();

    // Initialize world
    chunkManager.initializeWorld();

    // Initialize player position
    Vector3D position(5, 5, 5);
    player.setPosition(position);

    glClearColor(0, 0, 0, 0);
    glClearDepth(1);

}

/**
 * Enable GL stuff
 */
void enable()
{
    // Space vertex winding
    glFrontFace(GL_CCW);

	//glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

	glEnable(GL_DEPTH_TEST);

    // Keep current colors
	glEnable(GL_COLOR_MATERIAL);
    

    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};  /* White diffuse light. */
    //GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  /* Infinite light location. */

    /* Enable a single OpenGL light. */
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    //glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT1); //Enable light #1
	//glEnable(GL_NORMALIZE); //Automatically normalize normals

    // Back face culling
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glCullFace(GL_FRONT);

    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );

    //glShadeModel(GL_FLAT);
    glShadeModel(GL_SMOOTH);
    
	// Allows z-buffering 
    // Can chew up extra CPU time and graphics memory for very complicated scenes
    //glDepthMask(GL_TRUE);
}

/**
 * Lighting parameters
 */
void lighting()
{
    shader.createShader("shaders/VertexShader", "shaders/FragmentShader");
}

/*
 * Shutdown method
 */
void shutdown(int code)
{
    glfwTerminate();
    exit(code);
}

/**
 * The main update / render loop.
 */
void gameloop()
{
    const double dt = 0.01;

    double prevTime = glfwGetTime();
    double accumulator = 0.0;

    while (true)
    {
        // Calculate time since last frame
        double currentTime = glfwGetTime();
        double delta = currentTime - prevTime;

        if (delta > 0.25)
            delta = 0.25;

        accumulator += delta;

        prevTime = currentTime;

        // Clear screen to black
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        while ( accumulator >= dt )
        {
            update(dt);
            accumulator -= dt;
        }
        render();
    }
}

/**
 * Update the state of the world.
 *
 * @param delta - time since last update
 */
void update(double delta)
{
    // Handle mouse
    lookAround();

    Vector3D position = player.getPosition();

    // Interpolate velocity
    Vector3D distance = interpolate(delta);
    // Future position
    Vector3D future(position.x + distance.x, position.y + distance.y, position.z + distance.z);
    // Fix for collisions
    player.setPosition( collide(future, delta) );

    // Update render map
    player.setPosition( chunkManager.update(player.getPosition()) );

}


/**
 * Look around.
 */
void lookAround()
{
    // Mouse
    //int mouseX, mouseY;
    //glfwGetMousePos(&mouseX, &mouseY);
    //player.offsetOrientation(mouseX * _mouseSensitivity, mouseY * _mouseSensitivity);
    //glfwSetMousePos(0,0);
    
    float offX = 0;
    float offY = 0;

    if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS)
        offY -= 1.5;
    if (glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS)
        offY += 1.5;
    if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS)
        offX -= 1.5;
    if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS)
        offX += 1.5;

    player.offsetOrientation(offX, offY);
}

/**
 * Interpolates distance gives a delta (time) since last render. 
 */
Vector3D interpolate(double delta)
{
    float xvel = 0.0f;
    float yvel = 0.0f;
    float zvel = 0.0f;

    // Compute horizontal rotation in radians
    float xrot = player.side();
    float xrotrad = xrot / 180 * 3.141592654f; 
    // Projection on x and z axes
    float xproj = _maxspeed * float(cos(xrotrad)); 
    float zproj = _maxspeed * float(sin(xrotrad));

    // Exit
    if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
        shutdown(OK);

    // Left
    if (glfwGetKey('a') == GLFW_PRESS || glfwGetKey('A') == GLFW_PRESS)
    {
        xvel -= xproj;
        zvel -= zproj;
    }

    // Forward
    if (glfwGetKey('w') == GLFW_PRESS || glfwGetKey('W') == GLFW_PRESS)
    {
        xvel += zproj;
        zvel -= xproj;
    }

    // Back
    if (glfwGetKey('s') == GLFW_PRESS || glfwGetKey('S') == GLFW_PRESS)
    {
        xvel -= zproj;
        zvel += xproj;
    }

    // Right
    if (glfwGetKey('d') == GLFW_PRESS || glfwGetKey('D') == GLFW_PRESS)
    {
        xvel += xproj;
        zvel += zproj;
    }

    // JUMP
    if (glfwGetKey(GLFW_KEY_SPACE) == GLFW_PRESS && !player.isFalling())
    {
        player.setFalling(true);
        yvel += _jumpSpeed;
    }


    Vector3D distance(xvel * delta, yvel, zvel * delta);
    return distance;

}

/**
 * Returns a distance vector after fixing collisions. 
 */
Vector3D collide(Vector3D position, double delta)
{
    // Padding = 1/4 * BLOCK_HEIGHT
    float padding = 0.25 * BLOCK_HEIGHT;

    // Player is falling unless proven not to be 
    player.setFalling(true);

    // 'Center' corresponding to current position
    // Rounds to nearest height, width, length increment since
    // the first Block is centered around (0, 0, 0)
    Vector3D np( BLOCK_WIDTH * roundf(position.x / BLOCK_WIDTH),
            BLOCK_HEIGHT * roundf(position.y / BLOCK_HEIGHT),
            BLOCK_LENGTH * roundf(position.z / BLOCK_LENGTH) );

    // Check each face
    for (int i = 0; i < 6; i++)
    {

        Vector3D face = _faces[i];
        // Check each dimension
        for(int dim = 0; dim < 3; dim++)
        {
            if (face[dim] == 0)
                continue;

            // Overlap allowed for this dimension
            float overlap = (position[dim] - np[dim]) * face[dim];
            if (overlap < padding)
                continue;

            // Check all in height range
            for (int dy = 0; dy < int(3 / BLOCK_HEIGHT); dy++)
            {
                Vector3D op = np;
                op.y -= dy*BLOCK_HEIGHT;
                op[dim] += face[dim];

                // Is this block active?
                if (!chunkManager.isActive(op))
                    continue;
                position[dim] -= (overlap - padding) * face[dim];

                if (face[dim] != 0)
                    player.setFalling(false);
            }
        }
    }
    
    // Gravity
    if(player.isFalling())
    {
        position.y -= _gravity  * delta;
        //yvel -= delta * _gravity; 
        //yvel = yvel > -_terminalVelocity? -_terminalVelocity: yvel;
    }

    return position;

}

void drawCharacter()
{
    glColor3f(1.0f, 0.0f, 0.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glutSolidCube(1);
}

void drawDebugInfo()
{
    if (DEBUG)
    {
        glPushMatrix();
        glCallList(axes_list);
        glPopMatrix();
    }
}

/**
 * Rendering function
 */
void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Vertical rotation
    glRotatef(player.up(), 1.0, 0.0, 0.0);

    // Draw character
    glPushMatrix();
    // Translate camera away from player
    //glTranslatef(0.0f, 0.0f, -cameraRadius);
    drawCharacter();
    glPopMatrix();

    // Horizontal rotation
    glRotatef(player.side(), 0.0, 1.0, 0.0);

    // Draw debug info
    drawDebugInfo();

    // Translate away from player 
    Vector3D position = player.getPosition();
    glTranslated(-position.x, -position.y, -position.z);

    shader.begin();

    // Draw environment
    glPushMatrix();
    chunkManager.render();
    glPopMatrix();

    shader.end();


    // Swap back and front buffers
    glfwSwapBuffers();
}
