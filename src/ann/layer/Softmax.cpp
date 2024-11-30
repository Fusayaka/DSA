/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   Softmax.cpp
 * Author: ltsach
 * 
 * Created on August 25, 2024, 2:46 PM
 */

#include "layer/Softmax.h"
#include "ann/functions.h"
#include "sformat/fmt_lib.h"
#include <filesystem> //require C++17
namespace fs = std::filesystem;

Softmax::Softmax(int axis, string name): m_nAxis(axis) {
    if(trim(name).size() != 0) m_sName = name;
    else m_sName = "Softmax_" + to_string(++m_unLayer_idx);
}

Softmax::Softmax(const Softmax& orig) {
}

Softmax::~Softmax() {
}

xt::xarray<double> Softmax::forward(xt::xarray<double> X) {
    //YOUR CODE IS HERE
    m_aCached_Y =softmax(X,m_nAxis);
    return m_aCached_Y;
}
xt::xarray<double> Softmax::backward(xt::xarray<double> DY) {
    //YOUR CODE IS HERE
    if (DY.dimension()==1){
        auto J=xt::diag(m_aCached_Y) - xt::linalg::outer(m_aCached_Y,m_aCached_Y);
        return xt::linalg::dot(J,DY);
    }
    xt::xarray<double> DZ = xt::zeros<double>({DY.shape()[0], DY.shape()[1]});
    for (size_t i = 0; i < DY.shape()[0]; ++i) {
        auto y = xt::row(m_aCached_Y,i);
        auto dy = xt::row(DY,i);
        auto J = xt::diag(y) - xt::linalg::outer(y, y);
        auto dz = xt::linalg::dot(J,dy);
        xt::row(DZ,i)=dz;
    }

    return DZ;
}

string Softmax::get_desc(){
    string desc = fmt::format("{:<10s}, {:<15s}: {:4d}",
                    "Softmax", this->getname(), m_nAxis);
    return desc;
}
