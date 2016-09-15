
#include "entity.h"
Entity::Entity() : Image()
{
	_radius = 1.0;
	_edge.left = -1;
	_edge.top = -1;
	_edge.right = 1;
	_edge.bottom = 1;
	_mass = 1.0;
	_velocity.x = 0.0;
	_velocity.y = 0.0;
	_deltaV.x = 0.0;
	_deltaV.y = 0.0;
	_active = true;
	_rotatedBoxReady = false;
	_collisionType = EntityNS::CIRCLE;
	_health = 100;
	_gravity = EntityNS::GRAVITY;
}
bool Entity::initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM)
{
	_input = gamePtr->getInput();
	return(Image::initialize(gamePtr->getGraphics(), width, height, ncols, textureM));
}
void Entity::activate()
{
	_active = true;
}
void Entity::update(float frameTime)
{
	_velocity += _deltaV;
	_deltaV.x = 0;
	_deltaV.y = 0;
	Image::update(frameTime);
	_rotatedBoxReady = false;
}
void Entity::ai(float frameTime, Entity &ent)
{
}
bool Entity::collidesWith(Entity &ent, VECTOR2 &collisionVector)
{

	if (!_active || !ent.getActive())
		return false;
	if (_collisionType == EntityNS::CIRCLE && ent.getCollisionType() == EntityNS::CIRCLE)
		return collideCircle(ent, collisionVector);

	if (_collisionType == EntityNS::BOX && ent.getCollisionType() == EntityNS::BOX)
		return collideBox(ent, collisionVector);


	if (_collisionType != EntityNS::CIRCLE && ent.getCollisionType() != EntityNS::CIRCLE)
		return collideRotatedBox(ent, collisionVector);
	else
		if (_collisionType == EntityNS::CIRCLE)
		{

			bool collide = ent.collideRotatedBoxCircle(*this, collisionVector);

			collisionVector *= -1;
			return collide;
		}
		else
			return collideRotatedBoxCircle(ent, collisionVector);
	return false;
}
bool Entity::collideCircle(Entity &ent, VECTOR2 &collisionVector)
{

	_distSquared = *getCenter() - *ent.getCenter();
	_distSquared.x = _distSquared.x * _distSquared.x;
	_distSquared.y = _distSquared.y * _distSquared.y;
	_sumRadiiSquared = (_radius * getScale()) + (ent._radius * ent.getScale());
	_sumRadiiSquared *= _sumRadiiSquared;
	if (_distSquared.x + _distSquared.y <= _sumRadiiSquared)
	{
		collisionVector = *ent.getCenter() - *getCenter();
		return true;
	}
	return false;
}
bool Entity::collideBox(Entity &ent, VECTOR2 &collisionVector)
{
	if (!_active || !ent.getActive())
		return false;
	if ((getCenterX() + _edge.right*getScale() >= ent.getCenterX() + ent.getEdge().left*ent.getScale()) &&
		(getCenterX() + _edge.left*getScale() <= ent.getCenterX() + ent.getEdge().right*ent.getScale()) &&
		(getCenterY() + _edge.bottom*getScale() >= ent.getCenterY() + ent.getEdge().top*ent.getScale()) &&
		(getCenterY() + _edge.top*getScale() <= ent.getCenterY() + ent.getEdge().bottom*ent.getScale()))
	{
		collisionVector = *ent.getCenter() - *getCenter();
		return true;
	}
	return false;
}
bool Entity::collideRotatedBox(Entity &ent, VECTOR2 &collisionVector)
{
	computeRotatedBox();
	ent.computeRotatedBox();
	if (projectionsOverlap(ent) && ent.projectionsOverlap(*this))
	{

		collisionVector = *ent.getCenter() - *getCenter();
		return true;
	}
	return false;
}
bool Entity::projectionsOverlap(Entity &ent)
{
	float projection, min01, max01, min03, max03;
	projection = _graphics->Vector2Dot(&_edge01, ent.getCorner(0));
	min01 = projection;
	max01 = projection;

	for (int c = 1; c<4; c++)
	{

		projection = _graphics->Vector2Dot(&_edge01, ent.getCorner(c));
		if (projection < min01)
			min01 = projection;
		else if (projection > max01)
			max01 = projection;
	}
	if (min01 > _edge01Max || max01 < _edge01Min)
		return false;
	projection = _graphics->Vector2Dot(&_edge03, ent.getCorner(0));
	min03 = projection;
	max03 = projection;

	for (int c = 1; c<4; c++)
	{
		projection = _graphics->Vector2Dot(&_edge03, ent.getCorner(c));
		if (projection < min03)
			min03 = projection;
		else if (projection > max03)
			max03 = projection;
	}
	if (min03 > _edge03Max || max03 < _edge03Min)
		return false;                           return true;
}
bool Entity::collideRotatedBoxCircle(Entity &ent, VECTOR2 &collisionVector)
{
	float min01, min03, max01, max03, center01, center03;    computeRotatedBox();
	center01 = _graphics->Vector2Dot(&_edge01, ent.getCenter());
	min01 = center01 - ent.getRadius()*ent.getScale();
	max01 = center01 + ent.getRadius()*ent.getScale();
	if (min01 > _edge01Max || max01 < _edge01Min)
		return false;


	center03 = _graphics->Vector2Dot(&_edge03, ent.getCenter());
	min03 = center03 - ent.getRadius()*ent.getScale();
	max03 = center03 + ent.getRadius()*ent.getScale();
	if (min03 > _edge03Max || max03 < _edge03Min)
		return false;

	if (center01 < _edge01Min && center03 < _edge03Min)
		return collideCornerCircle(_corners[0], ent, collisionVector);
	if (center01 > _edge01Max && center03 < _edge03Min)
		return collideCornerCircle(_corners[1], ent, collisionVector);
	if (center01 > _edge01Max && center03 > _edge03Max)
		return collideCornerCircle(_corners[2], ent, collisionVector);
	if (center01 < _edge01Min && center03 > _edge03Max)
		return collideCornerCircle(_corners[3], ent, collisionVector);

	collisionVector = *ent.getCenter() - *getCenter();
	return true;
}bool Entity::collideCornerCircle(VECTOR2 corner, Entity &ent, VECTOR2 &collisionVector)
{
	_distSquared = corner - *ent.getCenter();
	_distSquared.x = _distSquared.x * _distSquared.x;
	_distSquared.y = _distSquared.y * _distSquared.y;
	_sumRadiiSquared = ent.getRadius()*ent.getScale();
	_sumRadiiSquared *= _sumRadiiSquared;
	if (_distSquared.x + _distSquared.y <= _sumRadiiSquared)
	{

		collisionVector = *ent.getCenter() - corner;
		return true;
	}
	return false;
}
void Entity::computeRotatedBox()
{
	if (_rotatedBoxReady)
		return;
	float projection;
	VECTOR2 rotatedX(cos(_spriteData.angle), sin(_spriteData.angle));
	VECTOR2 rotatedY(-sin(_spriteData.angle), cos(_spriteData.angle));    const VECTOR2 *center = getCenter();
	_corners[0] = *center + rotatedX * ((float)_edge.left*getScale()) +
		rotatedY * ((float)_edge.top*getScale());
	_corners[1] = *center + rotatedX * ((float)_edge.right*getScale()) +
		rotatedY * ((float)_edge.top*getScale());
	_corners[2] = *center + rotatedX * ((float)_edge.right*getScale()) +
		rotatedY * ((float)_edge.bottom*getScale());
	_corners[3] = *center + rotatedX * ((float)_edge.left*getScale()) +
		rotatedY * ((float)_edge.bottom*getScale());

	_edge01 = VECTOR2(_corners[1].x - _corners[0].x, _corners[1].y - _corners[0].y);
	_graphics->Vector2Normalize(&_edge01);
	_edge03 = VECTOR2(_corners[3].x - _corners[0].x, _corners[3].y - _corners[0].y);
	_graphics->Vector2Normalize(&_edge03);
	projection = _graphics->Vector2Dot(&_edge01, &_corners[0]);
	_edge01Min = projection;
	_edge01Max = projection;

	projection = _graphics->Vector2Dot(&_edge01, &_corners[1]);
	if (projection < _edge01Min)
		_edge01Min = projection;
	else if (projection > _edge01Max)
		_edge01Max = projection;

	projection = _graphics->Vector2Dot(&_edge03, &_corners[0]);
	_edge03Min = projection;
	_edge03Max = projection;
	projection = _graphics->Vector2Dot(&_edge03, &_corners[3]);
	if (projection < _edge03Min)
		_edge03Min = projection;
	else if (projection > _edge03Max)
		_edge03Max = projection;
	_rotatedBoxReady = true;
}
bool Entity::outsideRect(RECT rect)
{
	if (_spriteData.x + _spriteData.width*getScale() < rect.left ||
		_spriteData.x > rect.right ||
		_spriteData.y + _spriteData.height*getScale() < rect.top ||
		_spriteData.y > rect.bottom)
		return true;
	return false;
}
void Entity::damage(int weapon)
{

}
void Entity::bounce(VECTOR2 &collisionVector, Entity &ent)
{
	VECTOR2 Vdiff = ent.getVelocity() - _velocity;
	VECTOR2 cUV = collisionVector;
	Graphics::Vector2Normalize(&cUV);
	float cUVdotVdiff = Graphics::Vector2Dot(&cUV, &Vdiff);
	float massRatio = 2.0f;
	if (getMass() != 0)
		massRatio *= (ent.getMass() / (getMass() + ent.getMass()));


	if (cUVdotVdiff > 0)
	{
		setX(getX() - cUV.x * massRatio);
		setY(getY() - cUV.y * massRatio);
	}
	else
		_deltaV += ((massRatio * cUVdotVdiff) * cUV);
}
void Entity::gravityForce(Entity *ent, float frameTime)
{
	if (!_active || !ent->getActive())
		return;
	_rr = pow((ent->getCenterX() - getCenterX()), 2) + pow((ent->getCenterY() - getCenterY()), 2);
	_force = _gravity * ent->getMass() * _mass / _rr;

	VECTOR2 gravityV(ent->getCenterX() - getCenterX(),
		ent->getCenterY() - getCenterY());

	Graphics::Vector2Normalize(&gravityV);

	gravityV *= _force * frameTime;

	_velocity += gravityV;
}
