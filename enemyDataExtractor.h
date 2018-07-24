struct enemy_info {
  long long int unknown;
  int id_num;  
  unsigned char weapon_type;
  unsigned char tome_element;
  unsigned char move_type;
  bool spawnable_enemy;
  bool special_enemy;
};

struct stats_tuple {
  short int hp, atk, spd, def, res, dummy1, dummy2, dummy3;
};

struct enemy_definition {
  unsigned char* id_tag;
  unsigned char* roman;
  unsigned char* face_name;
  unsigned char* face_name2;
  enemy_info info;
  stats_tuple base_stats;
  stats_tuple growth_rates;
};

int GetEnemy(hsdarc_buffer buf, int num);
stats_tuple GetHeroStats(long long int ptr, char data[], const int Xor[], int addition);
std::string actOnData(hsdarc_buffer buf, int num, const int Xor[], int XorSize, std::string (*a)(long long int ptr, char data[], const int Xor[], int XorSize));
stats_tuple GetHeroGrowths(long long int ptr, char data[], const int Xor[]);
unsigned char GetFirstChar(hsdarc_buffer buf, int num);
std::string PrintStats(stats_tuple Stats);