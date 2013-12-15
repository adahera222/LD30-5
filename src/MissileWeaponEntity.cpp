#include "Common.h"
#include "MissileWeaponEntity.h"

MissileWeaponEntity::MissileWeaponEntity(const Vec2& startingPosition, weak_ptr<DamageableEntity> target) :
	SpecialWeaponEntity(startingPosition, target)
{
	mMissileTexture.loadFromFile("media/missile.png");
	Vec2 textureSize((float)mMissileTexture.getSize().x, (float)mMissileTexture.getSize().y);
	mMissileSprite.setTexture(mMissileTexture);
	mMissileSprite.setOrigin(textureSize * 0.5f);
	mMissileSprite.setRotation(180.0f);
}

MissileWeaponEntity::~MissileWeaponEntity()
{
}

void MissileWeaponEntity::update(float dt)
{
	float currentAngle = mMissileSprite.getRotation();

	// Rotate towards target using lerp
	float speed = 600.0f;
	shared_ptr<DamageableEntity> target = mTarget.lock();
	if (target != shared_ptr<DamageableEntity>())
	{
		Vec2 targetDirection = target->getSprite().getPosition() - mPosition;
		float targetAngle = atan2(targetDirection.y, targetDirection.x) * RAD_TO_DEG + 90.0f;
		if (targetAngle > currentAngle)
		{
			while (targetAngle - currentAngle > 180.0f)
				targetAngle -= 360.0f;
		}
		else
		{
			while (targetAngle - currentAngle < -180.0f)
				targetAngle += 360.0f;
		}
		float swayFactor = 1.0f + sin(random(0.0f, TWO_PI));
		mMissileSprite.setRotation(step(currentAngle, targetAngle, 500.0f * swayFactor * dt));
		speed = 600.0f / (1.0f + abs(targetAngle - currentAngle) / 180.0f);
	}

	// Move
	Vec2 direction(sin(currentAngle * DEG_TO_RAD), -cos(currentAngle * DEG_TO_RAD));
	mPosition += direction * speed * dt;
}

void MissileWeaponEntity::render(sf::RenderWindow& window)
{
	mMissileSprite.setPosition(mPosition);
	window.draw(mMissileSprite);
}

void MissileWeaponEntity::onCollision(shared_ptr<Entity> other)
{
}

sf::Sprite& MissileWeaponEntity::getSprite()
{
	return mMissileSprite;
}
