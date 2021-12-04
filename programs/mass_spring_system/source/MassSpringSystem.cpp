#include "MassSpringSystem.h"

MASS_SPRING_SYSTEM::MASS_SPRING_SYSTEM(
    int maxParticles,
    int maxSprings)
{
    init(maxParticles, maxSprings);
}

void MASS_SPRING_SYSTEM::init(
    int maxParticles,
    int maxSprings
) {
    m_Gravity = Vector3(0.0, -9.8, 0.0);
    m_FlgCollisionConstraints = false;

    m_MaxParticles = maxParticles;
    m_CurParticles = 0;
    mParticles = new PARTICLE[m_MaxParticles];
    //
    m_MaxSprings = maxSprings;
    m_CurSprings = 0;
    mSprings = new SPRING[m_MaxSprings];
}

int MASS_SPRING_SYSTEM::addParticle(SceneNode *n, Real radius, bool movable)
{
	mParticles[m_CurParticles].snode = n;
	mParticles[m_CurParticles].radius = radius;
	mParticles[m_CurParticles].movable = movable;
	m_CurParticles++;
    return 0;
}

SPRING *MASS_SPRING_SYSTEM::addSpring(
    int n0, int n1)
{
	mSprings[m_CurSprings].n0 = n0;
	mSprings[m_CurSprings].n1 = n1;
	Vector3 a = mParticles[n0].snode->getPosition();
	Vector3 b = mParticles[n1].snode->getPosition();
	mSprings[m_CurSprings].L0 = (a - b).length();
	mSprings[m_CurSprings].k = 500;
	m_CurSprings++;
    SPRING *s = 0;
    return s;
}

void MASS_SPRING_SYSTEM::setGravity(const Vector3 &g)
{
    m_Gravity = g;
}

//
// reset force
// compute damping force
// compute viscous damping force between particles
// compute spring force
// compute net force
void MASS_SPRING_SYSTEM::computeForce(float time_step)
{
	// gravaity
    for (int i = 0; i < m_CurParticles; ++i ) {
		mParticles[i].f = (mParticles[i].mass * time_step * m_Gravity);
    }

	// damping force 1
    for (int i = 0; i < m_CurParticles; ++i ) {
		mParticles[i].f -= 0.025 * mParticles[i].velocity;
    }

	// damping force 2
    for (int i = 0; i < m_CurSprings; ++i ) {
		SPRING spring = mSprings[i];
		Vector3 a = mParticles[spring.n0].velocity;
		Vector3 b = mParticles[spring.n1].velocity;
		Vector3 vec = a - b;
		
		mParticles[spring.n1].f -= 0.15 * vec * time_step;
		mParticles[spring.n0].f -= 0.15 * -vec * time_step;
    }

	// spring force
    for (int i = 0; i < m_CurSprings; ++i ) {
		SPRING spring = mSprings[i];
		Vector3 a = mParticles[spring.n0].snode->getPosition();
		Vector3 b = mParticles[spring.n1].snode->getPosition();
		Vector3 vec = a - b;
		vec.normalise();
		
		mParticles[spring.n1].f += spring.k * ((a - b).length() - spring.L0) * vec * time_step;
		mParticles[spring.n0].f += spring.k * ((a - b).length() - spring.L0) * -vec * time_step;
    }

	// reset
    for (int i = 0; i < 8; ++i ) {
		mParticles[i].f = 0;
    }
}

void MASS_SPRING_SYSTEM::computeVelocity(float time_step)
{
    for (int i = 8; i < m_CurParticles; ++i ) {
        mParticles[i].velocity += (mParticles[i].f / mParticles[i].mass) * time_step;
    }
}

void MASS_SPRING_SYSTEM::computePosition(float time_step)
{
    for (int i = 0; i < m_CurParticles; ++i ) {
		Vector3 pos = mParticles[i].snode->getPosition();
		Vector3 vec = mParticles[i].velocity;
		mParticles[i].snode->setPosition(pos.x + vec.x * time_step, pos.y + vec.y * time_step, pos.z + vec.z * time_step);
    }
}

void MASS_SPRING_SYSTEM::adjustVelocityDueToCollisionConstraint_Floor(float time_step)
{
    float e = 0.05;
    for (int i = 0; i < m_CurParticles; ++i ) {
		Vector3 pos = mParticles[i].snode->getPosition();
		if (pos.y <= e) {
			mParticles[i].velocity *= -1;
		}
    }
}

void MASS_SPRING_SYSTEM::adjustVelocityDueToCollisionConstraint_LargeSphere(float time_step)
{
    float large_radius = 100.0;
    float small_radius = 5.0;
    float e = 0.05;

	for (int i = 0; i < m_CurParticles; ++i ) {
		Vector3 pos = mParticles[i].snode->getPosition();
		float R = large_radius + small_radius + 0.05;
    
		Vector3 dir = -pos;
		dir.normalise();
		Vector3 vec = mParticles[i].velocity;
		Real innerProduct = vec.dotProduct(dir);
		if (dir.length() < R && innerProduct > 0) {
			dir.normalise();
			mParticles[i].velocity -= innerProduct * dir * time_step;
		}
	}
}

void MASS_SPRING_SYSTEM::adjustVelocityDueToCollisionConstraint(float time_step)
{
    adjustVelocityDueToCollisionConstraint_Floor(time_step);
    adjustVelocityDueToCollisionConstraint_LargeSphere(time_step);

}

void MASS_SPRING_SYSTEM::reset()
{
    resetVelocity();
}

void MASS_SPRING_SYSTEM::resetVelocity()
{
    if (m_CurParticles < 0) return;
    for (int i = 0; i < m_CurParticles; ++i ) {
        PARTICLE *p = &mParticles[i];
        p->velocity = Vector3::ZERO;
    }
}

void MASS_SPRING_SYSTEM::setCollisionConstraint(bool enable)
{
    m_FlgCollisionConstraints = enable;
}

void MASS_SPRING_SYSTEM::update(float time_step) {
    computeForce(time_step);
    computeVelocity(time_step);
    if (m_FlgCollisionConstraints) adjustVelocityDueToCollisionConstraint(time_step);
    computePosition(time_step);
}