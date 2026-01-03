#include "ParticleSystem.h"

/**
 * @brief Creates a particle system with a shader, texture, and settings.
 *
 * @param shader Shader used to render particles.
 * @param texture Texture applied to each particle.
 * @param amount Number of particles in the pool.
 * @param props Default particle properties.
 */
ParticleSystem::ParticleSystem(Shader* shader, Texture* texture,
                               unsigned int amount, ParticleProperties props)
    : shader(shader), texture(texture), amount(amount), properties(props)
{
    this->init();
}

/**
 * @brief Updates all particles.
 *
 * Decreases life, moves particles, and interpolates color/size.
 *
 * @param dt Delta time.
 */
void ParticleSystem::Update(float dt)
{
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        Particle& p = this->particles[i];
        p.Life -= dt;

        if (p.Life > 0.0f)
        {
            p.Position += p.Velocity * dt;
            float lifeRatio = p.Life / this->properties.Life;

            p.Color = glm::mix(this->properties.ColorEnd,
                               this->properties.ColorBegin, lifeRatio);

            p.Size = glm::mix(this->properties.SizeEnd,
                              this->properties.SizeBegin, lifeRatio);
        }
    }
}

/**
 * @brief Spawns new particles at a position.
 *
 * @param position Spawn position.
 * @param newParticles Number of particles to spawn.
 * @param offset Additional offset.
 */
void ParticleSystem::SpawnParticles(glm::vec3 position,
                                    unsigned int newParticles,
                                    glm::vec3 offset)
{
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unused = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unused], position, offset);
    }
}

/**
 * @brief Draws all active particles.
 */
void ParticleSystem::Draw()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);

    shader->UseShader();
    shader->setInt("sprite", 0);
    texture->UseTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);

    for (Particle& p : particles)
    {
        if (p.Life > 0.0f)
        {
            shader->setVec3("offset", p.Position);
            shader->setVec4("color", p.Color);
            shader->setFloat("scale", p.Size);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    glBindVertexArray(0);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glDisable(GL_BLEND);
}

/**
 * @brief Initializes particle quad mesh and particle pool.
 */
void ParticleSystem::init()
{
    float quad[] = {
        -0.5f,  0.5f, 0.0f, 0,1,
         0.5f, -0.5f, 0.0f, 1,0,
        -0.5f, -0.5f, 0.0f, 0,0,

        -0.5f,  0.5f, 0.0f, 0,1,
         0.5f,  0.5f, 0.0f, 1,1,
         0.5f, -0.5f, 0.0f, 1,0
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);

    for (unsigned int i = 0; i < amount; ++i)
        particles.push_back(Particle());
}

unsigned int lastUsedParticle = 0;

/**
 * @brief Finds the first dead particle to reuse.
 *
 * @return Index of unused particle.
 */
unsigned int ParticleSystem::firstUnusedParticle()
{
    for (unsigned int i = lastUsedParticle; i < amount; ++i)
        if (particles[i].Life <= 0.0f)
            return lastUsedParticle = i;

    for (unsigned int i = 0; i < lastUsedParticle; ++i)
        if (particles[i].Life <= 0.0f)
            return lastUsedParticle = i;

    return 0;
}

/**
 * @brief Resets a particle to a new position and default properties.
 *
 * @param particle Particle to reset.
 * @param position Base position.
 * @param offset Additional offset.
 */
void ParticleSystem::respawnParticle(Particle& particle,
                                     glm::vec3 position,
                                     glm::vec3 offset)
{
    float randomX = ((rand() % 100) - 50) / properties.Spread;
    float randomY = ((rand() % 100) - 50) / properties.Spread;
    float randomZ = ((rand() % 100) - 50) / properties.Spread;

    particle.Position = position + glm::vec3(randomX, randomY, randomZ) + offset;
    particle.Color = properties.ColorBegin;
    particle.Life = properties.Life;
    particle.Velocity = properties.Velocity;
    particle.Size = properties.SizeBegin;
}
