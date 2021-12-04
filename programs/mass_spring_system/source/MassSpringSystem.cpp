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
    return 0;
    
}

SPRING *MASS_SPRING_SYSTEM::addSpring(
    int n0, int n1)
{
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
    for (int i = 0; i < m_CurParticles; ++i ) {
        mParticles[i].f = Vector3::ZERO;
    }

}

void MASS_SPRING_SYSTEM::computeVelocity(float time_step)
{
}

void MASS_SPRING_SYSTEM::computePosition(float time_step)
{
}

void MASS_SPRING_SYSTEM::adjustVelocityDueToCollisionConstraint_Floor(float time_step)
{
    float e = 0.05;

}

void MASS_SPRING_SYSTEM::adjustVelocityDueToCollisionConstraint_LargeSphere(float time_step)
{
    float large_radius = 100.0;
    float small_radius = 5.0;
    float e = 0.05;
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