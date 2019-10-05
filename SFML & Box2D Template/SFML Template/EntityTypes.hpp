namespace CPPong {

	enum EntityType
	{
		ET_Player	= 1<<0,
		ET_Ball		= 1<<1,
		ET_Wall		= 1<<2,
		ET_Goal		= 1<<3,
		ET_Any		= (1<<4)-1
	};

}