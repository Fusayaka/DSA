/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

#ifndef DATALOADER_H
#define DATALOADER_H
#include "tensor/xtensor_lib.h"
#include "loader/dataset.h"

using namespace std;

template<typename DType, typename LType>
class DataLoader{
public:
    class Iterator; //forward declaration for class Iterator
    
private:
    Dataset<DType, LType>* ptr_dataset;
    int batch_size;
    bool shuffle;
    bool drop_last;
    int nbatch;
    ulong_tensor item_indices;
    int m_seed;
    
public:
    DataLoader(Dataset<DType, LType>* ptr_dataset, int batch_size, bool shuffle=true, bool drop_last=false, int seed=-1)
                : ptr_dataset(ptr_dataset), batch_size(batch_size), shuffle(shuffle),drop_last(drop_last),m_seed(seed){

            nbatch = ptr_dataset->len()/batch_size;
            item_indices = xt::arange(0, ptr_dataset->len());
            //Shuffle
            if (shuffle) {
                if (m_seed >= 0) {
                    xt::random::seed(m_seed);
                }
                xt::random::shuffle(item_indices);
            }
    }
    virtual ~DataLoader(){}
    
    //New method: from V2: begin
    int get_batch_size(){ return batch_size; }
    int get_sample_count(){ return ptr_dataset->len(); }
    int get_total_batch(){return int(ptr_dataset->len()/batch_size); }
    
    //New method: from V2: end
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// START: Section                                                     //
    /////////////////////////////////////////////////////////////////////////
public:
    Iterator begin() {
        return Iterator(this, 0);
    }

    Iterator end() {
        return Iterator(this, nbatch);
    }
    
    //BEGIN of Iterator

    class Iterator{
        private:
            int currbatch;
            DataLoader<DType, LType>* loader;
        public:
            Iterator(DataLoader<DType,LType>* loader,int currbatch):loader(loader),currbatch(currbatch){}

            Batch<DType, LType> operator*() const {
                // Tính toán chỉ số cho batch hiện tại
                if (loader->nbatch == 0) {
                    return Batch<DType, LType>(xt::xarray<DType>(), xt::xarray<LType>());
                }

                int start = currbatch * loader->batch_size;
                int end;

                //Drop last ảnh hưởng end
                if (loader->drop_last||currbatch!=loader->nbatch-1)
                    end = start+loader->batch_size;
                else
                    end = loader->ptr_dataset->len();
                //Tạo shape cho batch
                xt::svector<unsigned long> datashape=loader->ptr_dataset->get_data_shape();
                datashape[0]=end-start;
                xt::svector<unsigned long> labelshape = loader->ptr_dataset->get_label_shape();
                // Data và label của batch
                xt::xarray<DType>batchdata=xt::empty<DType>(xt::adapt(datashape));
                xt::xarray<LType> batchlabel;
                if (loader->ptr_dataset->get_label_shape().size() > 0) {
                    labelshape[0] = end - start;
                    batchlabel = xt::empty<LType>(xt::adapt(labelshape));
                }
                // Lặp để chép data vào batch
                for (int i=start;i<end;i++){
                    DataLabel<DType,LType> dtlb=loader->ptr_dataset->getitem(loader->item_indices[i]);
                    
                    xt::view(batchdata,i - start,xt::all()) = xt::view(dtlb.getData(),xt::all());
                    if (labelshape.size() > 0) {
                        xt::view(batchlabel, i - start, xt::all())= xt::view(dtlb.getLabel(),xt::all());
                    }
                }

                return Batch<DType, LType>(batchdata, batchlabel);
            }

            Iterator &operator++(){
                this->currbatch++;
                return *this;
            } 
            Iterator operator++(int) {
                Iterator temp = *this;
                ++(*this); 
                return temp;  
            }
            bool operator!=(const Iterator& other) const {
                return this->currbatch != other.currbatch;
            }
            bool operator==(const Iterator& other) const {
                return this->currbatch == other.currbatch;
            }
    };

    //END of Iterator
    
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// END: Section                                                       //
    /////////////////////////////////////////////////////////////////////////
};


#endif /* DATALOADER_H */

