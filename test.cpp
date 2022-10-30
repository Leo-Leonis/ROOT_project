#include "ResonanceType.hpp"
#include "ParticleType.hpp"
#include <iostream>
#include <vector>
//#include <algorithm>

int main()
{
  ParticleType *part = new ParticleType(std::string("hola"), 3.4, -1);
  ResonanceType *res = new ResonanceType(std::string("dora"), 1.3, 0, 10.);

  std::vector<ParticleType*> part_ptr_vectr;
  part_ptr_vectr.push_back(part);
  part_ptr_vectr.push_back(res);
  
  for (ParticleType* part_ptr : part_ptr_vectr) {
    part_ptr->Print();
    std::cout << '\n';
  }

  return 0;
}