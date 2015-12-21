#ifndef VCM_H
#define VCM_H
#include <stack>
/*
 * this class is about ViewChooseMode
 * store some flag infomation
 * and some data that will used
 *
 */


class VCM
{
public:
    VCM();
    void setVCM(bool f_vcm);
    void setChoosed(int choosed);
    void deletePic(int pic);
    int recovery();
public:
    std::stack<int> garbge;
    bool f_vcm;
    int choosed;
};

#endif // VCM_H
