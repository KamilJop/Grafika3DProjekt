#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "../Rendering/Shader.h"
#include "../Rendering/Texture.h"
struct Particle {
    glm::vec3 Position, Velocity;
    glm::vec4 Color;
    float     Life;
	float	  Size;

    Particle()
        : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f), Size(1.0f) {
    }
};


struct ParticleProperties {
    glm::vec3 Velocity;
    glm::vec4 ColorBegin;
	glm::vec4 ColorEnd;
    float Life;
	float SizeBegin;
	float SizeEnd;
    float Spread;
};

class ParticleSystem
{
public:
    ParticleSystem(Shader* shader, Texture* texture, unsigned int amount, ParticleProperties props);
    void Update(float dt);
    void SpawnParticles(glm::vec3 position, unsigned int amount, glm::vec3 offset = glm::vec3(0.0f));
	void Draw();
	~ParticleSystem();
private:
    std::vector<Particle> particles;
	ParticleProperties properties;
    unsigned int amount;
    Shader* shader;
    Texture* texture;
    unsigned int VAO;
    unsigned int VBO;
    void init();
    unsigned int firstUnusedParticle();
	void respawnParticle(Particle& particle, glm::vec3 position, glm::vec3 offset = glm::vec3(0.0f));
};

