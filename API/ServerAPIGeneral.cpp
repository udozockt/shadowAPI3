#include "stdafx.h"
#include "ServerAPIGeneral.h"
#include "NativeGeneral.h"

SerializeFunction(ServerAPIGeneral::Useless)
{
	int i = in.ReadInteger();
	out.WriteInteger(i * 2);
}

SerializeFunction(ServerAPIGeneral::Useless1)
{
	out.WriteInteger(18);
}

SerializeFunction(ServerAPIGeneral::GetWeatherId)
{
	out.WriteInteger(NativeGeneral::GetWeatherId());
}

SerializeFunction(ServerAPIGeneral::SetKey)
{
	int key = in.ReadInteger();
	bool pressed = in.ReadBoolean();
	NativeGeneral::SetKey(key, pressed);
}