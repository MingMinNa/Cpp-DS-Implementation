#pragma once

#include <cstdint>
#include <cassert>
#include <stdexcept>

/* Declaration */
namespace ds_imp {

enum class DSU_Rule {
    WEIGHT_RULE,
    HEIGHT_RULE
};

class DSU {

    public:
        DSU() = delete;
        DSU(uint32_t num_members, DSU_Rule rule);
        ~DSU();

        uint32_t find_root(uint32_t index);
        void union_root(uint32_t index_a, uint32_t index_b);
        bool is_same(uint32_t index_a, uint32_t index_b);

        uint32_t get_clusters_num() const;
        uint32_t get_members_num() const;

        static const uint32_t MAX_MEMBERS_NUM = static_cast<uint32_t>(1e9);
    
    private:
        int32_t *parent;
        uint32_t num_clusters;
        uint32_t num_members;
        DSU_Rule rule;
};

}

/* Implementation */
namespace ds_imp { 

    DSU::DSU(uint32_t num_members, DSU_Rule rule) {

        if(num_members == 0 || num_members > MAX_MEMBERS_NUM) {
            std::runtime_error("The number of members is out of range");
        }

        this->parent = new int32_t[num_members];
        this->num_members = num_members;
        this->num_clusters = num_members;
        this->rule = rule;

        for(decltype(num_members) i = 0; i < num_members; ++i)
            (this->parent)[i] = -1;
    }

    DSU::~DSU() {
        if(parent != nullptr)   
            delete [] parent;
    }

    uint32_t DSU::find_root(uint32_t index) {

        assert(this->parent != nullptr);

        if(index >= this->num_members)
            std::runtime_error("The index is out of range");

        if(this->parent[index] < 0)
            return index;

        uint32_t root = this->find_root(parent[index]);
        return parent[index] = root;
    }

    void DSU::union_root(uint32_t index_a, uint32_t index_b) {

        auto root_a = this->find_root(index_a);
        auto root_b = this->find_root(index_b);

        if(root_a == root_b)
            return;

        int16_t rank_a = (-1) * (this->parent[root_a]);
        int16_t rank_b = (-1) * (this->parent[root_b]);
        
        switch(this->rule) {
            case DSU_Rule::WEIGHT_RULE:
                if     (rank_a >= rank_b) {parent[root_a] -= rank_b; parent[root_b] = root_a;}
                else                      {parent[root_b] -= rank_a; parent[root_a] = root_b;}
                break;
            case DSU_Rule::HEIGHT_RULE:
                if     (rank_a >  rank_b)     {parent[root_b] = root_a;}
                else if(rank_a <  rank_b)     {parent[root_a] = root_b;}
                else/* (rank_a) == rank_b) */ {-- parent[root_a]; parent[root_b] = root_a;}
                break;
            default:
                assert(true);
                break;
        }
        (this->num_clusters) --;
        return;
    }

    bool DSU::is_same(uint32_t index_a, uint32_t index_b) {
        return find_root(index_a) == find_root(index_b);
    }

    uint32_t DSU::get_clusters_num() const {
        return this->num_clusters;
    }

    uint32_t DSU::get_members_num() const {
        return this->num_members;
    }
}
