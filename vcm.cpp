#include "vcm.h"

VCM::VCM()
{
    // true for view mode
    f_vcm = true;
    // choosed was used in choose mode
    // -1 means none was choosed
    // once you can only choose one item/pic
    choosed = -1;
}

void VCM::setVCM(bool f_vcm)
{
    this->f_vcm = f_vcm;
    choosed  = -1;
}

void VCM::setChoosed(int choosed)
{
    this->choosed = choosed;
}

void VCM::deletePic(int pic)
{
    garbge.push(pic);
}

int VCM::recovery()
{
    if(garbge.empty())
        return -1;
//    return garbge.pop();
    int res = garbge.top();
    garbge.pop();
    return res;
}

