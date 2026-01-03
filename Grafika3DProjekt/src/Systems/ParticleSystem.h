#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "../Rendering/Shader.h"
#include "../Rendering/Texture.h"

/**
 * @brief Single particle instance.
 */
struct Particle {
    glm::vec3 Position, Velocity;
    glm::vec4 Color;
    float     Life;
    float     Size;

    Particle()
        : Position(0.0f), Velocity(0.0f), Color(1.0f),
          Life(0.0f), Size(1.0f) {}
};

/**
 * @brief Default settings used when spawning particles.
 */
struct ParticleProperties {
    glm::vec3 Velocity;
    glm::vec4 ColorBegin;
    glm::vec4 ColorEnd;
    float Life;
    float SizeBegin;
    float SizeEnd;
    float Spread;
};

/**
 * @class ParticleSystem
 * @brief GPU‑based particle system using instanced quads.
 *
 * Handles updating, spawning, and drawing particles.
 */
class ParticleSystem
{
public:
    /**
     * @brief Creates a particle system.
     *
     * @param shader Shader used to render particles.
     * @param texture Texture applied to particles.
     * @param amount Number of particles in the pool.
     * @param props Default particle properties.
     */
    ParticleSystem(Shader* shader, Texture* texture,
                   unsigned int amount, ParticleProperties props);

    /** @brief Updates all particles. */
    void Update(float dt);

    /**
     * @brief Spawns new particles at a position.
     *
     * @param position Spawn position.
     * @param amount Number of particles to spawn.
     * @param offset Optional offset.
     */
    void SpawnParticles(glm::vec3 position, unsigned int amount,
                        glm::vec3 offset = glm::vec3(0.0f));

    /** @brief Draws all active particles. */
    void Draw();

    /** @brief Destructor. */
    ~ParticleSystem();

private:
    std::vector<Particle> particles; ///< Particle pool.
    ParticleProperties properties;   ///< Default spawn settings.
    unsigned int amount;             ///< Max particle count.

    Shader* shader;                  ///< Rendering shader.
    Texture* texture;                ///< Particle texture.

    unsigned int VAO;                ///< Quad VAO.
    unsigned int VBO;                ///< Quad VBO.

    /** @brief Initializes buffers and particle pool. */
    void init();

    /** @brief Finds the first dead particle. */
    unsigned int firstUnusedParticle();

    /** @brief Resets a particle to a new state. */
    void respawnParticle(Particle& particle, glm::vec3 position,
                         glm::vec3 offset = glm::vec3(0.0f));
};
