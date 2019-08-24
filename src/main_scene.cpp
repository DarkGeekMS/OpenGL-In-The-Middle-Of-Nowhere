#include <application.hpp>
#include "main_scene.hpp"

#include <mesh/mesh_utils.hpp>
#include <textures/texture_utils.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*
The main game scene all models, textures and light sources should be loaded here :)
*/

void MainScene::Initialize() {
	keyExists = true;

	//Defining the Shaders for the Scene
	shader = new Shader();
	shader->attach("assets/shaders/General.vert", GL_VERTEX_SHADER);
	shader->attach("assets/shaders/General.frag", GL_FRAGMENT_SHADER);
	shader->link();

	skyShader = new Shader();
	skyShader->attach("assets/shaders/sky.vert", GL_VERTEX_SHADER);
	skyShader->attach("assets/shaders/sky.frag", GL_FRAGMENT_SHADER);
	skyShader->link();

	grainEffectShader = new Shader();
	grainEffectShader->attach("assets/shaders/grain_effect.vert", GL_VERTEX_SHADER);
	grainEffectShader->attach("assets/shaders/grain_effect.frag", GL_FRAGMENT_SHADER);
	grainEffectShader->link();

	depthShader = new Shader();
	depthShader->attach("assets/shaders/DepthShader.vert", GL_VERTEX_SHADER);
	depthShader->attach("assets/shaders/DepthShader.geom", GL_GEOMETRY_SHADER);
	depthShader->attach("assets/shaders/DepthShader.frag", GL_FRAGMENT_SHADER);
	depthShader->link();

	//Defining the Uniform Variables for Grain Effect Shader
	timeLoc = glGetUniformLocation(grainEffectShader->getID(), "time");
	grainLoc = glGetUniformLocation(grainEffectShader->getID(), "grainIntensity");

	moonPos = glm::vec3(300, 300, 150);
	moonRadius = 30;
	
	//Model Loading as .Obj
	ground = MeshUtils::LoadObj("assets/models/Ground/ground.obj");
	house = MeshUtils::LoadObj("assets/models/House/Old_house.obj");
	key = MeshUtils::LoadObj("assets/models/Key/key.obj");
	hands = MeshUtils::LoadObj("assets/models/Hands/Hands.obj");
	tree = MeshUtils::LoadObj("assets/models/Tree/tree.obj");
	sky = MeshUtils::Box();
	quad = new Mesh();
	quad->setup<Vertex>({
		{ { -1, -1,  0 },{ 1, 1, 1, 1 },{ 0, 0 },{ 0,0,1 } },
		{ { 1, -1,  0 },{ 1, 1, 1, 1 },{ 1, 0 },{ 0,0,1 } },
		{ { 1,  1,  0 },{ 1, 1, 1, 1 },{ 1, 1 },{ 0,0,1 } },
		{ { -1,  1,  0 },{ 1, 1, 1, 1 },{ 0, 1 },{ 0,0,1 } },
		}, {
			0, 1, 2, 2, 3, 0
		});
	
	//Texture Loading (Albedo, Specular, Ambient Occlusion)
	groundTex[ALBEDO] = TextureUtils::Load2DTextureFromFile("assets/textures/mech_v_lese_tex_DiffuseMap.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	groundTex[SPECULAR] = TextureUtils::Load2DTextureFromFile("assets/textures/mech_v_lese_tex_SPEC.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	groundTex[AMBIENT_OCCLUSION] = TextureUtils::Load2DTextureFromFile("assets/textures/mech_v_lese_10k_ao.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);

	houseTex[ALBEDO] = TextureUtils::Load2DTextureFromFile("assets/textures/Old_house_d.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	houseTex[SPECULAR] = TextureUtils::Load2DTextureFromFile("assets/textures/Old_house_d.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	houseTex[AMBIENT_OCCLUSION] = TextureUtils::Load2DTextureFromFile("assets/textures/Old_house_ao.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);

	keyTex[ALBEDO] = TextureUtils::Load2DTextureFromFile("assets/textures/KeyRust_A.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	keyTex[SPECULAR] = TextureUtils::Load2DTextureFromFile("assets/textures/KeyRust_M.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	keyTex[AMBIENT_OCCLUSION] = TextureUtils::Load2DTextureFromFile("assets/textures/KeyRust_AO.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);

	handsTex[ALBEDO] = TextureUtils::Load2DTextureFromFile("assets/textures/skin_texture.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	handsTex[SPECULAR] = TextureUtils::Load2DTextureFromFile("assets/textures/skin_texture.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	handsTex[AMBIENT_OCCLUSION] = TextureUtils::Load2DTextureFromFile("assets/textures/skin_texture_ao.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);

	treeTex[ALBEDO] = TextureUtils::Load2DTextureFromFile("assets/textures/Trunc__1.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	treeTex[SPECULAR] = TextureUtils::Load2DTextureFromFile("assets/textures/Trunc__1.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	treeTex[AMBIENT_OCCLUSION] = TextureUtils::Load2DTextureFromFile("assets/textures/Trunc__1__ao.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);

	//Cube Texture Loading for the Sky
	const char* files[6] = {
		"assets/textures/Night_Sky/nightsky_ft.tga",
		"assets/textures/Night_Sky/nightsky_bk.tga",
		"assets/textures/Night_Sky/nightsky_up.tga",
		"assets/textures/Night_Sky/nightsky_dn.tga",
		"assets/textures/Night_Sky/nightsky_rt.tga",
		"assets/textures/Night_Sky/nightsky_lf.tga "
	};

	env = TextureUtils::LoadCubTextureFromFiles(files);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	
	//Initializing Camera and Fly Camera Controller
	camera = new Camera();
	glm::ivec2 windowSize = getApplication()->getWindowSize();
	camera->setupPerspective(glm::pi<float>() / 2, (float)windowSize.x / windowSize.y, 0.1f, 1000.0f);
	camera->setUp({ 0, 1, 0 });
	camera->setPosition({ 0, 20, 0 });

	controller = new FlyCameraController(this, camera);
	controller->setYaw(0);
	controller->setPitch(0);
	controller->setPosition({ 0, 20, 0 });

	//Calculating the Transformations for the Shadow
	lightFarPlane = 1000.f;
	shadowProj = glm::perspective(glm::pi<float>() / 2, 1.0f, moonRadius + 0.1f, lightFarPlane);
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(moonPos, moonPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(moonPos, moonPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(moonPos, moonPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(moonPos, moonPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(moonPos, moonPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(moonPos, moonPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
	
	//FrameBuffers for Grain Effect and Depth Map
	fbo = new FrameBuffer();

	unsigned int width = getApplication()->getWindowSize().x;
	unsigned int height = getApplication()->getWindowSize().y;

	fboTex = new Texture2D();
	fboTex->bind();
	fboTex->setup(GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	fbo->attachTex2D(fboTex, GL_COLOR_ATTACHMENT0);
	
	fboDepthTex = new Texture2D();
	fboDepthTex->bind();
	fboDepthTex->setup(GL_DEPTH24_STENCIL8, width, height, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
	fbo->attachTex2D(fboDepthTex, GL_DEPTH_STENCIL_ATTACHMENT);

	depthMapFBO = new FrameBuffer();

	depthCubeMap = new TextureCube();
	depthCubeMap->setup(GL_DEPTH_COMPONENT, shadow_res, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	depthMapFBO->bind();
	depthMapFBO->attachTexCube(depthCubeMap, GL_DEPTH_ATTACHMENT);
	glDrawBuffer(GL_NONE);	
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	useGrainEffect = true;
	useShadows = true;

	//Culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Environment Color
	glClearColor(0.008f, 0.0f, 0.1255f, 0.0f);

}

void MainScene::Update(double delta_time) {
	controller->update(delta_time);  //Controller Update

	//Checking for the Pressed Keys
	Keyboard* Kb = this->getKeyboard();  
	//Take the Key
	if (Kb->isPressed(GLFW_KEY_F))
	{
		float distance = glm::distance(controller->getPosition(), { 250, 15, 150 });
		if (distance < 10)
		{
			keyExists = false;
		}
	}
	//Toggle Grain Effect
	if (Kb->justPressed(GLFW_KEY_G)) useGrainEffect = !useGrainEffect;
	//Toggle Shadows
	if (Kb->justPressed(GLFW_KEY_H)) useShadows = !useShadows;
}

void MainScene::Draw() {
	//Getting Window Dimensions
	unsigned int width = getApplication()->getWindowSize().x;
	unsigned int height = getApplication()->getWindowSize().y;
	
	//Getting Camera Information
	glm::mat4 VP = camera->getVPMatrix();
	glm::vec3 cam_pos = camera->getPosition();

	//Binding the Depth FrameBuffer to Create th depth Map for Shadows
	glViewport(0, 0, shadow_res, shadow_res);
	depthMapFBO->bind();
	glClear(GL_DEPTH_BUFFER_BIT);

	//Passing the Shadow Transformation Matrices to the Depth Shader
	depthShader->use();
	for (unsigned int i = 0; i < 6; ++i)
	{
		std::string s = "shadowMatrices[" + std::to_string(i) + "]";
		depthShader->set(s, shadowTransforms[i]);
	}

	depthShader->set("far_plane", lightFarPlane);
	depthShader->set("lightPos", moonPos);

	//Drawing The Scene for the Depth Shader to Calculate the Depth Map
	glm::mat4 ground_mat = glm::translate(glm::mat4(), { 0, 0, 0 }) *
		glm::rotate(glm::mat4(), glm::half_pi<float>(), { -1,0,0 }) *
		glm::scale(glm::mat4(), glm::vec3(500, 500, 1));
	depthShader->set("model", ground_mat);
	ground->draw();

	glm::mat4 house_mat1 = glm::translate(glm::mat4(), { 150, 8, -150 })*
		glm::rotate(glm::mat4(), glm::pi<float>() / 3.0f, { 0, -1, 0 })*
		glm::scale(glm::mat4(), glm::vec3(15, 15, 15));
	depthShader->set("model", house_mat1);
	house->draw();

	glm::mat4 house_mat2 = glm::translate(glm::mat4(), { -150, 8, 200 })*
		glm::rotate(glm::mat4(), glm::pi<float>() / 2.0f, { 0, 1, 0 })*
		glm::scale(glm::mat4(), glm::vec3(15, 15, 15));
	depthShader->set("model", house_mat2);
	house->draw();

	glm::mat4 house_mat3 = glm::translate(glm::mat4(), { 250, 8, 150 })*
		glm::rotate(glm::mat4(), glm::pi<float>(), { 0, -1, 0 })*
		glm::scale(glm::mat4(), glm::vec3(15, 15, 15));
	depthShader->set("model", house_mat3);
	house->draw();

	if (keyExists)
	{
		glm::mat4 key_mat = glm::translate(glm::mat4(), { 250, 15, 150 })*
			glm::rotate(glm::mat4(), glm::half_pi<float>(), { 0, 0, -1 })*
			glm::scale(glm::mat4(), glm::vec3(100, 100, 100));
		depthShader->set("model", key_mat);
		key->draw();
	}

	glm::mat4 tree_mat1 = glm::translate(glm::mat4(), { -250, 3, 250 })*
		glm::scale(glm::mat4(), glm::vec3(50, 50, 50));
	depthShader->set("model", tree_mat1);
	tree->draw();

	glm::mat4 tree_mat2 = glm::translate(glm::mat4(), { 250, 3, -200 })*
		glm::scale(glm::mat4(), glm::vec3(50, 50, 50));
	depthShader->set("model", tree_mat2);
	tree->draw();

	glm::mat4 tree_mat3 = glm::translate(glm::mat4(), { 200, 3, 250 })*
		glm::scale(glm::mat4(), glm::vec3(50, 50, 50));
	depthShader->set("model", tree_mat3);
	tree->draw();

	glm::mat4 tree_mat4 = glm::translate(glm::mat4(), { -200, 3, 100 })*
		glm::scale(glm::mat4(), glm::vec3(50, 50, 50));
	depthShader->set("model", tree_mat4);
	tree->draw();

	glm::mat4 tree_mat5 = glm::translate(glm::mat4(), { -200, 3, -150 })*
		glm::scale(glm::mat4(), glm::vec3(50, 50, 50));
	depthShader->set("model", tree_mat5);
	tree->draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Bind ing the Grain Effect FrameBuffer
	if (useGrainEffect) fbo->bind();

	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Using the Main Shader and Passing the Uniform Variables to it
	shader->use();

	shader->set("VP", VP);
	shader->set("cam_pos", cam_pos);
	shader->set("far_plane", lightFarPlane);
	shader->set("shadows", useShadows);

	shader->set("light.diffuse", { 60.0f, 55.0f, 50.0f });
	shader->set("light.specular", { 60.0f, 55.0f, 50.0f });
	shader->set("light.position", { 300, 260, 150 });
	shader->set("light.attenuation_constant", 0.1f);
	shader->set("light.attenuation_linear", 1.0f);
	shader->set("light.attenuation_quadratic", 0.0f);

	shader->set("ambient", 1.2f);

	shader->set("material.albedo", 0);
	shader->set("material.specular", 1);
	shader->set("material.ambient_occlusion", 2);

	shader->set("material.albedo_tint", { 1,1,1 });
	shader->set("material.specular_tint", { 1,1,1 });

	shader->set("material.shininess", 0.5f);

	//Passing the Cube Depth Map for the Shader to calculate the Shadows
	glActiveTexture(GL_TEXTURE3);
	depthCubeMap->bind();
	shader->set("shadowMap", 3);

	//Drawing the Scene Once more for Display and Passing the Textures to it
	shader->set("M", ground_mat);
	shader->set("M_it", glm::transpose(glm::inverse(ground_mat)));
	for (int i = 0; i < 3; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		groundTex[i]->bind();
	}
	ground->draw();

	shader->set("M", house_mat1);
	shader->set("M_it", glm::transpose(glm::inverse(house_mat1)));
	for (int i = 0; i < 3; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		houseTex[i]->bind();
	}
	house->draw();

	shader->set("M", house_mat2);
	shader->set("M_it", glm::transpose(glm::inverse(house_mat2)));
	for (int i = 0; i < 3; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		houseTex[i]->bind();
	}
	house->draw();

	shader->set("M", house_mat3);
	shader->set("M_it", glm::transpose(glm::inverse(house_mat3)));
	for (int i = 0; i < 3; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		houseTex[i]->bind();
	}
	house->draw();
	
	if (keyExists)
	{
		glm::mat4 key_mat = glm::translate(glm::mat4(), { 250, 15, 150 })*
			glm::rotate(glm::mat4(), glm::half_pi<float>(), { 0, 0, -1 })*
			glm::scale(glm::mat4(), glm::vec3(100, 100, 100));
		shader->set("M", key_mat);
		shader->set("M_it", glm::transpose(glm::inverse(key_mat)));
		for (int i = 0; i < 3; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			keyTex[i]->bind();
		}
		key->draw();
	}

	shader->set("M", tree_mat1);
	shader->set("M_it", glm::transpose(glm::inverse(tree_mat1)));
	for (int i = 0; i < 3; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		treeTex[i]->bind();
	}
	tree->draw();

	shader->set("M", tree_mat2);
	shader->set("M_it", glm::transpose(glm::inverse(tree_mat2)));
	for (int i = 0; i < 3; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		treeTex[i]->bind();
	}
	tree->draw();

	shader->set("M", tree_mat3);
	shader->set("M_it", glm::transpose(glm::inverse(tree_mat3)));
	for (int i = 0; i < 3; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		treeTex[i]->bind();
	}
	tree->draw();


	shader->set("M", tree_mat4);
	shader->set("M_it", glm::transpose(glm::inverse(tree_mat4)));
	for (int i = 0; i < 3; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		treeTex[i]->bind();
	}
	tree->draw();

	shader->set("M", tree_mat5);
	shader->set("M_it", glm::transpose(glm::inverse(tree_mat5)));
	for (int i = 0; i < 3; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		treeTex[i]->bind();
	}
	tree->draw();

	glm::mat4 base = glm::translate(glm::mat4(), controller->getPosition()) *
		glm::rotate(glm::mat4(), -controller->getYaw(), { 0 , 1 , 0 });
	glm::mat4 hand_mat = base * glm::translate(glm::mat4(), { 17, -5, 0 }) * glm::scale(glm::mat4(), glm::vec3(5, 5, 5));
	shader->set("M", hand_mat);
	shader->set("M_it", glm::transpose(glm::inverse(hand_mat)));
	for (int i = 0; i < 3; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		handsTex[i]->bind();
	}
	hands->draw();

	//Using the Sky Shader to Draw the Sky Texture
	glActiveTexture(GL_TEXTURE0);
	env->bind();

	skyShader->use();
	glUniformMatrix4fv(glGetUniformLocation(skyShader->getID(), "VP"), 1, GL_FALSE, glm::value_ptr(VP));
	glUniform3f(glGetUniformLocation(skyShader->getID(), "cam_pos"), cam_pos.x, cam_pos.y, cam_pos.z);
	glUniform1i(glGetUniformLocation(skyShader->getID(), "tex"), 0);

	glDepthFunc(GL_LEQUAL);//Since sky shader always draws at maximum depth (1) we need to use less-than-or-equal
	glDepthMask(GL_FALSE);//Disable writing to depth buffer
	glCullFace(GL_FRONT);//Draw cube from the inside

	glm::mat4 sky_mat = glm::translate(glm::mat4(), cam_pos);
	glUniformMatrix4fv(glGetUniformLocation(skyShader->getID(), "M"), 1, GL_FALSE, glm::value_ptr(sky_mat));
	sky->draw();

	//Enabling Depth Test and Culling 
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glCullFace(GL_BACK);

	glActiveTexture(GL_TEXTURE0);

	//Checking if the Grain Effect is Enabled, if so, Apply it
	if (useGrainEffect) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		grainEffectShader->use();
		fboTex->bind();
		glUniform1f(timeLoc, (float)glfwGetTime());
		glUniform1f(grainLoc, 25);
		quad->draw();
	}
}

void MainScene::Finalize() {
	//Dynamic Deallocation
	delete shader;
	delete grainEffectShader;
	delete depthShader;

	delete ground;
	delete house;
	delete key;
	delete hands;
	delete tree;
	delete quad;

	delete controller;
	delete camera;
	
	for (int i = 0; i < 3; i++) {
		delete groundTex[i];
		delete houseTex[i];
		delete keyTex[i];
		delete handsTex[i];
		delete treeTex[i];
	}

	delete fboTex;
	delete fboDepthTex;
	delete depthCubeMap;
	delete fbo;
	delete depthMapFBO;
}
