#ifndef MAIN_SCENE_HPP
#define MAIN_SCENE_HPP

#define GLM_FORCE_CXX11
#include <glm/glm.hpp>

#include <scene.hpp>
#include <shader.hpp>
#include <mesh/mesh.hpp>
#include <textures/texture2d.hpp>
#include <framebuffer.hpp>
#include <camera/camera.hpp>
#include <camera/controllers/fly_camera_controller.hpp>

// A tuple to define the texture types we will use.
enum TextureType {
	ALBEDO = 0,
	SPECULAR = 1,
	AMBIENT_OCCLUSION = 2
};

const unsigned int shadow_res = 4000;  //A constant for shadow resolution

class MainScene : public Scene {
private:
	Shader * shader, *grainEffectShader, *depthShader, *skyShader;    //Shader objects
	Mesh* ground, *house, *key, *hands, *tree, *quad, *sky;           //Models Objects
	Camera* camera;                                                   //Camera Object
	FlyCameraController* controller;								  //Controller Object

	Texture2D* groundTex[3];                                          //Ground Object
	Texture2D* houseTex[3];                                           //House Object
	Texture2D* keyTex[3];                                             //Key Object
	Texture2D* handsTex[3];                                           //Hands Object
	Texture2D* treeTex[3];	                                          //Key Object
	
	Texture2D* fboTex, *fboDepthTex;                                  //Framebuffer and Depth Texture Object
	TextureCube* depthCubeMap, *env;                                  //Cube Texture Object for Depth Map
	FrameBuffer* fbo, *depthMapFBO;                                   //Framebuffer Objects

	GLuint timeLoc, grainLoc;                                         //Constants for Grain Effect

	glm::vec3 moonPos;                                                //Moon Position
	float moonRadius, lightFarPlane;                                  //Moon Radius and Far Plane for the Light

	glm::mat4 shadowProj;                                             //Shadow Projection Matrix
	std::vector<glm::mat4> shadowTransforms;                          //Shadow Tranformation Matrix

	bool keyExists, useGrainEffect, useShadows;                       //Booleans for Enabling Effects

public:
    MainScene(Application* app): Scene(app) {}

    void Initialize() override;
    void Update(double delta_time) override;
    void Draw() override;
    void Finalize() override;
};

#endif