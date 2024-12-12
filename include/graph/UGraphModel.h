/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   UGraphModel.h
 * Author: LTSACH
 *
 * Created on 24 August 2020, 15:16
 */

#ifndef UGRAPHMODEL_H
#define UGRAPHMODEL_H

#include "graph/AbstractGraph.h"
// #include "stacknqueue/PriorityQueue.h"

//////////////////////////////////////////////////////////////////////
///////////// UGraphModel: Undirected Graph Model ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class UGraphModel : public AbstractGraph<T>
{
private:
public:
    // class UGraphAlgorithm;
    // friend class UGraphAlgorithm;

    UGraphModel(
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str)
    {
    }

    void connect(T from, T to, float weight = 0)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode* fromver = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode* tover = this->getVertexNode(to);
        
        if (!fromver){
            // fromver = new typename AbstractGraph<T>::VertexNode(from, this->vertexEQ, this->vertex2str);
            // throw VertexNotFoundException(fromver->toString());

            throw VertexNotFoundException(this->vertex2str(from));
        }
        if (!tover){
            // tover = new typename AbstractGraph<T>::VertexNode(to, this->vertexEQ, this->vertex2str);
            // throw VertexNotFoundException(tover->toString());

            throw VertexNotFoundException(this->vertex2str(to));
        }

        fromver->connect(tover, weight);
        if (this->vertexEQ(from, to)){
            return;
        }
        tover->connect(fromver, weight);
    }
    void disconnect(T from, T to)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode* fromver = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode* tover = this->getVertexNode(to);
        
        if (!fromver){
            // fromver = new typename AbstractGraph<T>::VertexNode(from, this->vertexEQ, this->vertex2str);
            // throw VertexNotFoundException(fromver->toString());

            throw VertexNotFoundException(this->vertex2str(from));
        }
        if (!tover){
            // tover = new typename AbstractGraph<T>::VertexNode(to, this->vertexEQ, this->vertex2str);
            // throw VertexNotFoundException(tover->toString());

            throw VertexNotFoundException(this->vertex2str(to));
        }
        
        if (fromver->getEdge(tover)) {
            fromver->removeTo(tover);
        } 
        else {
            string s = fromver->toString() + ", " + tover->toString();
            throw EdgeNotFoundException(s);
        }
        if (this->vertexEQ(from, to)){
            return;
        }
        if (tover->getEdge(fromver)) {
            tover->removeTo(fromver);
        } 
        else {
            string s = tover->toString() + ", " + fromver->toString();
            throw EdgeNotFoundException(s);
        }
    }
    void remove(T vertex)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode* ver = this->getVertexNode(vertex);
        if (!ver) {
            // ver = new typename AbstractGraph<T>::VertexNode(vertex, this->vertexEQ, this->vertex2str);
            // throw VertexNotFoundException(ver->toString());

            throw VertexNotFoundException(this->vertex2str(vertex));
        }
        
        for (typename AbstractGraph<T>::VertexNode* node : this->nodeList) {
            if (node->getEdge(ver)) {
                node->removeTo(ver);
                ver->removeTo(node);
            }
        }


        int idx = 0;
        for (typename AbstractGraph<T>::VertexNode* node : this->nodeList) {
            if (this->vertexEQ(node->getVertex(), ver->getVertex()))  
                break;
            idx++;
        }
        this->nodeList.removeAt(idx);

        delete ver;
    }
    static UGraphModel<T> *create(
        T *vertices, int nvertices, Edge<T> *edges, int nedges,
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &))
    {
        // TODO
        UGraphModel<T>* tempgraph= new UGraphModel<T>(vertexEQ, vertex2str);
        for (int i = 0; i < nvertices; i++)
            tempgraph->add(vertices[i]);
        for (int i = 0; i < nedges; i++){
            tempgraph->connect(edges[i].from, edges[i].to, edges[i].weight);
        }
        return tempgraph;
    }
};

#endif /* UGRAPHMODEL_H */
