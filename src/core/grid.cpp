//
// Created by Sumedhe Dissanayake on 10/6/18.
//

#include "grid.h"

#include <limits>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <cstdlib>
#include <cmath>


#include "../helpers/trainer_helper.h"
#include "../struct/node.h"

#include <iostream>



Grid::Grid(int x_dim, int y_dim, int dimention, bool random_initialization){
    this->x_dim     = x_dim;
    this->y_dim     = y_dim;
    this->dimention = dimention;

    this->nodes.resize((u_long)x_dim * y_dim);
}


void Grid::InitializeNodes() {
    this->nodes.resize((u_long)this->Size());

    // Set coordinates
    for (int i = 0; i < this->Size(); ++i){
        pair<float, float> loc = CalculateLocation(i);
        this->nodes[i].x_pos = loc.first;
        this->nodes[i].y_pos = loc.second;
    }

    // Set distances between nodes
    for (int i = 0; i < this->Size(); ++i){
        this->nodes[i].distance_to = vector<float>((u_long)this->Size());
    }
    for (int i = 0; i < this->Size(); ++i){
        for (int j = i; j < this->Size(); ++j){
            float dist = this->CalculateDistance(this->nodes[i], this->nodes[j]);
            this->nodes[i].distance_to[j] = dist;
            this->nodes[j].distance_to[i] = dist;
        }
    }

}

float Grid::CalculateDistance(Node node1, Node node2) {
    return euclidean_distance(node1.x_pos, node1.y_pos, node2.x_pos, node2.y_pos);
}

int Grid::FindBMU(vector<double> input_vector) {
    float min_dist = std::numeric_limits<float>::max();
    int bmu = -1;
    for (int i = 0; i < this->Size(); ++i) {
        float dist = squared_euclidean_distance(input_vector, this->nodes[i].features);
        if (dist < min_dist){
            min_dist = dist;
            bmu = i;
        }
    }
    return bmu;
}

Node Grid::GetNode(int node_index){
    return this->nodes[node_index];
}

vector<vector<double> > Grid::GetWeightMatrix(){
    vector<vector<double> > matrix(this->Size());
    for (int i = 0; i < this->Size(); ++i){
        std::vector<double> item = vector<double>((u_long)this->dimention);
        for (int j = 0; j < this->dimention; ++j){
            item[j] = this->nodes[i].features[j];
        }
        matrix[i] = item;
    }
    return matrix;
}

string Grid::ToString() {
    std::ostringstream oss;

    for (int i = 0; i < this->Size(); ++i){
        oss << "[";
        for (int j = 0; j < this->dimention; ++j){
            oss << this->nodes[i].features[j];
            if (this->dimention != (j + 1)){
                oss << ",";
            }
        }
        oss << "]\t";

        if ((i+1) % this->x_dim == 0){
            oss << "\n";
        }
    }

    return oss.str();
}

int Grid::Size(){
    return (int)this->nodes.size();
}
