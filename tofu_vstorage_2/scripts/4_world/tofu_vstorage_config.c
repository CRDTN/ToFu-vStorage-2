class ToFu_Vstorage_Config
{		
	protected TStringArray m_BlacklistItems;
	protected bool m_DoBackups;
			
	void ToFu_Vstorage_Config(bool backup, TStringArray blacklistitems)
	{
		m_DoBackups = backup;
		m_BlacklistItems = blacklistitems;
	}
	
	static ToFu_Vstorage_Config LoadToFuCrateConfig(bool backup, TStringArray blacklistitems)
	{
		
		ToFu_Vstorage_Config crateConfig = new ToFu_Vstorage_Config(backup, blacklistitems);
		
		if (!FileExist("$profile:CRDTN/Storage" + "/"))
        {
			MakeDirectory("$profile:CRDTN/Storage" + "/");
		}
		
		
		if (FileExist("$profile:CRDTN/Storage/tofu_vstorage_config.json")) {
			JsonFileLoader<ToFu_Vstorage_Config>.JsonLoadFile("$profile:CRDTN/Storage/tofu_vstorage_config.json", crateConfig);
		} else {
			crateConfig = CreateDefaultCrateConfig(backup,blacklistitems);
		}
		
		return crateConfig;
	}
			
		
	protected static ToFu_Vstorage_Config CreateDefaultCrateConfig(bool backup, TStringArray blacklistitems )
	{
		ToFu_Vstorage_Config crateConfig = new ToFu_Vstorage_Config(backup, blacklistitems);
		SaveCrateConfig(crateConfig, backup, blacklistitems);
		return crateConfig;
	}
		
	
	static void SaveCrateConfig(ToFu_Vstorage_Config crateConfig, bool backup, TStringArray blacklistitems) {
		if(!crateConfig)
		{
			return;
		}
		
		JsonFileLoader<ToFu_Vstorage_Config>.JsonSaveFile("$profile:CRDTN/Storage/tofu_vstorage_config.json", crateConfig);
	}
		
	TStringArray GetBlacklistItems() {
		return m_BlacklistItems;
	}
	
	
	
};