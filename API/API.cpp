// API.cpp: Definiert die exportierten Funktionen für die DLL-Anwendung.
//

#include "stdafx.h"
#include "API.h"
#include "SerializeableQueue.h"
#include <Psapi.h>

#define CheckConnection(ret) if(g_client == nullptr) return ret

DllPublic int API::Initialize(const wchar_t * path, const wchar_t * file)
{
	if (g_client != nullptr)
		return -1;

	HWND window = FindWindow(0, L"GTA:SA:MP");
	if (!window)
		return -2;

	DWORD processId;
	GetWindowThreadProcessId(window, &processId);
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, 0, processId);

	if (!process)
		return -3;

	// Check if API is already injected
	std::wstring apiName = std::wstring(file) + L".dll";
	HMODULE modules[1024] = {};
	DWORD sizeNeeded = 0;
	if (EnumProcessModules(process, modules, sizeof(modules), &sizeNeeded))
	{
		for (size_t i = 0; i < (sizeNeeded / sizeof(HMODULE)); i++)
		{
			wchar_t moduleName[MAX_PATH + 1];
			if (GetModuleFileNameEx(process, modules[i], moduleName, MAX_PATH))
			{
				std::wstring name = moduleName;
				auto fileName = name.substr(name.find_last_of(L'\\') + 1);
				if (fileName == apiName)
				{
					CloseHandle(process);

					g_client = std::make_shared<Client>();
					return 0;
				}
			}
		}
	}

	int pathLength = wcslen(path);
	HMODULE module = GetModuleHandle(L"kernel32.dll");
	LPTHREAD_START_ROUTINE routine = (LPTHREAD_START_ROUTINE)GetProcAddress(module, "LoadLibraryW");

	void *pathMemory = VirtualAllocEx(process, 0, (MAX_PATH * 2) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!pathMemory)
	{
		CloseHandle(process);
		return GetLastError();
	}

	WriteProcessMemory(process, pathMemory, path, pathLength * 2, 0);
	HANDLE thread = CreateRemoteThread(process, 0, 0, routine, pathMemory, 0, 0);
	WaitForSingleObject(thread, INFINITE);

	VirtualFreeEx(process, pathMemory, (MAX_PATH * 2) + 1, MEM_RELEASE);
	CloseHandle(process);

	// Now we create our client
	g_client = std::make_shared<Client>();

	return 0;
}

DllPublic void API::ResetInitialize()
{
	g_client = nullptr;
}

DllPublic int API::Other::GetWeatherID()
{
	return Useless(42);
}

DllPublic int API::Other::Useless(int useless)
{
	/*SerializeableQueue in;
	int integerIn = useless;
	float floatIn = 4.2f;
	bool booleanIn = true;
	std::wstring stringIn = L"Hello World! It's me :(";
	in.WriteInteger(integerIn);
	in.WriteFloat(floatIn);
	in.WriteBoolean(booleanIn);
	in.WriteString(stringIn);

	SerializeableQueue out(in);
	int integerOut = out.ReadInteger();
	float floatOut = out.ReadFloat();
	bool booleanOut = out.ReadBoolean();
	std::wstring stringOut = out.ReadString();

	return useless;*/

	auto message = g_client->CreateMessage(PacketIdentifier::Useless);
	message->GetInput().WriteInteger(useless);

	g_client->Write(message);

	return message->GetResponse()->ReadInteger();
}

DllPublic int API::Other::Useless1()
{
	return 0;
}

DllPublic int API::Overlay::Box::Create()
{
	return 0;
}

DllPublic void API::Overlay::Box::Delete(int id)
{
}

DllPublic void API::Overlay::Box::SetColor(int id, unsigned long color)
{
}

DllPublic void API::Overlay::Box::SetX(int id, int x)
{
}

DllPublic void API::Overlay::Box::SetY(int id, int y)
{
}

DllPublic void API::Overlay::Box::SetWidth(int id, int width)
{
}

DllPublic void API::Overlay::Box::SetHeight(int id, int height)
{
}

DllPublic void API::Overlay::Box::SetActive(int id, bool active)
{
}

DllPublic int API::Overlay::Text::Create()
{
	CheckConnection(-1);

	return g_client->FastReadInteger(PacketIdentifier::Overlay_Text_Create);
}

DllPublic void API::Overlay::Text::Delete(int id)
{
	CheckConnection();

	g_client->FastWriteInteger(PacketIdentifier::Overlay_Text_Delete, id);
}

DllPublic void API::Overlay::Text::SetColor(int id, unsigned long color)
{
	CheckConnection();

	auto message = g_client->CreateMessage(PacketIdentifier::Overlay_Text_SetColor);
	message->GetInput().WriteInteger(id);
	message->GetInput().WriteInteger(color);
	g_client->Write(message);
}

DllPublic void API::Overlay::Text::SetX(int id, int x)
{
	CheckConnection();

	auto message = g_client->CreateMessage(PacketIdentifier::Overlay_Text_SetX);
	message->GetInput().WriteInteger(id);
	message->GetInput().WriteInteger(x);
	g_client->Write(message);
}

DllPublic void API::Overlay::Text::SetY(int id, int y)
{
	CheckConnection();

	auto message = g_client->CreateMessage(PacketIdentifier::Overlay_Text_SetY);
	message->GetInput().WriteInteger(id);
	message->GetInput().WriteInteger(y);
	g_client->Write(message);
}

DllPublic void API::Overlay::Text::SetWidth(int id, int width)
{
}

DllPublic void API::Overlay::Text::SetHeight(int id, int height)
{
}

DllPublic void API::Overlay::Text::SetMaxWidth(int id, int maxWidth)
{
}

DllPublic void API::Overlay::Text::SetMaxHeight(int id, int maxHeight)
{
}

DllPublic void API::Overlay::Text::SetActive(int id, bool active)
{
}

DllPublic void API::Overlay::Text::SetText(int id, const wchar_t * text)
{
	CheckConnection();

	auto message = g_client->CreateMessage(PacketIdentifier::Overlay_Text_SetText);
	message->GetInput().WriteInteger(id);
	message->GetInput().WriteString(text);
	g_client->Write(message);
}

DllPublic void API::Overlay::Text::SetSize(int id, int size)
{
	CheckConnection();

	auto message = g_client->CreateMessage(PacketIdentifier::Overlay_Text_SetSize);
	message->GetInput().WriteInteger(id);
	message->GetInput().WriteInteger(size);
	g_client->Write(message);
}

DllPublic void API::Overlay::Text::SetUseMaxWidth(int id, bool state)
{
}

DllPublic void API::Overlay::Text::SetUseMaxHeight(int id, bool state)
{
}

DllPublic void API::Overlay::Text::GetTextExtent(int id, int & width, int & height)
{
	CheckConnection();

	auto message = g_client->CreateMessage(PacketIdentifier::Overlay_Text_GetTextExtent);
	message->GetInput().WriteInteger(id);
	g_client->Write(message);

	width = message->GetResponse()->ReadInteger();
	height = message->GetResponse()->ReadInteger();
}

DllPublic void API::SAMP::Chat::Send(const wchar_t * message)
{
	CheckConnection();

	g_client->FastWriteString(PacketIdentifier::SAMP_Chat_Send, message);
}

DllPublic void API::SAMP::Chat::AddMessage(const wchar_t * message)
{
	CheckConnection();

	g_client->FastWriteString(PacketIdentifier::SAMP_Chat_AddMessage1, message);
}

DllPublic void API::SAMP::Chat::AddMessage(const wchar_t * message, unsigned long color)
{
	CheckConnection();

	auto m = g_client->CreateMessage(PacketIdentifier::SAMP_Chat_AddMessage2);
	m->GetInput().WriteString(message);
	m->GetInput().WriteInteger(color);
	g_client->Write(m);
}

DllPublic void API::SAMP::Chat::GetText(wchar_t * text, int length)
{
	CheckConnection();

	auto t = g_client->FastReadString(PacketIdentifier::SAMP_Chat_GetText);
	wcscpy_s(text, length, t.c_str());
}

DllPublic void API::SAMP::Chat::SetText(const wchar_t * text)
{
	CheckConnection();

	g_client->FastWriteString(PacketIdentifier::SAMP_Chat_SetText, text);
}

DllPublic void API::SAMP::Chat::GetBufferMessage(int index, wchar_t * message, int length)
{
	CheckConnection();

	auto m = g_client->CreateMessage(PacketIdentifier::SAMP_Chat_GetBufferMessage);
	m->GetInput().WriteInteger(index);
	g_client->Write(m);

	auto text = m->GetResponse()->ReadString();
	wcscpy_s(message, length, text.c_str());
}

DllPublic void API::SAMP::Chat::AddBufferMessage(const wchar_t * message)
{
	CheckConnection();

	g_client->FastWriteString(PacketIdentifier::SAMP_Chat_AddBufferMessage, message);
}

DllPublic void API::SAMP::Chat::SetCursorPosition(int index)
{
	CheckConnection();

	g_client->FastWriteInteger(PacketIdentifier::SAMP_Chat_SetCursorPosition1, index);
}

DllPublic void API::SAMP::Chat::SetCursorPosition(int indexStart, int indexEnd)
{
	CheckConnection();

	auto message = g_client->CreateMessage(PacketIdentifier::SAMP_Chat_SetCursorPosition2);
	message->GetInput().WriteInteger(indexStart);
	message->GetInput().WriteInteger(indexEnd);
	g_client->Write(message);
}

DllPublic void API::SAMP::Chat::Toggle(bool state)
{
	CheckConnection();

	g_client->FastWriteBoolean(PacketIdentifier::SAMP_Chat_Toggle, state);
}

DllPublic void API::SAMP::Chat::Clear()
{
	CheckConnection();

	g_client->FastWriteVoid(PacketIdentifier::SAMP_Chat_Clear);
}

DllPublic bool API::SAMP::Chat::IsOpen()
{
	CheckConnection(false);

	return g_client->FastReadBoolean(PacketIdentifier::SAMP_Chat_IsOpen);
}

DllPublic void API::SAMP::Dialog::GetText(wchar_t * text, int length)
{
}

DllPublic void API::SAMP::Dialog::SetText(const wchar_t * text)
{
}

DllPublic void API::SAMP::Dialog::ClearText()
{
}

DllPublic void API::SAMP::Dialog::SetCursorPosition(int index)
{
}

DllPublic void API::SAMP::Dialog::SetCursorPosition(int indexStart, int indexEnd)
{
}

DllPublic int API::SAMP::Dialog::GetID()
{
	return 0;
}

DllPublic int API::SAMP::Dialog::GetStringCount()
{
	return 0;
}

DllPublic void API::SAMP::Dialog::GetStringByIndex(int index, wchar_t * text, int length)
{
}

DllPublic bool API::SAMP::Dialog::IsOpen()
{
	return false;
}

DllPublic void API::SAMP::Dialog::Close(int reason)
{
}

DllPublic void API::SAMP::Dialog::SelectIndex(int index)
{
}

DllPublic void API::SAMP::Dialog::BlockGetCaption(wchar_t * caption, int length)
{
}

DllPublic void API::SAMP::Dialog::BlockGetText(wchar_t * text, int length)
{
}

DllPublic void API::SAMP::Dialog::Block(int id, const wchar_t * text)
{
}

DllPublic bool API::SAMP::Dialog::BlockHasBlockedDialog()
{
	return false;
}

DllPublic bool API::SAMP::Dialog::BlockHasNeedBlocking()
{
	return false;
}

DllPublic void API::SAMP::Player::GetLocalName(wchar_t * name, int length)
{
}

DllPublic int API::SAMP::Player::GetLocalId()
{
	return 0;
}

DllPublic void API::SAMP::Player::GetNameById(int id, wchar_t * name, int length)
{
}

DllPublic int API::SAMP::Player::GetIdByName(const wchar_t * name)
{
	return 0;
}

DllPublic void API::SAMP::Player::GetFullName(const wchar_t * name, wchar_t * fullName, int length)
{
}

DllPublic void API::SAMP::Vehicle::GetNumberplate(wchar_t * numberplate, int length)
{
}

DllPublic void API::SAMP::Vehicle::ToggleSiren(bool state)
{
}

DllPublic int API::Player::GetHealth()
{
	CheckConnection(-1);

	return g_client->FastReadInteger(PacketIdentifier::Player_GetHealth);
}

DllPublic int API::Player::GetArmor()
{
	CheckConnection(-1);

	return g_client->FastReadInteger(PacketIdentifier::Player_GetArmor);
}

DllPublic void API::Player::GetCity(wchar_t * city, int length)
{
}

DllPublic void API::Player::GetDistrict(wchar_t * district, int length)
{
}

DllPublic void API::Player::GetPosition(float & x, float & y, float & z)
{
	CheckConnection();

	x = g_client->FastReadFloat(PacketIdentifier::Player_GetX);
	y = g_client->FastReadFloat(PacketIdentifier::Player_GetY);
	z = g_client->FastReadFloat(PacketIdentifier::Player_GetZ);
}

DllPublic float API::Player::GetYaw()
{
	CheckConnection(0.0f);

	return g_client->FastReadFloat(PacketIdentifier::Player_GetYaw);
}

DllPublic bool API::Player::InInterior()
{
	CheckConnection(false);

	return g_client->FastReadBoolean(PacketIdentifier::Player_InInterior);
}

DllPublic bool API::Player::InVehicle()
{
	CheckConnection(false);

	return g_client->FastReadBoolean(PacketIdentifier::Player_InVehicle);
}

DllPublic bool API::Player::IsDriver()
{
	CheckConnection(false);

	return g_client->FastReadBoolean(PacketIdentifier::Player_IsDriver);
}

DllPublic float API::Vehicle::GetSpeed()
{
	return 0.0f;
}

DllPublic float API::Vehicle::GetHealth()
{
	return 0.0f;
}

DllPublic int API::Vehicle::GetModelID()
{
	return 0;
}

DllPublic bool API::Vehicle::IsLightActive()
{
	return false;
}

DllPublic bool API::Vehicle::IsLocked()
{
	return false;
}

DllPublic bool API::Vehicle::IsEngineRunning()
{
	return false;
}

DllPublic bool API::Vehicle::UseHorn()
{
	return false;
}

DllPublic bool API::Vehicle::UseSiren()
{
	return false;
}