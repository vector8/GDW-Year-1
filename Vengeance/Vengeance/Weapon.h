#pragma once

class Weapon
{
private:
	int magAmmo, reserveAmmo, magCapacity, maxAmmo, range, damage;

	bool melee;
public:
	Weapon(bool isMelee, int damage, int range, int magCapacity = 0, int maxAmmo = 0);
	~Weapon();

	int getDamage();
	int getRange();

	int getMagazineCapacity();
	int getMaxAmmo();

	bool isMelee();

	int getReserveAmmo();
	int getMagazineAmmo();
	int getTotalAmmo();

	void setMagazineAmmo(int a);
	void setReserveAmmo(int a);
	void addToReserveAmmo(int a);
	void subtractFromReserveAmmo(int a);

	// doesnt actually damage, just reduces current mag ammo
	void fire();

	void reload();
};