#include "Window.h"
#include <glm/gtx/string_cast.hpp>

/* 
 * Declare your variables below. Unnamed namespace is used here to avoid 
 * declaring global or static variables.
 */
namespace
{
	int width, height;
	std::string windowTitle("GLFW Starter Project");

	// Boolean switches
	bool glRasterize = true;

	Cube* cube;
	PointCloud* cubePoints; 
	Object* currentObj;  // The object currently displaying.
    Object* dragon;
    Object* bunny;
    Object* bear;
    Object* light_sphere;
	RasterizerQuad* quad;  // Object textured with your rasterization results

	glm::vec3 eye(0, 0, 20); // Camera position.
	glm::vec3 center(0, 0, 0); // The point we are looking at.
	glm::vec3 up(0, 1, 0); // The up direction of the camera.
	float fovy = 60;
	float near = 1;
	float far = 1000;
	glm::mat4 view = glm::lookAt(eye, center, up); // View matrix, defined by eye, center and up.
	glm::mat4 projection; // Projection matrix.

    glm::vec3 z_rm(0,0,1); // z axis rotation matrix
    glm::vec3 y_rm(0,1,0); // y axis rotation matrix
    glm::vec3 t_bm(2,0,0); // bunny initial translation matrix
    glm::vec3 t_dm(-2,0,0); // dragon intial translation matrix

    int left_press = 0;
    int right_press = 0;
    double cur_x,cur_y;
    glm::vec3 ball_point(0,0,20);
    glm::vec3 lightPos = glm::vec3(1.8f,0,1);
    glm::vec3 pointLightPos = glm::vec3(-2.0f,0,1);
    int change_light = 0;
    int dirLight_switch = 1;
    int object_switch = 0;
    int key_press = 1;
    int object_pointer = 1;
    int start = 0;

	GLuint program; // The shader program id.
	GLuint projectionLoc; // Location of projection in shader.
	GLuint viewLoc; // Location of view in shader.
	GLuint modelLoc; // Location of model in shader.
	GLuint colorLoc; // Location of color in shader.
    GLuint lightswitch;
    GLuint light_check;
    GLuint light_position;
    GLuint material_ambient_Loc;
    GLuint material_diffuse_Loc;
    GLuint material_specular_Loc;
    GLuint shininess_Loc;
    GLuint light_ambient_Loc;
    GLuint light_diffuse_Loc;
    GLuint light_specular_Loc;
    GLuint pointlight_position;
    GLuint pointlight_constant;
    GLuint pointlight_linear;
    GLuint pointlight_quadratic;
    GLuint pointlight_ambient_Loc;
    GLuint pointlight_diffuse_Loc;
    GLuint pointlight_specular_Loc;

	GLuint programQuad;
};

bool Window::initializeProgram()
{
	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
	// This shader program is for displaying your rasterizer results
	// DO NOT MODIFY THESE SHADER PROGRAMS
	programQuad = LoadShaders("shaders/RasterizerQuad.vert", "shaders/RasterizerQuad.frag");

	// Check the shader programs.
	if (!program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}
	if (!programQuad)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	// Activate the shader program.
	glUseProgram(program);
	// Get the locations of uniform variables.
	projectionLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");
	colorLoc = glGetUniformLocation(program, "color");
    lightswitch = glGetUniformLocation(program, "change_light");
    light_check = glGetUniformLocation(program, "is_light");
    
    // Shader part for material
    material_ambient_Loc = glGetUniformLocation(program, "material.ambient");
    material_diffuse_Loc = glGetUniformLocation(program, "material.diffuse");
    material_specular_Loc = glGetUniformLocation(program, "material.specular");
    shininess_Loc = glGetUniformLocation(program,"material.shininess");
    
    // Shader part for dirlight
    light_position = glGetUniformLocation(program, "dirLight.position");
    light_ambient_Loc = glGetUniformLocation(program, "dirLight.ambient");
    light_diffuse_Loc = glGetUniformLocation(program, "dirLight.diffuse");
    light_specular_Loc = glGetUniformLocation(program, "dirLight.specular");
    
    // Shader part for pointlight
    pointlight_position = glGetUniformLocation(program, "pointlight.position");
    pointlight_linear = glGetUniformLocation(program, "pointlight.linear");
    pointlight_constant = glGetUniformLocation(program, "pointlight.constant");
    pointlight_quadratic = glGetUniformLocation(program, "pointlight.quadratic");
    pointlight_ambient_Loc = glGetUniformLocation(program, "pointlight.ambient");
    pointlight_diffuse_Loc = glGetUniformLocation(program, "pointlight.diffuse");
    pointlight_specular_Loc = glGetUniformLocation(program, "pointlight.specular");
    

	return true;
}

bool Window::initializeObjects()
{
	// Create a cube of size 5.
	//cube = new Cube(5.0f);
	// Create a point cloud consisting of cube vertices.
	/*cubePoints = new PointCloud("/Users/yilincai/CSE167/hw0_example/bunny.obj", 5, glm::vec3(1.0f,0.0f,0.0f));
    cubePoints->saveOrigin();
    bunny = cubePoints;
	currentObj = cubePoints;
    
    cubePoints = new PointCloud("/Users/yilincai/CSE167/hw0_example/dragon.obj", 5, glm::vec3(0,0.7f,0));
    cubePoints->saveOrigin();
    dragon = cubePoints;
    
    cubePoints = new PointCloud("/Users/yilincai/CSE167/hw0_example/bear.obj", 5,glm::vec3(0.7f,0.4f,0.2f));
    cubePoints->saveOrigin();
    bear = cubePoints;
    
    light_sphere = new PointCloud("/Users/yilincai/CSE167/hw0_example/sphere.obj",5, glm::vec3(1,1,1));
    light_sphere->saveOrigin();*/

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete cube;
	delete cubePoints;
	delete quad;

	// Delete the shader programs.
	glDeleteProgram(program);
	glDeleteProgram(programQuad);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);
	
	// Initialize the quad that will be textured with your image
	// The quad must be made with the window
	//quad = new RasterizerQuad(width, height);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int w, int h)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height);
    cur_x = w;
    cur_y = h;
    w = w * 2;
    h = h * 2;
#endif
	width = w;
	height = h;
	// Resize our CPU rasterizer's pixel buffer and zbuffer
	//quad->updateBufSiz(width, height);

	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	projection = glm::perspective(glm::radians(fovy),
		(float)width / (float)height, near, far);
}

void Window::idleCallback()
{
	// Perform any updates as necessary.
	currentObj->update();
    
    //dragon->update();
}

void Window::displayCallback(GLFWwindow* window)
{
	// Switch between OpenGL rasterizer and your rasterizer
	//if (glRasterize) {
		// Switch back to using OpenGL's rasterizer
		glUseProgram(program);
		// Clear the color and depth buffers.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Specify the values of the uniform variables we are going to use.
		/*
		 * TODO: Section 3 and 4: Modify the code here to draw both the bunny and
		 * the dragon
		 * Note that the model matrix sent to the shader belongs only
		 * to what object the currentObj ptr is pointing to. You will need to
		 * use another call to glUniformMatrix4fv to change the model matrix
		 * data being sent to the vertex shader before you draw the other object
		 */
		/*glm::mat4 model = currentObj->getModel();
		glm::vec3 color = currentObj->getColor();
        model = glm::scale(model,currentObj->getScaleTrnasformation());
        model = glm::translate(model,currentObj->getCenterTransformation());
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1f(lightswitch, change_light);
        glUniform1f(light_check,0);
		glUniform3fv(colorLoc, 1, glm::value_ptr(color));
        //Bunny material
        if(object_pointer==1){
            glUniform3fv(material_ambient_Loc,1,glm::value_ptr(glm::vec3(0.2f,0.2f,0.2f)));
            glUniform3fv(material_diffuse_Loc,1,glm::value_ptr(glm::vec3(0.3f,0.0f,0.0f)));
            glUniform3fv(material_specular_Loc,1,glm::value_ptr(glm::vec3(1.0f,1.0f,1.0f)));
            glUniform1f(shininess_Loc,32.0f);
        }
        //Dragon material
        else if(object_pointer==2){
            glUniform3fv(material_ambient_Loc,1,glm::value_ptr(glm::vec3(0.2f,0.2f,0.2f)));
            glUniform3fv(material_diffuse_Loc,1,glm::value_ptr(glm::vec3(0.9f,0.9f,0.9f)));
            glUniform3fv(material_specular_Loc,1,glm::value_ptr(glm::vec3(0.1f,0.1f,0.1f)));
            glUniform1f(shininess_Loc,64.0f);
        }
        //Bear material
        else{
            glUniform3fv(material_ambient_Loc,1,glm::value_ptr(glm::vec3(0.2f,0.2f,0.2f)));
            glUniform3fv(material_diffuse_Loc,1,glm::value_ptr(glm::vec3(0.5f,0.5f,0.5f)));
            glUniform3fv(material_specular_Loc,1,glm::value_ptr(glm::vec3(0.8f,0.8f,0.8f)));
            glUniform1f(shininess_Loc,32.0f);
        }
        
        if(dirLight_switch==1){
            glUniform3fv(light_ambient_Loc,1,glm::value_ptr(glm::vec3(1.0f,1.0f,1.0f)));
            glUniform3fv(light_diffuse_Loc,1,glm::value_ptr(glm::vec3(1.0f,1.0f,1.0f)));
            glUniform3fv(light_specular_Loc,1,glm::value_ptr(glm::vec3(1.0f,1.0f,1.0f)));
            glUniform3fv(light_position, 1, glm::value_ptr(lightPos));
        }
        else{
            glUniform3fv(light_ambient_Loc,1,glm::value_ptr(glm::vec3(0.5,0.5,0.5)));
            glUniform3fv(light_diffuse_Loc,1,glm::value_ptr(glm::vec3(0,0,0)));
            glUniform3fv(light_specular_Loc,1,glm::value_ptr(glm::vec3(0,0,0)));
        }
        glUniform1f(pointlight_constant,1.0f);
        glUniform1f(pointlight_linear,0.09f);
        glUniform1f(pointlight_quadratic, 0.032f);
        glUniform3fv(pointlight_position, 1, glm::value_ptr(pointLightPos));
    
        if(key_press == 2){
            glUniform3fv(pointlight_ambient_Loc,1,glm::value_ptr(glm::vec3(1,1,1)));
            glUniform3fv(pointlight_diffuse_Loc, 1, glm::value_ptr(glm::vec3(1,1,1)));
            glUniform3fv(pointlight_specular_Loc, 1, glm::value_ptr(glm::vec3(1,1,1)));
        }
        else{
            glUniform3fv(pointlight_ambient_Loc,1,glm::value_ptr(glm::vec3(0.5,0.5,0.5)));
            glUniform3fv(pointlight_diffuse_Loc, 1, glm::value_ptr(glm::vec3(0,0,0)));
            glUniform3fv(pointlight_specular_Loc, 1, glm::value_ptr(glm::vec3(0,0,0)));
        }
		// Render the object.
		currentObj->draw();
        float r = -0.0087;
        lightPos[0] = lightPos[0]*cos(r)-lightPos[2]*sin(r);
        lightPos[2] = lightPos[0]*sin(r)+lightPos[2]*cos(r);
    
        if(change_light==1&&key_press==2){
            glm::vec3 color2 = light_sphere->getColor();
            glm::mat4 model2 = light_sphere ->getModel();
            if(start==0){
                start = 1;
                model2 = glm::translate(model2,pointLightPos);
                model2 = glm::scale(model2,glm::vec3(0.2f));
                light_sphere->setModel(model2);
            }
            glUniform1f(light_check,1);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
            glUniform3fv(colorLoc, 1, glm::value_ptr(color2));
            light_sphere->draw();
        }*/

		// Gets events, including input such as keyboard and mouse or window resizing.
		glfwPollEvents();
		// Swap buffers.
		glfwSwapBuffers(window);
	//}
	/*else {
		// Uncomment when you want to see your rasterizer results
		glUseProgram(programQuad);
		
		quad->clearBuffers();
        
        glm::mat4 M = dragon->getModel();
        glm::mat4 V = view;
        glm::mat4 P = projection;
        glm::mat4 D = glm::mat4(0.0f);
        D[0][0] = width/2;
        D[1][1] = height/2;
        D[2][2] = 0.5;
        D[3][0] = width/2;
        D[3][1] = height/2;
        D[3][2] = 0.5;
        D[3][3] = 1;
		// CPU based rasterization. Fills in the pixel buffer and displays result. 
		// Replace cubePoints with the PointCloud of your choice
		quad->rasterize(M, V, P, D, *cubePoints);

		glfwPollEvents();
		glfwSwapBuffers(window);
		
	}*/
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Section 4: Modify below to add your key callbacks.
	 */

	 // Check for a key press.
	if (action == GLFW_PRESS)
	{
		// Uppercase key presses (shift held down + key press)
		if (mods == GLFW_MOD_SHIFT) {
			switch (key) {
                /*case GLFW_KEY_P:
                    bunny->increase_point_size();
                    dragon->increase_point_size();
                break;*/
                case GLFW_KEY_Z:
                    eye[2] = eye[2]+1;
                    view = glm::lookAt(eye, center, up);
                break;
                case GLFW_KEY_S:
                    currentObj->scale_up();
                    break;
                /*case GLFW_KEY_R:
                    bunny->reset_shape();
                    dragon->reset_shape();*/
			default:
				break;
			}
		}
        else{
		// Deals with lowercase key presses
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_F1:
			// Set currentObj to cube
			currentObj = bunny;
                object_pointer = 1;
			break;
		case GLFW_KEY_F2:
			// Set currentObj to cubePoints
			currentObj = dragon;
                object_pointer = 2;
			break;
        case GLFW_KEY_F3:
            currentObj = bear;
                object_pointer = 3;
            break;
		/*case GLFW_KEY_M:
			if (glRasterize)
				std::cout << "Switching to CPU rasterizer\n";
			else
				std::cout << "Switching to OpenGL rasterizer\n";
			glRasterize = !glRasterize;
			break;
        case GLFW_KEY_W:
            bunny->wup(z_rm);
            dragon->wup(y_rm);
            break;
        case GLFW_KEY_S:
            bunny->sdown(z_rm);
            dragon->sdown(y_rm);
            break;
        case GLFW_KEY_D:
            bunny->dright(z_rm);
            dragon->dright(y_rm);
            break;
        case GLFW_KEY_A:
            bunny->aleft(z_rm);
            dragon->aleft(y_rm);
            break;
        case GLFW_KEY_R:
            bunny->reset_position(z_rm,t_bm);
            dragon->reset_position(y_rm,t_dm);
            break;
        case GLFW_KEY_P:
            bunny->decrease_point_size();
            dragon->decrease_point_size();
            break;*/
        case GLFW_KEY_Z:
            eye[2] = eye[2]-1;
            view = glm::lookAt(eye, center, up);
            break;
        case GLFW_KEY_S:
                currentObj->scale_down();
                break;
        case GLFW_KEY_R:
                currentObj->reset_whole();
                break;
            case GLFW_KEY_N:
                if(change_light==0){
                    change_light=1;
                }
                else{
                    change_light = 0;
                }
                break;
            case GLFW_KEY_3:
                if(dirLight_switch==0){
                    printf("dirlight on\n");
                    dirLight_switch = 1;
                }
                else{
                    printf("dirlight off\n");
                    dirLight_switch = 0;
                }
                break;
            case GLFW_KEY_0:
                if(object_switch==0){
                    printf("switch to light sphere\n");
                    object_switch = 1;
                }
                else{
                    printf("switch to object\n");
                    object_switch = 0;
                }
                break;
            case GLFW_KEY_1:
                printf("Dirlight only\n");
                key_press = 1;
                break;
            case GLFW_KEY_2:
                printf("Dirlight+pointlight\n");
                key_press = 2;
                break;
		default:
			break;
		}
        }
	}
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
    if(action==GLFW_PRESS){
        switch(button){
            case GLFW_MOUSE_BUTTON_LEFT:{
                left_press = 1;
                glfwGetCursorPos(window, &cur_x, &cur_y);
                ball_point = trackBallMapping(glm::vec3(cur_x,cur_y,0));
                break;}
            case GLFW_MOUSE_BUTTON_RIGHT:
                right_press = 1;
                glfwGetCursorPos(window, &cur_x, &cur_y);
                break;
            default:
                break;
        }
    }
    else{
        left_press=0;
        right_press=0;
    }
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    glfwGetCursorPos(window, &xpos, &ypos);
    if(object_switch==0){
        
    if(right_press==1){
        double x_dif,y_dif;
        x_dif = xpos-cur_x;
        y_dif = ypos-cur_y;
        cur_x = xpos;
        cur_y = ypos;
        currentObj->xyTranslate(x_dif, y_dif);
    }
    if(left_press==1){
        glm::vec3 coor = glm::vec3(xpos,ypos,0);
        glm::vec3 result = trackBallMapping(coor);
        glm::vec3 point_dif = result-ball_point;
        glm::vec3 cross_product = glm::cross(result,ball_point);
        ball_point = result;
        float velocity = glm::length(point_dif);
        if(velocity>0.0001){
            velocity = velocity*1000;
            cross_product[0] = -cross_product[0];
            cross_product[1] = -cross_product[1];
            cross_product[2] = -cross_product[2];
            currentObj->xyzRotate(velocity, cross_product);
        }
    }
        
    }
    else{
        if(left_press==1){
            glm::vec3 coor = glm::vec3(xpos,ypos,0);
            glm::vec3 result = trackBallMapping(coor);
            glm::vec3 point_dif = result-ball_point;
            glm::vec3 cross_product = glm::cross(result,ball_point);
            ball_point = result;
            float velocity = glm::length(point_dif);
            if(velocity>0.0001){
                velocity = velocity*1000;
                cross_product[0] = -cross_product[0];
                cross_product[1] = -cross_product[1];
                cross_product[2] = -cross_product[2];
                light_sphere->lightRotate(velocity, cross_product);
                glm::mat4 model_m = light_sphere->getModel();
                pointLightPos[0] = model_m[3][0];
                pointLightPos[1] = model_m[3][1];
                pointLightPos[2] = model_m[3][2];
            }
        }
    }
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    currentObj->zTranslate(yoffset);
}

glm::vec3 Window::trackBallMapping(glm::vec3 point){
    glm::vec3 v;
    float d;
    v[0]=(2.0f*point[0]-width/2) / width/2;
    v[1]=(height/2-2.0f*point[1]) / height/2;
    v[2]=0;
    d = glm::length(v);
    d = (d<1.0f) ? d: 1.0f;
    v[2]=sqrtf(1.001 - d*d);
    v = normalize(v);
    return v;
}
