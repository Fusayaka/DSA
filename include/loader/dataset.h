/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */


#ifndef DATASET_H
#define DATASET_H
#include "tensor/xtensor_lib.h"
using namespace std;

template<typename DType, typename LType>
class DataLabel{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
public:
    DataLabel(xt::xarray<DType> data,  xt::xarray<LType> label):data(data), label(label){}
    xt::xarray<DType> getData() const{ return data; }
    xt::xarray<LType> getLabel() const{ return label; }
};

template<typename DType, typename LType>
class Batch{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
public:
    Batch(xt::xarray<DType> data,  xt::xarray<LType> label):data(data), label(label){}
    virtual ~Batch(){}
    xt::xarray<DType>& getData(){return data; }
    xt::xarray<LType>& getLabel(){return label; }
};


template<typename DType, typename LType>
class Dataset{
private:
public:
    Dataset(){};
    virtual ~Dataset(){};
    
    virtual int len()=0;
    virtual DataLabel<DType, LType> getitem(int index)=0;
    virtual xt::svector<unsigned long> get_data_shape()=0;
    virtual xt::svector<unsigned long> get_label_shape()=0;
    
};

//////////////////////////////////////////////////////////////////////
template<typename DType, typename LType>
class TensorDataset: public Dataset<DType, LType>{
private:
    xt::xarray<DType> data;
    xt::xarray<LType> label;
    xt::svector<unsigned long> data_shape, label_shape;
    
public:
    /* TensorDataset: 
     * need to initialize:
     * 1. data, label;
     * 2. data_shape, label_shape
    */
    TensorDataset(xt::xarray<DType> data, xt::xarray<LType> label){
        /* TODO: your code is here for the initialization
         */
        this->data=data;
        this->data_shape=xt::svector<unsigned long>(data.shape().begin(),data.shape().end());
        // this->label_shape=xt::svector<unsigned long>(label.shape().begin(),label.shape().end());
        if (label.size() == 0) {
            // Label rỗng
            label_shape = xt::svector<unsigned long>(); 
        } 
        else{
            this->label=label;
            label_shape = xt::svector<unsigned long>(this->label.shape().begin(), this->label.shape().end());
        }
    }
    /* len():
     *  return the size of dimension 0
    */
    int len(){
        /* TODO: your code is here to return the dataset's length
         */
        return data_shape[0]; 
    }
    
    /* getitem:
     * return the data item (of type: DataLabel) that is specified by index
     */
    DataLabel<DType, LType> getitem(int index){
        /* TODO: your code is here
         */
        if (index < 0 || index >= this->len()) {
            throw std::out_of_range("Index is out of range!");
        }
        xt::xarray<DType> tempdata = xt::view(data, index, xt::all());
        //TH1: Không cần nhãn
        if (label_shape.size() == 0) {
            xt::xarray<LType> virlabel;
            return DataLabel<DType, LType>(tempdata, virlabel);
        }

        // TH2: Cần nhãn
        if (label_shape[0] != data_shape[0]) {
            throw std::runtime_error("Data and label size mismatch.");
        }
        
        xt::xarray<LType> templabel = xt::view(label, index, xt::all());

        return DataLabel<DType, LType>(tempdata, templabel);
    }
    
    xt::svector<unsigned long> get_data_shape(){
        /* TODO: your code is here to return data_shape
         */
        return data_shape;
    }
    xt::svector<unsigned long> get_label_shape(){
        /* TODO: your code is here to return label_shape
         */
        return label_shape;
    }
};



#endif /* DATASET_H */

