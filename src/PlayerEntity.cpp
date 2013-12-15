// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "PlayerEntity.h"
#include "BulletManager.h"
#include "EntityManager.h"

PlayerEntity::PlayerEntity(const Vec2& position, uint shipID) :
	DamageableEntity(position, 1000, false),
	mShipID(shipID)
{
	// Load textures
	if (!mShipTexture.loadFromFile("media/player" + to_string(shipID) + ".png"))
		throw std::exception("Failed to load player texture");
	if (!mShieldTexture.loadFromFile("media/shield.png"))
		throw std::exception("Failed to load shield texture");
	if (!mTargetTexture.loadFromFile("media/target.png"))
		throw std::exception("Failed to load target texture");
	if (!mLockOnTexture.loadFromFile("media/lock.png"))
		throw std::exception("Failed to load lock texture");
	if (!mLockingOnTexture.loadFromFile("media/lock-progress.png"))
		throw std::exception("Failed to load lock-progress texture");

	// Load gun points
	// TODO move this to JSON
	if (shipID == 1)
	{
		mGunPoints.push_back(Vec2(-10.0f, -16.0f));
		mGunPoints.push_back(Vec2(10.0f, -16.0f));
		mGunPoints.push_back(Vec2(-22.0f, -8.0f));
		mGunPoints.push_back(Vec2(22.0f, -8.0f));
	}
	else if (shipID == 2)
	{
		mGunPoints.push_back(Vec2(-20.0f, -20.0f));
		mGunPoints.push_back(Vec2(20.0f, -20.0f));
	}

	// Set up ship sprite
	Vec2 shipTextureSize((float)mShipTexture.getSize().x, (float)mShipTexture.getSize().y);
	mShipSprite.setTexture(mShipTexture);
	mShipSprite.setOrigin(shipTextureSize * 0.5f);

	// Set up shield sprites
	Vec2 shieldTextureSize((float)mShieldTexture.getSize().x, (float)mShieldTexture.getSize().y);
	mShieldSprite.setTexture(mShieldTexture);
	mShieldSprite.setOrigin(shieldTextureSize * 0.5f);
	for (uint i = 0; i < 10; ++i)
	{
		Shield shield;
		shield.visibility = 0.0f;
		shield.rotation = 0.0f;
		mShields.push_back(shield);
	}

	// Set up target sprite
	Vec2 targetTextureSize((float)mTargetTexture.getSize().x, (float)mTargetTexture.getSize().y);
	mTargetSprite.setTexture(mTargetTexture);
	mTargetSprite.setOrigin(targetTextureSize * 0.5f);

	// Set up lock sprites
	Vec2 lockOnTextureSize((float)mLockOnTexture.getSize().x, (float)mLockOnTexture.getSize().y);
	mLockOnSprite.setTexture(mLockOnTexture);
	mLockOnSprite.setOrigin(lockOnTextureSize * 0.5f);
	Vec2 lockingOnTextureSize((float)mLockingOnTexture.getSize().x, (float)mLockingOnTexture.getSize().y);
	mLockingOnSprite.setTexture(mLockingOnTexture);
	mLockingOnSprite.setOrigin(lockingOnTextureSize * 0.5f);

	// Create locks
	for (uint i = 0; i < 8; ++i)
	{
		Lock lock;
		lock.hasLock = false;
		lock.lockProgress = 0.0f;
		mLocks.push_back(lock);
	}
}

PlayerEntity::~PlayerEntity()
{
}

void PlayerEntity::damage(const Vec2& direction, uint damageTaken)
{
	DamageableEntity::damage(direction, damageTaken);

	// Flare up shield
	_hitShield(direction);
}

bool PlayerEntity::inBulletTime() const
{
	return mBulletTime;
}

void PlayerEntity::_specialAttack()
{
	// Get entities
	vector<shared_ptr<DamageableEntity>> entities;
	for (auto i = mLocks.begin(); i != mLocks.end(); ++i)
	{
		if ((*i).hasLock && (*i).weapon.lock() == shared_ptr<MissileWeaponEntity>())
		{
			shared_ptr<DamageableEntity> entity = (*i).target.lock();
			if (entity != shared_ptr<DamageableEntity>())
			{
				// Do something depending on currently selected player
				// TODO: script this

				// Player 1
				// Fire a savlo of missiles from the back of the ship
				(*i).weapon = EntityManager::inst().createMissileWeapon(mPosition, entity);
			}
			(*i).fired = true;
		}
	}
}

void PlayerEntity::_hitShield(const Vec2& direction)
{
	// Take the front shield
	list<Shield>::iterator front = mShields.begin();

	// Flare that shield up
	(*front).visibility = 1.0f;
	(*front).rotation = atan2(direction.y, direction.x) * RAD_TO_DEG + 90.0f;

	// Move that shield to the back and shuffle the remaining shield elements forward by 1
	list<Shield>::iterator newFront = front;
	advance(newFront, 1);
	rotate(front, newFront, mShields.end());
}

void PlayerEntity::_drawLocks(sf::RenderWindow& window)
{
	// Draw lock ons
	static const float maxScale = 10.0f;
	for (auto i = mLocks.begin(); i != mLocks.end(); ++i)
	{
		if ((*i).hasLock)
		{
			shared_ptr<DamageableEntity> entity = (*i).target.lock();
			if (entity)
			{
				if ((*i).lockProgress < 1.0f)
				{
					mLockingOnSprite.setPosition(entity->getSprite().getPosition());
					float scale = maxScale - (maxScale - 1) * (*i).lockProgress;
					mLockingOnSprite.setScale(scale, scale);
					mLockingOnSprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)(255.0f * (*i).lockProgress)));
					window.draw(mLockingOnSprite);
				}
				else
				{
					mLockOnSprite.setPosition(entity->getSprite().getPosition());
					window.draw(mLockOnSprite);
				}
			}
		}
	}
}

void PlayerEntity::update(float dt)
{
	// Handle controls
	static const float MAX_VELOCITY = 300.0f;
	static const float ACCELERATION = 2000.0f;
	Vec2 targetVelocity(0.0f, 0.0f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		targetVelocity.y = -MAX_VELOCITY;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		targetVelocity.y = MAX_VELOCITY;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		targetVelocity.x = -MAX_VELOCITY;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		targetVelocity.x = MAX_VELOCITY;

	// Bullet time
	mBulletTime = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);

	// Shooting
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (mGunTimer.getElapsedTime().asSeconds() > 0.1f / Game::getTimeRate())
		{
			for (auto i = mGunPoints.begin(); i != mGunPoints.end(); ++i)
				BulletManager::inst().spawn(mPosition + (*i), Vec2(0.0f, -500.0f), shared_from_this(), BT_PLAYER_COMMON, true);
			mGunTimer.restart();
		}
	}

	// Advance locks
	for (auto i = mLocks.begin(); i != mLocks.end(); ++i)
	{
		if ((*i).hasLock)
		{
			// Remove the lock if the target or weapon was destroyed after it was fired
			if ((*i).target.lock() == shared_ptr<DamageableEntity>() ||
				((*i).fired && (*i).weapon.lock() == shared_ptr<MissileWeaponEntity>()))
			{
				(*i).target = shared_ptr<DamageableEntity>();
				(*i).lockProgress = 0.0f;
				(*i).hasLock = false;
				(*i).fired = false;
				(*i).weapon = shared_ptr<MissileWeaponEntity>();
			}
			else
			{
				(*i).lockProgress += 4.0f * dt;
				if ((*i).lockProgress > 1.0f)
					(*i).lockProgress = 1.0f;
			}
		}
	}

	// Special attack
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		_specialAttack();

	// Update velocity
	mVelocity.x = step(mVelocity.x, targetVelocity.x, ACCELERATION * dt);
	mVelocity.y = step(mVelocity.y, targetVelocity.y, ACCELERATION * dt);
	mPosition += mVelocity * dt;

	// Fade out shields
	for (auto i = mShields.begin(); i != mShields.end(); ++i)
	{
		if ((*i).visibility > EPSILON)
		{
			(*i).visibility -= dt;
			if ((*i).visibility < 0.0f)
				(*i).visibility = 0.0f;
		}
	}

	// Check if the target touched anything
	float targetRadiusSq = ((float)(mTargetTexture.getSize().x * mTargetTexture.getSize().x) +
		(float)(mTargetTexture.getSize().y * mTargetTexture.getSize().y)) * 0.25f;
	for (auto i = EntityManager::inst().getEntitiesBegin(); i != EntityManager::inst().getEntitiesEnd(); ++i)
	{
		if (*i != shared_from_this())
		{
			shared_ptr<DamageableEntity> ent = dynamic_pointer_cast<DamageableEntity>(*i);
			if (ent != shared_ptr<DamageableEntity>())
			{
				if (ent->isLockable())
				{
					// The entitys centre must be inside the target for a lock to register
					Vec2 delta = mTargetSprite.getPosition() - ent->getSprite().getPosition();
					float distanceSq = delta.x * delta.x + delta.y * delta.y;
					if (distanceSq < targetRadiusSq)
					{
						// Make sure this entity hasn't already been locked onto
						// TODO max number of locks = ceil(health / missile damage)
						uint lockedCounter = 0;
						bool currentlyLockingOn = false;
						for (auto l = mLocks.begin(); l != mLocks.end(); ++l)
						{
							if ((*l).target.lock() == ent)
							{
								lockedCounter++;
								if ((*l).lockProgress < 1.0f)
								{
									currentlyLockingOn = true;
									break;
								}
							}
						}

						// Find a free lock
						// TODO magic number
						uint maxLocks = (uint)ceil((float)ent->getHealth() / 100.0f);
						if (lockedCounter < maxLocks && !currentlyLockingOn)
						{
							for (auto i = mLocks.begin(); i != mLocks.end(); ++i)
							{
								// Lock on!!
								if (!(*i).hasLock)
								{
									(*i).hasLock = true;
									(*i).lockProgress = 0.0f;
									(*i).target = ent;
									(*i).fired = false;
									break;
								}
							}
						}
					}
				}
			}
		}
	}
}

void PlayerEntity::render(sf::RenderWindow& window)
{
	mShipSprite.setPosition(mPosition);
	window.draw(mShipSprite);

	mTargetSprite.setPosition(mPosition + Vec2(0.0f, -250.0f));
	window.draw(mTargetSprite);

	// Draw shields
	for (auto i = mShields.begin(); i != mShields.end(); ++i)
	{
		if ((*i).visibility > EPSILON)
		{
			mShieldSprite.setPosition(mPosition);
			mShieldSprite.setRotation((*i).rotation);
			mShieldSprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)(255.0f * (*i).visibility)));
			window.draw(mShieldSprite);
		}
	}
}

void PlayerEntity::onCollision(shared_ptr<Entity> other)
{
	shared_ptr<BulletEntity> bullet = dynamic_pointer_cast<BulletEntity>(other);
	if (bullet != shared_ptr<BulletEntity>())
	{
		// Take damage
		damage(other->getPosition() - mPosition, 100);

		// Despawn the bullet
		bullet->despawn();
	}
}

sf::Sprite& PlayerEntity::getSprite()
{
	return mShipSprite;
}