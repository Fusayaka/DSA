/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   CrossEntropy.cpp
 * Author: ltsach
 * 
 * Created on August 25, 2024, 2:47 PM
 */

#include "loss/CrossEntropy.h"
#include "ann/functions.h"

CrossEntropy::CrossEntropy(LossReduction reduction): ILossLayer(reduction){
    
}

CrossEntropy::CrossEntropy(const CrossEntropy& orig):
ILossLayer(orig){
}

CrossEntropy::~CrossEntropy() {
}

double CrossEntropy::forward(xt::xarray<double> X, xt::xarray<double> t){
    //YOUR CODE IS HERE
    m_aYtarget= t;
    m_aCached_Ypred = X;
    return cross_entropy(X,t,m_eReduction==REDUCE_MEAN);
}
xt::xarray<double> CrossEntropy::backward() {
    //YOUR CODE IS HERE
    int Nnorm=(m_eReduction==REDUCE_MEAN)?m_aCached_Ypred.shape()[0]:1;
    return -1.0 / Nnorm * (m_aYtarget / (m_aCached_Ypred + 1e-7));
}