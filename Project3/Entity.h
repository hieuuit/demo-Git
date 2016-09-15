
#ifndef ENTITY_H               
#define ENTITY_H               
#define WIN32_LEAN_AND_MEAN

#include "Image.h"
#include "Input.h"
#include "Game.h"

namespace EntityNS
{
	enum COLLISION_TYPE
	{
		NONE,
		CIRCLE,
		BOX,
		ROTATED_BOX
	};
	const float GRAVITY = 6.67428e-11f;
}

class Entity : public Image
{
protected:
	EntityNS::COLLISION_TYPE _collisionType;
	VECTOR2 _center;
	float _radius;
	VECTOR2 _distSquared;
	float _sumRadiiSquared;
	RECT _edge;
	VECTOR2 _corners[4];
	VECTOR2 _edge01, _edge03;
	float _edge01Min, _edge01Max, _edge03Min, _edge03Max;
	VECTOR2 _velocity;
	VECTOR2 _deltaV;
	float _mass;
	float _health;
	float _rr;
	float _force;
	float _gravity;
	Input *_input;
	HRESULT _result;
	bool _active;
	bool _rotatedBoxReady;
	virtual bool collideCircle(Entity &ent, VECTOR2 &collisionVector);
	virtual bool collideBox(Entity &ent, VECTOR2 &collisionVector);
	virtual bool collideRotatedBox(Entity &ent, VECTOR2 &collisionVector);
	virtual bool collideRotatedBoxCircle(Entity &ent, VECTOR2 &collisionVector);
	void computeRotatedBox();
	bool projectionsOverlap(Entity &ent);
	bool collideCornerCircle(VECTOR2 corner, Entity &ent, VECTOR2 &collisionVector);

public:
	Entity();
	virtual ~Entity() {}

	virtual const VECTOR2* getCenter() { _center = VECTOR2(getCenterX(), getCenterY()); return &_center; }
	virtual float getRadius() const { return _radius; }
	virtual const RECT& getEdge() const { return _edge; }
	virtual const VECTOR2* getCorner(UINT c) const { if (c >= 4) c = 0; return &_corners[c]; }
	virtual const VECTOR2 getVelocity() const { return _velocity; }
	virtual bool  getActive() const { return _active; }
	virtual float getMass() const { return _mass; }
	virtual float getGravity() const { return _gravity; }
	virtual float getHealth() const { return _health; }
	virtual EntityNS::COLLISION_TYPE getCollisionType() { return _collisionType; }

	virtual void  setVelocity(VECTOR2 v) { _velocity = v; }
	virtual void  setDeltaV(VECTOR2 dv) { _deltaV = dv; }
	virtual void  setActive(bool a) { _active = a; }
	virtual void setHealth(float h) { _health = h; }
	virtual void  setMass(float m) { _mass = m; }
	virtual void  setGravity(float g) { _gravity = g; }
	virtual void setCollisionRadius(float r) { _radius = r; }

	virtual void update(float frameTime);
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM); virtual void activate();
	virtual void ai(float frameTime, Entity &ent);
	virtual bool outsideRect(RECT rect);
	virtual bool collidesWith(Entity &ent, VECTOR2 &collisionVector);
	virtual void damage(int weapon);
	void bounce(VECTOR2 &collisionVector, Entity &ent);
	void gravityForce(Entity *other, float frameTime);
};

#endif
