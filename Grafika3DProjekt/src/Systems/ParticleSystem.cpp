#include "ParticleSystem.h"


ParticleSystem::ParticleSystem(Shader* shader, Texture *texture, unsigned int amount, ParticleProperties props)
	: shader(shader), texture(texture), amount(amount), properties(props)
{
	this->init();
}

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
			p.Color = glm::mix(this->properties.ColorEnd, this->properties.ColorBegin, lifeRatio);
			p.Size = glm::mix(this->properties.SizeEnd, this->properties.SizeBegin, lifeRatio);
		}
	}
}

void ParticleSystem::SpawnParticles(glm::vec3 position, unsigned int newParticles, glm::vec3 offset)
{
	for (unsigned int i = 0; i < newParticles; ++i)
	{
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->particles[unusedParticle], position, offset);
	}
}


void ParticleSystem::Draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(GL_FALSE); 
	glDepthFunc(GL_LEQUAL);;
	glDisable(GL_CULL_FACE);

	this->shader->UseShader();

	this->shader->setInt("sprite", 0);

	this->texture->UseTexture(GL_TEXTURE0);

	glBindVertexArray(this->VAO);

	for (Particle& particle : this->particles)
	{
		if (particle.Life > 0.0f)
		{
			this->shader->setVec3("offset", particle.Position);
			this->shader->setVec4("color", particle.Color);
			this->shader->setFloat("scale", particle.Size);
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
void ParticleSystem::init()
{
	float particle_quad[] = {
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,

		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);


	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);

	for (unsigned int i = 0; i < this->amount; ++i)
		this->particles.push_back(Particle());
}

unsigned int lastUsedParticle = 0;

unsigned int ParticleSystem::firstUnusedParticle()
{
	for (unsigned int i = lastUsedParticle; i < this->amount; ++i) {
		if (this->particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	for (unsigned int i = 0; i < lastUsedParticle; ++i) {
		if (this->particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	return 0;
}


void ParticleSystem::respawnParticle(Particle& particle, glm::vec3 position, glm::vec3 offset)
{
	float randomX = ((rand() % 100) - 50) / properties.Spread;
	float randomY = ((rand() % 100) - 50) / properties.Spread;
	float randomZ = ((rand() % 100) - 50) / properties.Spread;

	float rColor = 0.5f + ((rand() % 100) / 100.0f);

	particle.Position = position + glm::vec3(randomX, randomY, randomZ) + offset;
	particle.Color = this->properties.ColorBegin;
	particle.Life = this->properties.Life;
	particle.Velocity = this->properties.Velocity;
	particle.Size = this->properties.SizeBegin;
}