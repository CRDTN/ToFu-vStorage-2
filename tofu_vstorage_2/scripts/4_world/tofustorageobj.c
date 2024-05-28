class tofuvStorageObj
{
	string itemName = "";
	string itemType = "";
	int itemRow = 0;
	int itemCol = 0;
	bool itemFliped = false;
	int itemQuantity = 0;
	int itemAmmoQuantity = 0;
	float itemHealth = 0;
	int itemLiquidType = 0;
	int itemFoodstage = 0;
	float itemTemp = 0;
	float itemWetness = 0;
	int itemInventoryType = 0;
	int itemIdx = 0;
	int itemSlotId = 0;
	int itemTimeStored = 0;
	string itemID = "";
	string itemWpnBarrelInfo = "";
	string itemWpnInternalMagInfo = "";
	string itemMagInfo = "";

	autoptr array<string> itemMagInhalt = {};

	string itemUnusedString1 = "";
	string itemUnusedString2 = "";
	string itemUnusedString3 = "";
	string itemUnusedString4 = "";
	string itemUnusedString5 = "";
	string itemUnusedString6 = "";

	int itemEnergy = 0;
	int itemUnusedInt2 = 0;
	int itemUnusedInt3 = 0;
	int itemUnusedInt4 = 0;
	int itemUnusedInt5 = 0;
	int itemUnusedInt6 = 0;

	bool itemHasEnergy = false;
	bool itemUnusedBool2 = false;
	bool itemUnusedBool3 = false;
	bool itemUnusedBool4 = false;
	bool itemUnusedBool5 = false;
	bool itemUnusedBool6 = false;

	ref array<ref tofuvStorageObj> itemChildren = {};
};

class tofuvStorageContainer
{
	string persistentId = "";
	ref array<ref tofuvStorageObj> storedItems = {};
};

tofuvStorageObj vstore(EntityAI item_in_storage, out bool m_didVStorage = false)
{
	autoptr tofuvStorageObj itemObj = new tofuvStorageObj();
	InventoryLocation item_in_storage_location = new InventoryLocation;
	item_in_storage.GetInventory().GetCurrentInventoryLocation(item_in_storage_location);
	ItemBase item_to_check = ItemBase.Cast(item_in_storage);

	itemObj.itemName = item_in_storage.GetType();
	itemObj.itemRow = item_in_storage_location.GetRow();
	itemObj.itemCol = item_in_storage_location.GetCol();
	itemObj.itemFliped = item_in_storage_location.GetFlip();
	itemObj.itemIdx = item_in_storage_location.GetIdx();
	itemObj.itemSlotId = item_in_storage_location.GetSlot();
	itemObj.itemInventoryType = item_in_storage_location.GetType();

	if (item_to_check.HasQuantity())
	{
		itemObj.itemQuantity = item_to_check.GetQuantity();
		if (item_to_check.IsLiquidContainer())
			itemObj.itemLiquidType = item_to_check.GetLiquidType();
	}
	else
	{
		itemObj.itemQuantity = 0;
		itemObj.itemLiquidType = 0;
	}

	if (item_in_storage.m_EM)
	{
		itemObj.itemEnergy = Math.Ceil(item_in_storage.m_EM.GetEnergy());
		itemObj.itemHasEnergy = true;
	}

	Magazine magazine_check = Magazine.Cast(item_in_storage);
	Ammunition_Base ammo_check = Ammunition_Base.Cast(item_in_storage);

	if (item_in_storage.IsMagazine() && !(ammo_check && ammo_check.IsAmmoPile()))
	{
		itemObj.itemType = "magazine";
		itemObj.itemAmmoQuantity = magazine_check.GetAmmoCount();

		for (int f = 0; f < magazine_check.GetAmmoCount(); f++)
		{
			float dmg;
			string class_name;
			magazine_check.GetCartridgeAtIndex(f, dmg, class_name);
			string ma_temp = f.ToString() + "," + class_name;

			itemObj.itemMagInhalt.Insert(ma_temp);
		}
		// Print("MAGAZINE AMMO: "+magazine_check.GetAmmoCount());
	}

	array<string> a_itemWpnBarrelInfo = new array<string>;
	array<string> a_itemWpnInternalMagInfo = new array<string>;

	if (item_in_storage.IsWeapon())
	{
		Weapon_Base wpn = Weapon_Base.Cast(item_in_storage);
		float damage = 0.0;
		string type;
		string itemWpnBarrelInfo;
		string itemWpnInternalMagInfo;

		for (int mi = 0; mi < wpn.GetMuzzleCount(); ++mi)
		{
			if (!wpn.IsChamberEmpty(mi))
			{
				if (wpn.GetCartridgeInfo(mi, damage, type))
				{
					// Print ("[Im Lauf] " +mi+" "+damage+" "+type);
					string bi_temp = "" + mi + "," + type;
					a_itemWpnBarrelInfo.Insert(bi_temp);
					// PushCartridgeToChamber(mi, damage, type);
				}
			}

			for (int ci = 0; ci < wpn.GetInternalMagazineCartridgeCount(mi); ++ci)
			{
				if (wpn.GetInternalMagazineCartridgeInfo(mi, ci, damage, type))
				{
					// Print ("[In internen Mag] " +mi+" "+ci+" "+damage+" "+type);
					string ci_temp = "" + mi + "," + type;
					a_itemWpnInternalMagInfo.Insert(ci_temp);
					// PushCartridgeToInternalMagazine(mi, damage, type);
				}
			}
		}

		if (a_itemWpnBarrelInfo.Count() > 0)
		{
			for (int bi = 0; bi < a_itemWpnBarrelInfo.Count(); bi++)
			{
				itemWpnBarrelInfo += a_itemWpnBarrelInfo.Get(bi) + "|";
			}
			itemObj.itemWpnBarrelInfo = itemWpnBarrelInfo;
			// Print(itemWpnBarrelInfo);
		}

		if (a_itemWpnInternalMagInfo.Count() > 0)
		{
			for (int im = 0; im < a_itemWpnInternalMagInfo.Count(); im++)
			{
				itemWpnInternalMagInfo += a_itemWpnInternalMagInfo.Get(im) + "|";
			}
			itemObj.itemWpnInternalMagInfo = itemWpnInternalMagInfo;
			// Print(itemWpnInternalMagInfo);
		}
	}

	if (ammo_check && ammo_check.IsAmmoPile())
	{
		itemObj.itemAmmoQuantity = ammo_check.GetAmmoCount();
	}

	itemObj.itemHealth = item_in_storage.GetHealth();

	if (item_in_storage.IsFood())
	{
		Edible_Base item_to_check_food = Edible_Base.Cast(item_in_storage);
		if (item_to_check_food)
		{
			itemObj.itemFoodstage = item_to_check_food.GetFoodStageType();
		}
	}

	itemObj.itemTemp = item_to_check.GetTemperature();
	itemObj.itemWetness = item_to_check.GetWet();

	array<EntityAI> items = new array<EntityAI>;
	item_in_storage.GetInventory().EnumerateInventory(InventoryTraversalType.LEVELORDER, items);
	int count = items.Count();
	for (int i = 0; i < count; i++)
	{
		EntityAI item_in_storage_child = items.Get(i);
		if (item_in_storage_child)
		{
			itemObj.itemChildren.Insert(vstore(item_in_storage_child));
			m_didVStorage = true;
		}
	};

	/*
  Print("------- NEW ITEM ---------");
  Print(itemObj.itemName);
  Print(itemObj.itemType);
  Print(itemObj.itemRow);
  Print(itemObj.itemCol);
  Print(itemObj.itemFliped);
  Print(itemObj.itemQuantity);
  Print(itemObj.itemAmmoQuantity);
  Print(itemObj.itemHealth);
  Print(itemObj.itemLiquidType);
  Print(itemObj.itemFoodstage);
  Print(itemObj.itemTemp);
  Print(itemObj.itemWetness);
  Print(itemObj.itemInventoryType);
  Print(itemObj.itemIdx);
  Print(itemObj.itemSlotId);
  Print(itemObj.itemChildren);
  Print("----------------");
  */

	return itemObj;
};

ItemBase SSURV_CreateSerializedItem(ref tofuvStorageObj itemTemplate, EntityAI parent = NULL, vector pos = "0 0 0")
{

	bool itemFailed = false;
	bool indexError = false;

	ItemBase new_item;

	// ! WEAPON

	if (parent == NULL)
	{
		new_item = ItemBase.Cast(GetGame().CreateObject(itemTemplate.itemName, pos));
	}
	else
	{
		// ! SPAWNING A MAGAZINE INTO WEAPON IF POSSIBLE
		if (itemTemplate.itemType == "magazine" && parent.IsWeapon())
		{

			Weapon_Base weapon = Weapon_Base.Cast(parent);
			weapon.SpawnAmmo(itemTemplate.itemName, 0);

			array<EntityAI> items = new array<EntityAI>;
			weapon.GetInventory().EnumerateInventory(InventoryTraversalType.LEVELORDER, items);

			int count = items.Count();

			for (int i = 0; i < count; i++)
			{
				ItemBase item_in_storage = ItemBase.Cast(items.Get(i));
				if (item_in_storage)
				{
					Magazine magazine_check = Magazine.Cast(item_in_storage);
					Ammunition_Base ammo_check = Ammunition_Base.Cast(item_in_storage);
					if (item_in_storage.IsMagazine() && !(ammo_check && ammo_check.IsAmmoPile()))
					{
						magazine_check.ServerSetAmmoCount(0);
						array<string> itemMagInhalt3 = itemTemplate.itemMagInhalt;
						for (int ae = 0; ae < itemMagInhalt3.Count(); ae++)
						{
							string part_itemMagInfo3 = itemMagInhalt3.Get(ae);
							if (part_itemMagInfo3 != "")
							{
								TStringArray a_part_itemMagInfo3 = new TStringArray;
								part_itemMagInfo3.Split(",", a_part_itemMagInfo3);

								if (magazine_check.ServerStoreCartridge(0.0, a_part_itemMagInfo3[1]))
								{
									// Print("Stored Ammo in Mag");
								}
							}
						}
						magazine_check.SetSynchDirty();
						ItemBase mag_stats = ItemBase.Cast(item_in_storage);
						mag_stats.SetHealth(itemTemplate.itemHealth);
						mag_stats.SetTemperature(itemTemplate.itemTemp);
						mag_stats.SetWet(itemTemplate.itemWetness);
					}
				}
			}
		}
		else
		{
			if (itemTemplate.itemType == "magazine")
			{
				if (parent == NULL)
				{
					new_item = ItemBase.Cast(GetGame().CreateObject(itemTemplate.itemName, pos));
				}
				else
				{
					new_item = ItemBase.Cast(parent.GetInventory().CreateAttachmentEx(itemTemplate.itemName, itemTemplate.itemSlotId));
				}

				Magazine magazine_check3 = Magazine.Cast(new_item);
				Ammunition_Base ammo_check2 = Ammunition_Base.Cast(new_item);
				if (new_item && new_item.IsMagazine() && !(ammo_check2 && ammo_check2.IsAmmoPile()))
				{
					magazine_check3.ServerSetAmmoCount(0);

					array<string> itemMagInhalt2 = itemTemplate.itemMagInhalt;

					for (int ad = 0; ad < itemMagInhalt2.Count(); ad++)
					{
						string part_itemMagInfo2 = itemMagInhalt2.Get(ad);
						if (part_itemMagInfo2 != "")
						{
							TStringArray a_part_itemMagInfo2 = new TStringArray;
							part_itemMagInfo2.Split(",", a_part_itemMagInfo2);

							if (magazine_check3.ServerStoreCartridge(0.0, a_part_itemMagInfo2[1]))
							{
								// Print("Stored Ammo in Mag");
							}
						}
					}
					magazine_check3.SetSynchDirty();
					ItemBase mag_stats2 = ItemBase.Cast(new_item);
					mag_stats2.SetHealth(itemTemplate.itemHealth);
					mag_stats2.SetTemperature(itemTemplate.itemTemp);
					mag_stats2.SetWet(itemTemplate.itemWetness);
				}
			}
			else
			{
				new_item = ItemBase.Cast(parent.GetInventory().CreateAttachmentEx(itemTemplate.itemName, itemTemplate.itemSlotId));
			}
		}
	}

	if (new_item)
	{
		// ! WEAPON

		if (new_item.IsWeapon())
		{
			bool didChamberAction = false;
			Weapon_Base wpn = Weapon_Base.Cast(new_item);

			string itemWpnBarrelInfo = itemTemplate.itemWpnBarrelInfo;

			TStringArray a_itemWpnBarrelInfo = new TStringArray;
			itemWpnBarrelInfo.Split("|", a_itemWpnBarrelInfo);
			for (int ab = 0; ab < a_itemWpnBarrelInfo.Count(); ab++)
			{
				string part_itemWpnBarrelInfo = a_itemWpnBarrelInfo.Get(ab);
				if (part_itemWpnBarrelInfo != "")
				{
					TStringArray a_part_itemWpnBarrelInfo = new TStringArray;
					part_itemWpnBarrelInfo.Split(",", a_part_itemWpnBarrelInfo);
					if (wpn.PushCartridgeToChamber(a_part_itemWpnBarrelInfo[0].ToInt(), 0.0, a_part_itemWpnBarrelInfo[1]))
					{
						// Print("Loaded Ammo to Chamber");
						didChamberAction = true;
					}
					else
					{
						// Print("NOT Loaded Ammo to Chamber");
					}
				}
			}

			string itemWpnInternalMagInfo = itemTemplate.itemWpnInternalMagInfo;

			TStringArray a_itemWpnInternalMagInfo = new TStringArray;
			itemWpnInternalMagInfo.Split("|", a_itemWpnInternalMagInfo);
			for (int ib = 0; ib < a_itemWpnInternalMagInfo.Count(); ib++)
			{
				string part_itemWpnInternalMagInfo = a_itemWpnInternalMagInfo.Get(ib);
				if (part_itemWpnInternalMagInfo != "")
				{
					TStringArray a_part_itemWpnInternalMagInfo = new TStringArray;
					part_itemWpnInternalMagInfo.Split(",", a_part_itemWpnInternalMagInfo);
					if (wpn.PushCartridgeToInternalMagazine(a_part_itemWpnInternalMagInfo[0].ToInt(), 0.0, a_part_itemWpnInternalMagInfo[1]))
					{
						// Print("Loaded Ammo to intenal Mag");
						didChamberAction = true;
					}
					else
					{
						// Print("NOT Loaded Ammo to intenal Mag");
					}
				}
			}

			if (didChamberAction)
			{
				if (g_Game.IsServer())
					wpn.RandomizeFSMState();

				wpn.Synchronize();

				/*
				RandomizeFSMState();
				Synchronize();
				*/
			}

			// PushCartridgeToChamber(int muzzleIndex, float ammoDamage, string ammoTypeName);
		}

		// ! QUANTITY

		if (new_item.HasQuantity())
		{
			new_item.SetQuantity(itemTemplate.itemQuantity);
		}

		// ! LIQUID

		new_item.SetLiquidType(itemTemplate.itemLiquidType);

		if (itemTemplate.itemLiquidType >= 1 && itemTemplate.itemLiquidType <= 128)
		{
			BloodContainerBase bloodBag = BloodContainerBase.Cast(new_item);
			bloodBag.SetBloodTypeVisible(true);
		}

		// ! ENERGY

		if (itemTemplate.itemHasEnergy)
		{
			new_item.m_EM.SetEnergy(itemTemplate.itemEnergy);
		}

		// ! AMMO

		if (new_item.IsAmmoPile())
		{
			Magazine mgzn = Magazine.Cast(new_item);
			mgzn.ServerSetAmmoCount(itemTemplate.itemAmmoQuantity);
		}

		// ! MAGAZINE

		if (itemTemplate.itemType == "magazine")
		{
			bool is_single_or_server = !GetGame().IsMultiplayer() || GetGame().IsServer();
			if (is_single_or_server)
			{
				Magazine magazine_check2 = Magazine.Cast(new_item);
				magazine_check2.ServerSetAmmoCount(0);

				array<string> itemMagInhalt = itemTemplate.itemMagInhalt;

				for (int ac = 0; ac < itemMagInhalt.Count(); ac++)
				{
					string part_itemMagInfo = itemMagInhalt.Get(ac);
					if (part_itemMagInfo != "")
					{
						TStringArray a_part_itemMagInfo = new TStringArray;
						part_itemMagInfo.Split(",", a_part_itemMagInfo);

						if (magazine_check2.ServerStoreCartridge(0.0, a_part_itemMagInfo[1]))
						{
							// Print("Stored Ammo in Mag");
						}
					}
				}
				magazine_check2.SetSynchDirty();
			}
		}

		// ! FOOD

		if (new_item.IsFood())
		{
			Edible_Base new_item_food = Edible_Base.Cast(new_item);
			new_item_food.ChangeFoodStage(itemTemplate.itemFoodstage);
		}

		new_item.SetHealth(itemTemplate.itemHealth);

		new_item.SetTemperature(itemTemplate.itemTemp);

		new_item.SetWet(itemTemplate.itemWetness);

		foreach (ref tofuvStorageObj childitem : itemTemplate.itemChildren)
		{
			SSURV_CreateSerializedItem(childitem, new_item, pos);
		}
	}
	else
	{
		DebugUtils.Log("[tofustorageobj]::Failed to create item: " + itemTemplate.itemName);
	}

	return new_item;
}
