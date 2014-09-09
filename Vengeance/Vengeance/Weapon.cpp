#include "Weapon.h"
#include <assert.h>
#include <algorithm>
#include "SoundManager.h"

Weapon::Weapon(bool isMelee, int damage, int range, int magCapacity, int maxAmmo) : melee(isMelee), damage(damage), range(range), magCapacity(magCapacity), 
																					maxAmmo(maxAmmo), magAmmo(0), reserveAmmo(0)
{

}

Weapon::~Weapon()
{

}

int Weapon::getDamage()
{
	return this->damage;
}

int Weapon::getRange()
{
	return this->range;
}

int Weapon::getMagazineCapacity()
{
	return this->magCapacity;
}

int Weapon::getMaxAmmo()
{
	return this->maxAmmo;
}

bool Weapon::isMelee()
{
	return this->melee;
}

int Weapon::getReserveAmmo()
{
	assert(!this->isMelee());

	return this->reserveAmmo;
}

int Weapon::getMagazineAmmo()
{
	assert(!this->isMelee());

	return this->magAmmo;
}

int Weapon::getTotalAmmo()
{
	assert(!this->isMelee());

	return this->getReserveAmmo() + this->getMagazineAmmo();
}

void Weapon::setMagazineAmmo(int a)
{
	assert(!this->isMelee());

	assert(a >= 0);
	assert(a <= this->getMagazineCapacity());

	this->magAmmo = a;
}

void Weapon::setReserveAmmo(int a)
{
	assert(!this->isMelee());

	assert(a >= 0);

	this->reserveAmmo = std::min(a, this->getMaxAmmo() - this->getMagazineAmmo());
}

void Weapon::addToReserveAmmo(int a)
{
	assert(!this->isMelee());

	this->setReserveAmmo(this->getReserveAmmo() + a);
}

void Weapon::subtractFromReserveAmmo(int a)
{
	assert(!this->isMelee());

	this->addToReserveAmmo(-a);
}

void Weapon::fire()
{
	assert(!this->isMelee());
	assert(this->getMagazineAmmo() > 0);

	this->setMagazineAmmo(this->getMagazineAmmo() - 1);
}

void Weapon::reload()
{
	assert(!this->isMelee());

	int required = this->getMagazineCapacity() - this->getMagazineAmmo();

	required = std::min(required, this->getReserveAmmo());

	if(required > 0)
	{
		this->subtractFromReserveAmmo(required);
		this->setMagazineAmmo(this->getMagazineAmmo() + required);

		SoundManager::getInstance()->playSoundEffect(PISTOL_PICKUP);
	}
}