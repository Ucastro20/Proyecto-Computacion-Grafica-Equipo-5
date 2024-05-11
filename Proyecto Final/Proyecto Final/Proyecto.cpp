
/*-------------------------------------------------------------*/
/* ----------------   Proyecto Final --------------------------*/
/*-----------------    Semestre 2024-2   ----------------------*/
/*-----------------      Equipo 5        ----------------------*/
/*------------- Alumnos: Castro Rodriguez Ulises --------------*/
/*-------------			 MÉNDEZ COSTALES LUIS ENRIQUE ---------*/
/*-------------------------------------------------------------*/

#include <Windows.h>

#include <irrKlang/irrKlang.h>

using namespace irrklang;

#pragma comment(lib, "irrKlang.lib")

#include <glad/glad.h>
#include <glfw3.h>						//main
#include <stdlib.h>		
#include <glm/glm.hpp>					//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>					//Texture

#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>
#include <mmsystem.h>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor* monitors;

GLuint VBO[3], VAO[3], EBO[3];

//Camera
Camera camera(glm::vec3(0.0f, 10.0f, 150.0f)); //Constructor con la posicion inicial de la camara
float MovementSpeed = 0.1f;
GLfloat lastX = SCR_WIDTH / 2.0f,
lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//Timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
lastFrame = 0.0f;

void getResolution(void);
void myData(void);							// De la practica 4
void LoadTextures(void);					// De la práctica 6
unsigned int generateTextures(char*, bool, bool);	// De la práctica 6

//Sound
	ISoundEngine* engine = createIrrKlangDevice(); 

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rotX = 0.0f,
rotBrazo = 0.0f;



//Lighting
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f); //Direccion de los rayos luminosos



//// Light
glm::vec3 lightColor = glm::vec3(0.7f);
glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
glm::vec3 ambientColor = diffuseColor * glm::vec3(0.5f);

// posiciones
float	movAuto_x = 0.0f,
movAuto_z = 0.0f,
orienta = 90.0f;
bool	animacion = false,
recorrido1 = true,
recorrido2 = false,
recorrido3 = false,
recorrido4 = false;



void saveFrame(void)
{

}

void resetElements(void)
{

}

void interpolation(void)
{


}

unsigned int generateTextures(const char* filename, bool alfa, bool isPrimitive)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;

	if (isPrimitive)
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	else
		stbi_set_flip_vertically_on_load(false); // tell stb_image.h to flip loaded texture's on the y-axis.


	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void LoadTextures()
{
}



void animate(void)
{
}

void getResolution() {
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData() {
}

int main() {

	if (!engine) 
		return 0; // error starting up the engine

	// play some sound stream, looped

	engine->play2D("InnerLight.mp3", true);

	// glfw: initialize and configure
	glfwInit();

	// glfw window creation
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto Equipo 5", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	//Mis funciones
	//Datos a utilizar
	LoadTextures();
	myData();
	glEnable(GL_DEPTH_TEST);



	// build and compile shaders
	// -------------------------
	Shader myShader("shaders/shader_texture_color.vs", "shaders/shader_texture_color.fs"); //To use with primitives
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");	//To use with static models
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");	//To use with skybox
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");	//To use with animated models 

	vector<std::string> faces{
		"resources/skybox/right.jpg",
			"resources/skybox/left.jpg",
			"resources/skybox/top.jpg",
			"resources/skybox/bottom.jpg",
			"resources/skybox/front.jpg",
			"resources/skybox/back.jpg"
	}; //Aqui ponemos las imagenes de nuestro skybox o donde pondremos todo

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// Carga de Modelos
	// -----------

	//  Estructura Principal
	Model Estructura("resources/objects/Estructura/Estructura.obj");
	Model Barandilla("resources/objects/Estructura/Barandilla.obj");
	Model Cascada("resources/objects/Estructura/Cascada.obj");
	Model Bancas("resources/objects/Estructura/Plantas_Bancas/Plantas_Bancas.obj");
	Model Arboles("resources/objects/Estructura/Plantas_Bancas/arboles.obj");
	Model Escaleras("resources/objects/Estructura/Escaleras/Escaleras.obj");


	//Locales 1er Piso
	Model Locales1er("resources/objects/Locales1er_piso/Locales1er.obj");
	Model Carrito("resources/objects/Locales1er_piso/Carrito/Carrito.obj");
		//Local de Ropa
		Model LocalRopa("resources/objects/Local_ropa/Local_ropa.obj");
		Model Maniquin("resources/objects/Local_ropa/Maniquin.obj");
		Model MesasRopa("resources/objects/Local_ropa/Mesas.obj");
		Model Registradora("resources/objects/Local_ropa/Registradora.obj");

	//Locales 2do Piso
	Model Locales2do("resources/objects/Locales2do_piso/Locales2do.obj");
	Model MesasComida("resources/objects/Mesas_comida_rapida/Mesas.obj");
		//Local de Comida Rapida
	Model LocalComida("resources/objects/Local_comida/Local_comida.obj");



	ModelAnim AnimSkate("resources/objects/Skate/Skateboarding.dae"); //La informacion se vacia en memoria
	AnimSkate.initShaders(animShader.ID);//Le ponemos un identificador para evitar problemas

	ModelAnim AnimDive("resources/objects/Dive/Dive.dae"); //La informacion se vacia en memoria
	AnimDive.initShaders(animShader.ID);

	ModelAnim AnimDance("resources/objects/Dance/Dance.dae"); //La informacion se vacia en memoria
	AnimDive.initShaders(animShader.ID);


	// create transformations and Projection
	glm::mat4 modelOp = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 viewOp = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projectionOp = glm::mat4(1.0f);	//This matrix is for Projection

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		//Setup shader for static models
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection); //Luz Direccional, actua como el sol
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.3f, 0.3f, 0.3f)); //Componente ambiental
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.7f, 0.7f, 0.7f)); //componente difusa
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f)); //Componente especular

		staticShader.setVec3("pointLight[0].position", glm::vec3(0.0f, 0.0f, 0.0f)); //Luz posicional/Puntual .position es para la position de la luz (x,y,z)
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.008f); //Modificamos la distancia 
		staticShader.setFloat("pointLight[0].linear", 0.009f);  //Control de intensidad menor, mas fino
		staticShader.setFloat("pointLight[0].quadratic", 0.00032f); //modificamos la intensidad


		staticShader.setVec3("spotLight[0].position", camera.Position); //la luz spotlight esta en la misma posicion q la camara
		staticShader.setVec3("spotLight[0].direction", camera.Front); //la luz spotlight apunta a la misma direccion q la camara
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f))); //Angulo de apertura
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(20.0f))); //Angulo de apertura externo
		staticShader.setFloat("spotLight[0].constant", 0.001f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.00005f);

		staticShader.setFloat("material_shininess", 32.0f);

		//glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		projectionOp = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		viewOp = camera.GetViewMatrix();
		staticShader.setMat4("projection", projectionOp);
		staticShader.setMat4("view", viewOp);

		//Setup shader for primitives
		myShader.use();
		// view/projection transformations
		//projectionOp = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 400.0f);
		viewOp = camera.GetViewMatrix();
		// pass them to the shaders
		//myShader.setMat4("model", modelOp);
		myShader.setMat4("view", viewOp);
		// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		myShader.setMat4("projection", projectionOp);
		/**********/


		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion // Descomentar esto
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use(); //Con esta linea activamos el shader
		animShader.setMat4("projection", projectionOp);
		animShader.setMat4("view", viewOp);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		// -------------------------------------------------------------------------------------------------------------------------
		// Animaciones
		// -------------------------------------------------------------------------------------------------------------------------
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.1f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.07f));
		animShader.setMat4("model", modelOp);
		AnimSkate.Draw(animShader);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 38.0f, -10.0f));
		modelOp = glm::rotate(modelOp, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.07f));
		animShader.setMat4("model", modelOp);
		AnimDive.Draw(animShader);


		//modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 0.1f, 50.0f));
		//modelOp = glm::scale(modelOp, glm::vec3(0.07f));
		//animShader.setMat4("model", modelOp);
		///*AnimDance.Draw(animShader);*/


		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario Primitivas
		// -------------------------------------------------------------------------------------------------------------------------
		myShader.use();

		

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario Estatico
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projectionOp);
		staticShader.setMat4("view", viewOp);

		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 
		modelOp = glm::scale(modelOp, glm::vec3(1.0f)); 
		staticShader.setMat4("model", modelOp); 
		Estructura.Draw(staticShader); 

		staticShader.setMat4("model", modelOp);
		Locales1er.Draw(staticShader);


		staticShader.setMat4("model", modelOp);
		Carrito.Draw(staticShader);

		staticShader.setMat4("model", modelOp);
		Bancas.Draw(staticShader);

		staticShader.setMat4("model", modelOp);
		Arboles.Draw(staticShader);

		staticShader.setMat4("model", modelOp);
		Barandilla.Draw(staticShader);

		staticShader.setMat4("model", modelOp);
		Cascada.Draw(staticShader);

		staticShader.setMat4("model", modelOp);
		Escaleras.Draw(staticShader);

		staticShader.setMat4("model", modelOp);
		LocalRopa.Draw(staticShader);

		staticShader.setMat4("model", modelOp);
		Maniquin.Draw(staticShader);

		staticShader.setMat4("model", modelOp);
		MesasRopa.Draw(staticShader);

		staticShader.setMat4("model", modelOp);
		Registradora.Draw(staticShader);

		staticShader.setMat4("model", modelOp);
		Locales2do.Draw(staticShader);

		staticShader.setMat4("model", modelOp);
		MesasComida.Draw(staticShader);
		
		staticShader.setMat4("model", modelOp);
		LocalComida.Draw(staticShader);

		

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, viewOp, projectionOp, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	//skybox.Terminate();
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		engine->drop(); 
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}
