class VST_Config
{	
		
    static const string FULLPATH = "$profile:CRDTN/Storage/VST_Config.json";
	
	protected int script_logging = 0;
	
	protected int auto_close_random_seconds_min = 120;
	protected int auto_close_random_seconds_max = 240;

	protected ref array<string> Blacklist;
	protected ref array<string> Admins;
	
	void VST_Config()
	{
		if (GetGame().IsServer())
		{			
		
			if (!FileExist("$profile:CRDTN/Storage"))
			{
				MakeDirectory("$profile:CRDTN/Storage");
			}

			if (!FileExist(FULLPATH))
			{
				Default();
				return; 
			}

			Load();
		}
    }

	void Load()
    {
		JsonFileLoader<VST_Config>.JsonLoadFile(FULLPATH, this);
		Save();
    }

	
	protected void Save()
    {
        JsonFileLoader<VST_Config>.JsonSaveFile(FULLPATH, this);
    }

	protected void Default()
    {
		
		script_logging = 0;
        
		auto_close_random_seconds_min = 120;
		auto_close_random_seconds_max = 240;
		
		Blacklist = new array<string>;
		Blacklist.Insert("WrittenNote");
		Blacklist.Insert("VehicleKeyBase");
		Blacklist.Insert("MCK_CarKey_Base");
		Blacklist.Insert("ChickenBreastMeat");
		Blacklist.Insert("GoatSteakMeat");
		Blacklist.Insert("SheepSteakMeat");
		Blacklist.Insert("PigSteakMeat");
		Blacklist.Insert("WolfSteakMeat");
		Blacklist.Insert("BearSteakMeat");
		Blacklist.Insert("DeerSteakMeat");
		Blacklist.Insert("CowSteakMeat");
		Blacklist.Insert("Lard");
		Blacklist.Insert("Carp");
		Blacklist.Insert("Mackerel");
		Blacklist.Insert("tofu_vstorage_barrel");
		
		Admins = new array<string>;
		Admins.Insert("12345678901234567");
		
		Save();
	}

	array<string> Get_Blacklist()
	{
		return Blacklist;
	}
	
	array<string> Get_Admins()
	{
		return Admins;
	}

	int Get_script_logging()
	{
		return script_logging;
	}
	
	int Get_auto_close_random_seconds_min()
	{
		return auto_close_random_seconds_min;
	}

	int Get_auto_close_random_seconds_max()
	{
		return auto_close_random_seconds_max;
	}

	
};