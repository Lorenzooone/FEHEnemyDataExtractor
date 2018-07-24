#include<iostream>
#include<string>
#include"HSDArc.h"
#include"enemyDataExtractor.h"
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #include <Windows.h>
#endif
using namespace std;

//Const declarations
const int Xor_Unknown[] = {
  0x64, 0xB7, 0x49, 0x16, 0xBD, 0x18, 0x3E, 0x42};
const int Xor_Unknown_Size = 8;
const int Xor_Stats[] = {
  0x32, 0xD6, 0xA0, 0x14, 0x5E, 0xA5, 0x66, 0x85,
  0xE5, 0xAE, 0x57, 0x64, 0x1A, 0x29, 0x59, 0x05};
const int Xor_Stats_Size = 10;
const int Xor_EnId[] = {
  0xD4, 0x41, 0x2F, 0x42};
const int Xor_Id_Size = 4;
const int Xor_EnWeap[] = {
  0xE4};
const int Xor_Weap_Size = 1;
const int Xor_EnTome[] = {
  0x81};
const int Xor_Tome_Size = 1;
const int Xor_EnMove[] = {
  0x0D};
const int Xor_Move_Size = 1;
const int Xor_EnSpawnable[] = {
  0xC4};
const int Xor_Spawnable_Size = 1;
const int Xor_EnSpecial[] = {
  0x6A};
const int Xor_Special_Size = 1;


const int statGrowths[][13] = {
    {6, 8, 9, 11, 13, 14, 16, 18, 19, 21, 23, 24, 26},
    {7, 8, 10, 12, 14, 15, 17, 19, 21, 23, 25, 26, 28},
    {7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31},
    {8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 31, 33},
    {8, 10, 13, 15, 17, 19, 22, 24, 26, 28, 30, 33, 35}
};

const unsigned char Skills[][20] = {"Default Weapon: ", "Default Assist: ", "Default Special: ", "Unknown: ", "Unknown: ", "Unknown: ", "Unlocked Weapon: ", "Unlocked Assist: ",
  "Unlocked Special: ", "Passive A: ", "Passive B: ", "Passive C: ", "Unknown: ", "Unknown: "};
const unsigned char Legendary[][6] = {"Fire", "Water", "Wind", "Earth"};
const unsigned char Weapons[][20] = {"Red Sword", "Blue Lance", "Green Axe", "Red Bow", "Blue Bow", "Green Bow", "Colorless Bow",  "Colorless Dagger", "Red Tome",
  "Blue Tome", "Green Tome", "Colorless Staff", "Red Breath", "Blue Breath", "Green Breath", "Colorless Breath"};
const unsigned char Tome_Elem[][8] = {"None", "Fire", "Thunder", "Wind", "Light", "Dark"};
const unsigned char Movement[][10] = {"Infantry", "Armored", "Cavalry", "Flying"};
const unsigned char Series[][105] = {"Heroes", "Shadow Dragon and the Blade of Light / Mystery of the Emblem / Shadow Dragon / New Mystery of the Emblem",
  "Gaiden / Echoes", "Genealogy of the Holy War", "Thracia 776", "The Binding Blade", "The Blazing Blade", "The Sacred Stones", "Path of Radiance", "Radiant Dawn", "Awakening", "Fates"};

//--------------------------------

stats_tuple GetLvl40Stats(stats_tuple lvl1, stats_tuple growths)
{
	stats_tuple stats=lvl1;
    stats.hp += statGrowths[4][growths.hp];
    stats.atk += statGrowths[4][growths.atk];
    stats.spd += statGrowths[4][growths.spd];
    stats.def += statGrowths[4][growths.def];
    stats.res += statGrowths[4][growths.res];
	return stats;
}

string actOnData(hsdarc_buffer buf, int num, const int Xor[], int XorSize, string (*a)(long long int ptr, char data[], const int Xor[], int XorSize))
{
	if(read_data_long(buf.data, buf.ptr_list[num], 8) == 0)
        return "None";
	return a(read_data_long(buf.data, buf.ptr_list[num], 8) + 0x20, buf.data, Xor_Str, Xor_Str_Size);
}

string GetSkillXor(hsdarc_buffer buf, long long int ptr, const int Xor[], int XorSize, string (*a)(long long int ptr, char data[], const int Xor[], int XorSize))
{
	if(read_data_long(buf.data, ptr, 8) == 0)
        return "\0";
	return a(read_data_long(buf.data, ptr, 8) + 0x20, buf.data, Xor_Str, Xor_Str_Size);
}

int GetEnemy(hsdarc_buffer buf, int num)
{
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
        SetConsoleOutputCP(CP_UTF8);
    #endif
    string strbuf = actOnData(buf, num++, Xor_Str, Xor_Str_Size, GetStringXorred);
    cout<<"Internal Identifier: "<<strbuf<<endl;
    strbuf = actOnData(buf, num++, Xor_Str, Xor_Str_Size, GetStringXorred);
    cout<<"Romanized Identifier: "<<strbuf<<endl;
    if(read_data_long(buf.data, buf.ptr_list[num-1] + 8, 8) == 0)
    {
        cout<<"----------------------------------------------------------------------------------------------------"<<endl;
        return num;
    }
    strbuf = actOnData(buf, num++, Xor_Str, Xor_Str_Size, GetStringXorred);
    cout<<"Face Folder: "<<strbuf<<endl;
    strbuf = actOnData(buf, num, Xor_Str, Xor_Str_Size, GetStringXorred);
    cout<<"Face Folder no.2: "<<strbuf<<endl;
	cout<<"Default Weapon: ";
	int fnum = num + 1;
    int weap = read_data_long(buf.data, buf.ptr_list[num] + 0x8, 8);
    if(weap != 0) 
    {
        strbuf = GetSkillXor(buf, buf.ptr_list[num] + 0x8 , Xor_Str, Xor_Str_Size, GetStringXorred);
        cout<<strbuf;
		fnum++;
    }
	cout<<endl;
    cout<<"ID: "<<read_data_Xorred(buf.data, buf.ptr_list[num] + 0x18, Xor_Id_Size, Xor_EnId, 0)<<endl;
    cout<<"Weapon: "<<Weapons[read_data_Xorred(buf.data, buf.ptr_list[num] + 0x1C, Xor_Weap_Size, Xor_EnWeap, 0)]<<endl;
    cout<<"Tome Element: "<<Tome_Elem[read_data_Xorred(buf.data, buf.ptr_list[num] + 0x1D, Xor_Tome_Size, Xor_EnTome, 0)]<<endl;
    cout<<"Movement Type: "<<Movement[read_data_Xorred(buf.data, buf.ptr_list[num] + 0x1E, Xor_Move_Size, Xor_EnMove, 0)]<<endl;
	cout<<"Unknown: ";
    for(int i=0; i<8; i++)
		cout<<read_data_Xorred(buf.data, buf.ptr_list[num] + 0x10 + i, 1, Xor_Unknown, i)<< " ";
	cout<<endl;
    if(read_data_Xorred(buf.data, buf.ptr_list[num] + 0x1F, Xor_Spawnable_Size, Xor_EnSpawnable, 0) != 0)
        cout<<"Randomly Spawnable Enemy"<<endl;
    else
        cout<<"Not Randomly Spawnable Enemy"<<endl;
    if(read_data_Xorred(buf.data, buf.ptr_list[num] + 0x20, Xor_Special_Size, Xor_EnSpecial, 0) != 0)
        cout<<"Special Enemy"<<endl;
    else
        cout<<"Not Special Enemy"<<endl;
    stats_tuple base_stats = GetHeroStats(buf.ptr_list[num] + 0x28, buf.data, Xor_Stats, 1);
    strbuf = PrintStats(base_stats);
    cout<<"5 Stars Level 1 Stats: "<<strbuf<<endl;
    stats_tuple hero_growths = GetHeroGrowths(buf.ptr_list[num] + 0x38, buf.data, Xor_Stats);
    strbuf = PrintStats(hero_growths);
    cout<<"Growth Points: "<<strbuf<<endl;
	stats_tuple level40stats = GetLvl40Stats(base_stats, hero_growths);
    strbuf = PrintStats(level40stats);
    cout<<"5 Stars Level 40 Stats: "<<strbuf<<endl;
	cout<<"-------------------------------------------------------------------------------------------"<<endl;
    return fnum;
}

stats_tuple GetHeroStats(long long int ptr, char data[], const int Xor[], int addition)
{
	stats_tuple stats;
    stats.hp = read_data_Xorred(data, ptr, 2, Xor, 0) + addition;
    stats.atk = read_data_Xorred(data, ptr + 2, 2, Xor, 2) + addition;
    stats.spd = read_data_Xorred(data, ptr + 4, 2, Xor, 4) + addition;
    stats.def = read_data_Xorred(data, ptr + 6, 2, Xor, 6) + addition;
    stats.res = read_data_Xorred(data, ptr + 8, 2, Xor, 8) + addition;
    stats.dummy1 = read_data_Xorred(data, ptr + 10, 2, Xor, 10) + addition;
    stats.dummy2 = read_data_Xorred(data, ptr + 12, 2, Xor, 12) + addition;
    stats.dummy3 = read_data_Xorred(data, ptr + 14, 2, Xor, 14) + addition;
	return stats;
}
stats_tuple GetHeroGrowths(long long int ptr, char data[], const int Xor[])
{
	stats_tuple stats;
    stats.hp = (read_data_Xorred(data, ptr, 2, Xor, 0) - 20) / 5;
    stats.atk = (read_data_Xorred(data, ptr + 2, 2, Xor, 2) - 20) / 5;
    stats.spd = (read_data_Xorred(data, ptr + 4, 2, Xor, 4) - 20) / 5;
    stats.def = (read_data_Xorred(data, ptr + 6, 2, Xor, 6) - 20) / 5;
    stats.res = (read_data_Xorred(data, ptr + 8, 2, Xor, 8) - 20) / 5;
    stats.dummy1 = (read_data_Xorred(data, ptr + 10, 2, Xor, 10) - 20) / 5;
    stats.dummy2 = (read_data_Xorred(data, ptr + 12, 2, Xor, 12) - 20) / 5;
    stats.dummy3 = (read_data_Xorred(data, ptr + 14, 2, Xor, 14) - 20) / 5;
	return stats;
}

string readShortInt(short int tmp, string String)
{
		if(tmp > 10000)
			String+=((tmp/10000)%10) + '0';
		if(tmp > 1000)
			String+=((tmp/1000)%10) + '0';
		if(tmp > 100)
			String+=((tmp/100)%10) + '0';
		if(tmp > 10)
			String+=((tmp/10)%10) + '0';
		String+=(tmp%10) + '0';
		return String;
}

string PrintStats(stats_tuple Stats)
{
	string String;
	String = readShortInt(Stats.hp, String);
	String += '|';
	String = readShortInt(Stats.atk, String);
	String += '|';
	String = readShortInt(Stats.spd, String);
	String += '|';
	String = readShortInt(Stats.def, String);
	String += '|';
	String = readShortInt(Stats.res, String);
	return String;
}

unsigned char GetFirstChar(hsdarc_buffer buf, int num)
{
	if(read_data_long(buf.data, buf.ptr_list[num], 8) == 0)
        return '\0';
    return buf.data[read_data_long(buf.data, buf.ptr_list[num], 8) + 0x20] ^ Xor_Str[0];
}