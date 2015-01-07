#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <math.h>
#include <vector>

#include "player.h"
#include "chunk_manager.h"
#include "shader.h"

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

float msPerFrame = 0.0f;

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


// font face
FT_Face face;
// The shader for the text.
Shader textShader;
// Attributes for the text shader.
GLint attribute_coord;
GLint uniform_tex;
GLint uniform_color;

// A helpful struct.
struct point {
    GLfloat x;
    GLfloat y;
    GLfloat s;
    GLfloat t;
};
// The text vertex buffer object.
GLuint textVBO;



GLFWwindow * window;
int screenWidth = 0;
int screenHeight = 0;

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
    if (!glfwInit())
        shutdown(ERROR);

    int count;
    const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);
    for (int i = 0; i < count; i++)
    {
        if (modes[i].width > screenWidth && modes[i].height > screenHeight)
        {
            screenWidth = modes[i].width;
            screenHeight = modes[i].height;
        }
    }

    // Create window
    window = glfwCreateWindow(screenWidth, screenHeight, "LowD", glfwGetPrimaryMonitor(), nullptr);
    if (!window)
    {
        shutdown(ERROR);
    }

    glfwMakeContextCurrent(window);

    // Initialize mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    

    // Initialize GLEW
	glewInit();
	if (!glewIsSupported("GL_VERSION_2_0"))
		shutdown(ERROR);
    std::cout <<"OpenGL version supported by this platform: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Using GLEW version: " << glewGetString(GLEW_VERSION) << std::endl;
    printf("Using GLFW version: %i.%i.%i\n", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);


    // Initialize FreeType.
    std::cout << "Loading FreeType..." << std::endl;
    FT_Library ft;
     
    if (FT_Init_FreeType(&ft)) 
    {
        // Unable to initialize the FreeType library.
        std::cout << "Unable to load FreeType library" << std::endl;
		shutdown(ERROR);
    }

    // Load a font.
    if (FT_New_Face(ft, "fonts/FreeSans.ttf", 0, &face)) 
    {
        // Unable to load font.
        std::cout << "Unable to load font." << std::endl;
		shutdown(ERROR);
    }


    glViewport(0, 0, (GLsizei) screenWidth, (GLsizei) screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set the perspective (angle of sight, perspective, depth)
    gluPerspective(60, (GLfloat) screenWidth / (GLfloat) screenHeight, 0.1, 100.0); 
    glMatrixMode(GL_MODELVIEW);

    // Initialize debug info
    initDebugInfo();

    // VBO for text
	glGenBuffers(1, &textVBO);

    // Initialize world
    std::cout << "Initializing world..." << std::endl;
    chunkManager.initializeWorld();

    // Initialize player position
    std::cout << "Initializing player..." << std::endl;
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

    textShader.createShader("shaders/text.v.glsl", "shaders/text.f.glsl");
	attribute_coord = textShader.getAttrib("coord");
	uniform_tex = textShader.getUniform("tex");
	uniform_color = textShader.getUniform("color");

    // Check that the attributes are set.
	if(attribute_coord == -1 || uniform_tex == -1 || uniform_color == -1)
		std::cout << "Error setting shader attributes." << std::endl;
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
    double lastFPSUpdate = prevTime;
    double accumulator = 0.0;
    int numberOfFrames = 0;

    while (true)
    {
        // Calculate time since last frame
        double currentTime = glfwGetTime();
        double delta = currentTime - prevTime;
        numberOfFrames++;
        std::cout << "frame start " << currentTime << std::endl;
        // ms / frame output, averaged over 1 second.
        if (currentTime - lastFPSUpdate >= 1.0)
        {
            // 60 FPS = 16.666 ms
            // 30 FPS = 33.333 ms
            msPerFrame = 1000.0f / numberOfFrames;
            numberOfFrames = 0;
            lastFPSUpdate += 1;
        }


        prevTime = currentTime;

        if (delta > 0.25)
            delta = 0.25;

        accumulator += delta;


        // Clear screen to black
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        std::cout << "  accumulator " << glfwGetTime() << std::endl;
        while ( accumulator >= dt )
        {
            update(dt);
            accumulator -= dt;
        }

        std::cout << "  render start " << glfwGetTime() << std::endl;
        render();
        std::cout << "  render end " << glfwGetTime() << std::endl;


        // Swap back and front buffers
        std::cout << "  swap start " << glfwGetTime() << std::endl;
        glfwSwapBuffers(window);
        std::cout << "  swap end " << glfwGetTime() << std::endl;

        std::cout << "  events start " << glfwGetTime() << std::endl;
        glfwPollEvents();
        std::cout << "  events end " << glfwGetTime() << std::endl;

        std::cout << "frame end " << glfwGetTime() << std::endl;


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

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        offY -= 1.5;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        offY += 1.5;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        offX -= 1.5;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        shutdown(OK);

    // Left
    if (glfwGetKey(window, 'a') == GLFW_PRESS || glfwGetKey(window, 'A') == GLFW_PRESS)
    {
        xvel -= xproj;
        zvel -= zproj;
    }

    // Forward
    if (glfwGetKey(window, 'w') == GLFW_PRESS || glfwGetKey(window, 'W') == GLFW_PRESS)
    {
        xvel += zproj;
        zvel -= xproj;
    }

    // Back
    if (glfwGetKey(window, 's') == GLFW_PRESS || glfwGetKey(window, 'S') == GLFW_PRESS)
    {
        xvel -= zproj;
        zvel += xproj;
    }

    // Right
    if (glfwGetKey(window, 'd') == GLFW_PRESS || glfwGetKey(window, 'D') == GLFW_PRESS)
    {
        xvel += xproj;
        zvel += zproj;
    }

    // JUMP
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !player.isFalling())
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
        //position.y -= _gravity  * delta;
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

void render_text(const char *text, float x, float y, float sx, float sy) 
{
	const char *p;
	FT_GlyphSlot g = face->glyph;

	/* Create a texture that will be used to hold one "glyph" */
	GLuint tex;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(uniform_tex, 0);

	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* Set up the VBO for our vertex data */
	glEnableVertexAttribArray(attribute_coord);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);

	/* Loop through all characters */
	for (p = text; *p; p++) 
    {
		/* Try to load and render the character */
		if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;

		/* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		/* Calculate the vertex and texture coordinates */
		float x2 = x + g->bitmap_left * sx;
		float y2 = -y - g->bitmap_top * sy;
		float w = g->bitmap.width * sx;
		float h = g->bitmap.rows * sy;

		point box[4] = {
			{x2, -y2, 0, 0},
			{x2 + w, -y2, 1, 0},
			{x2, -y2 - h, 0, 1},
			{x2 + w, -y2 - h, 1, 1},
		};

		/* Draw the character on the screen */
		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		/* Advance the cursor to the start of the next character */
		x += (g->advance.x >> 6) * sx;
		y += (g->advance.y >> 6) * sy;
	}

	glDisableVertexAttribArray(attribute_coord);
	glDeleteTextures(1, &tex);
}

void enter2DMode()
{
    // Prep for 2D rendering.
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void enter3DMode()
{
    // Prep for 3D rendering.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set the perspective (angle of sight, perspective, depth)
    gluPerspective(60, (GLfloat) screenWidth / (GLfloat) screenHeight, 0.1, 100.0); 
    glMatrixMode(GL_MODELVIEW);
    // Prep for 3D drawing.
	glEnable(GL_DEPTH_TEST);
}

/**
 * Rendering function
 */
void render()
{
    enter3DMode();
    glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    // shader.begin();

    std::cout << "    chunk render start " << glfwGetTime() << std::endl;
    // Draw environment
    glPushMatrix();
    chunkManager.render();
    glPopMatrix();
    std::cout << "    chunk render end " << glfwGetTime() << std::endl;

   // shader.end();

    enter2DMode();

    // Scale for the text.
    float sx = 2.0 / screenWidth;
	float sy = 2.0 / screenHeight;

    textShader.begin();

	// Enable blending, necessary for our alpha texture.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // We will color the font white.
	GLfloat white[4] = { 1, 1, 1, 1 };

	// Set font size to, set the color.
    FT_Set_Pixel_Sizes(face, 0, 24);
	glUniform4fv(uniform_color, 1, white);

    std::cout << "    rendering text start." << std::endl;
    render_text(std::to_string(msPerFrame).c_str(), 0.1 * sx, 0.1 * sy, sx, sy);
    std::cout << "    rendering text end." << std::endl;

    textShader.end();
}
