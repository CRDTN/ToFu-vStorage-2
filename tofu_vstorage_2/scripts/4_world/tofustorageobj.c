class tofuvStorageObj 
{
	string itemName               = "";
	string itemType               = "";
	int    itemRow                = 0;
	int    itemCol                = 0;
	bool   itemFliped             = false;
	int    itemQuantity           = 0;
	int    itemAmmoQuantity       = 0;
	float  itemHealth             = 0;
	int    itemLiquidType         = 0;
	int    itemFoodstage          = 0;
	float  itemTemp               = 0;
	float  itemWetness            = 0;
	int    itemInventoryType      = 0;
	int    itemIdx                = 0;
	int    itemSlotId             = 0;
	int    itemTimeStored         = 0;
	string itemID                 = "";
	string itemWpnBarrelInfo      = "";
	string itemWpnInternalMagInfo = "";
	string itemMagInfo            = "";
	
	autoptr array<string> itemMagInhalt = {};
	
	string itemUnusedString1 = "";
	string itemUnusedString2 = "";
	string itemUnusedString3 = "";
	string itemUnusedString4 = "";
	string itemUnusedString5 = "";
	string itemUnusedString6 = "";
	
	int itemEnergy     = 0;
	int itemUnusedInt2 = 0;
	int itemUnusedInt3 = 0;
	int itemUnusedInt4 = 0;
	int itemUnusedInt5 = 0;
	int itemUnusedInt6 = 0;
	
	bool itemHasEnergy   = false;
	bool itemUnusedBool2 = false;
	bool itemUnusedBool3 = false;
	bool itemUnusedBool4 = false;
	bool itemUnusedBool5 = false;
	bool itemUnusedBool6 = false;
	
	ref array<ref tofuvStorageObj> itemChildren = {};
};

class tofuvStorageContainer 
{
	string persistentId                           = "";
	ref    array<ref tofuvStorageObj> storedItems = {};
};

tofuvStorageObj vstore(EntityAI item_in_storage, out bool m_didVStorage = false)
{
	autoptr           tofuvStorageObj itemObj  = new tofuvStorageObj();
	InventoryLocation item_in_storage_location = new InventoryLocation;
	item_in_storage.GetInventory().GetCurrentInventoryLocation( item_in_storage_location );
	ItemBase item_to_check = ItemBase.Cast(item_in_storage);
	
	itemObj.itemName          = item_in_storage.GetType();
	itemObj.itemRow           = item_in_storage_location.GetRow();
	itemObj.itemCol           = item_in_storage_location.GetCol();
	itemObj.itemFliped        = item_in_storage_location.GetFlip();
	itemObj.itemIdx           = item_in_storage_location.GetIdx();
	itemObj.itemSlotId        = item_in_storage_location.GetSlot();
	itemObj.itemInventoryType = item_in_storage_location.GetType();
	
	if(item_to_check.HasQuantity())
	{
		itemObj.itemQuantity = item_to_check.GetQuantity();
		if(item_to_check.IsLiquidContainer())
			itemObj.itemLiquidType = item_to_check.GetLiquidType();
	} else
	{
		itemObj.itemQuantity   = 0;
		itemObj.itemLiquidType = 0;
	}
	
	if(item_in_storage.m_EM)
	{
		itemObj.itemEnergy    = Math.Ceil(item_in_storage.m_EM.GetEnergy());
		itemObj.itemHasEnergy = true;
	}
	
	Magazine        magazine_check = Magazine.Cast(item_in_storage);
	Ammunition_Base ammo_check     = Ammunition_Base.Cast(item_in_storage);

	if(item_in_storage.IsMagazine() && !(ammo_check && ammo_check.IsAmmoPile()))
	{
		itemObj.itemType         = "magazine";
		itemObj.itemAmmoQuantity = magazine_check.GetAmmoCount();
		
		for (int f = 0; f < magazine_check.GetAmmoCount(); f++)
		{
			float dmg;
			string class_name;
			magazine_check.GetCartridgeAtIndex(f, dmg, class_name);
			string ma_temp = f.ToString() + "," + class_name;
			
			itemObj.itemMagInhalt.Insert(ma_temp);
		}
		  //Print("MAGAZINE AMMO: "+magazine_check.GetAmmoCount());
	}
	
	array<string> a_itemWpnBarrelInfo      = new array<string>;
	array<string> a_itemWpnInternalMagInfo = new array<string>;
			
	if(item_in_storage.IsWeapon())
	{
		Weapon_Base wpn    = Weapon_Base.Cast(item_in_storage);
		float       damage = 0.0;
		string type;
		string itemWpnBarrelInfo;
		string itemWpnInternalMagInfo;
		
		for (int mi = 0; mi < wpn.GetMuzzleCount(); ++mi)
		{
			if (!wpn.IsChamberEmpty(mi))
			{
				if (wpn.GetCartridgeInfo(mi, damage, type))
				{
					  //Print ("[Im Lauf] " +mi+" "+damage+" "+type);
					string bi_temp = ""+mi+","+type;
					a_itemWpnBarrelInfo.Insert(bi_temp);
					  //PushCartridgeToChamber(mi, damage, type);
				}
			}
			
			for (int ci = 0; ci < wpn.GetInternalMagazineCartridgeCount(mi); ++ci)
			{
				if (wpn.GetInternalMagazineCartridgeInfo(mi, ci, damage, type))
				{
					  //Print ("[In internen Mag] " +mi+" "+ci+" "+damage+" "+type);
					string ci_temp = ""+mi+","+type;
					a_itemWpnInternalMagInfo.Insert(ci_temp);
					  //PushCartridgeToInternalMagazine(mi, damage, type);
				}
			}
		}
		
		if(a_itemWpnBarrelInfo.Count() > 0)
		{
			for (int bi = 0; bi < a_itemWpnBarrelInfo.Count(); bi++)
			{
				itemWpnBarrelInfo += a_itemWpnBarrelInfo.Get(bi)+"|";
			}
			itemObj.itemWpnBarrelInfo = itemWpnBarrelInfo;
			  //Print(itemWpnBarrelInfo);
		}
		
		if(a_itemWpnInternalMagInfo.Count() > 0)
		{
			for (int im = 0; im < a_itemWpnInternalMagInfo.Count(); im++)
			{
				itemWpnInternalMagInfo += a_itemWpnInternalMagInfo.Get(im)+"|";
			}
			itemObj.itemWpnInternalMagInfo = itemWpnInternalMagInfo;
			  //Print(itemWpnInternalMagInfo);
		}
	}
	
	if(ammo_check && ammo_check.IsAmmoPile())
	{
		itemObj.itemAmmoQuantity = ammo_check.GetAmmoCount();
	}
	
	itemObj.itemHealth = item_in_storage.GetHealthLevel();
					
	if(item_in_storage.IsFood())
	{
		Edible_Base item_to_check_food = Edible_Base.Cast(item_in_storage);
		if(item_to_check_food)
		{
			itemObj.itemFoodstage = item_to_check_food.GetFoodStageType();
		}
	}
	
	itemObj.itemTemp    = item_to_check.GetTemperature();
	itemObj.itemWetness = item_to_check.GetWet();
	
	
	array<EntityAI> items = new array<EntityAI>;
	item_in_storage.GetInventory().EnumerateInventory(InventoryTraversalType.LEVELORDER, items);
	int count = items.Count();
	for (int i = 0; i < count; i++)
	{
		EntityAI item_in_storage_child = items.Get(i);
		if (item_in_storage_child) {
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
