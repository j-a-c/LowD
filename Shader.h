#ifndef SHADER_H
#define SHADER_H

/**
 * Class to create a use a GLSL shader.
 */
class Shader
{

    public:
        Shader();
        ~Shader();

        void createShader(char * vertexFile, char * fragmentFile);

        void begin();
        void end();

    private:
        GLuint program;
        GLuint vertexShader;
        GLuint fragmentShader;

        char * readFile(char *file);
};

Shader::Shader()
{

}

Shader::~Shader()
{

}

void Shader::createShader(char * vertexFile, char * fragmentFile)
{
	char *vs = NULL,*fs = NULL;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	vs = readFile(vertexFile);
	fs = readFile(fragmentFile);

	const char * ff = fs;
	const char * vv = vs;

	glShaderSource(vertexShader, 1, &vv, NULL);
	glShaderSource(fragmentShader, 1, &ff, NULL);

	free(vs);
    free(fs);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	program = glCreateProgram();
	glAttachShader(program, fragmentShader);
	glAttachShader(program, vertexShader);

	glLinkProgram(program);
}

void Shader::begin()
{
	glUseProgram(program);
}

void Shader::end()
{
	glUseProgram(0);
}

char * Shader::readFile(char *fn)
{
	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");

		if (fp != NULL) {
      
      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

#endif
