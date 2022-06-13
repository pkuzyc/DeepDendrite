#ifndef NRN_SECTION_MAPPING
#define NRN_SECTION_MAPPING

#include <string>
#include <vector>
#include <map>
#include <iostream>

namespace coreneuron {

/** type to store every section and associated segments */
typedef std::vector<int> segvec_type;
typedef std::map<int, segvec_type> secseg_map_type;
typedef secseg_map_type::iterator secseg_it_type;

/** @brief Section to segment mapping
 *
 *  For a section list (of a particulat type), store mapping
 *  of section to segments
 *  a section is a arbitrary user classification to recognize some segments (ex: api, soma, dend,
 * axon)
 *
 */
struct SecMapping {
    /** name of section list */
    std::string name;

    /** map of section and associated segments */
    secseg_map_type secmap;

    SecMapping() {
    }
    SecMapping(std::string s) : name(s) {
    }

    /** @brief return total number of sections in section list */
    size_t num_sections() {
        return secmap.size();
    }

    /** @brief return number of segments in section list */
    size_t num_segments() {
        size_t count = 0;
        for (secseg_it_type iterator = secmap.begin(); iterator != secmap.end(); iterator++) {
            count += iterator->second.size();
        }
        return count;
    }

    /** @brief add section to associated segment */
    void add_segment(int sec, int seg) {
        secmap[sec].push_back(seg);
    }
};

/** @brief Compartment mapping information for a cell
 *
 * A cell can have multiple section list types like
 * soma, axon, apic, dend etc. User will add these
 * section lists using HOC interface.
 */
struct CellMapping {
    /** gid of a cell */
    int gid;

    /** list of section lists (like soma, axon, apic) */
    std::vector<SecMapping*> secmapvec;

    CellMapping(int g) : gid(g) {
    }

    /** @brief total number of sections in a cell */
    int num_sections() {
        int nsec = 0;
        for (size_t i = 0; i < secmapvec.size(); i++) {
            nsec += secmapvec[i]->num_sections();
        }
        return nsec;
    }

    /** @brief return number of segments in a cell */
    int num_segments() {
        int nseg = 0;
        for (size_t i = 0; i < secmapvec.size(); i++) {
            nseg += secmapvec[i]->num_segments();
        }
        return nseg;
    }

    /** @brief number of section lists */
    size_t size() {
        return secmapvec.size();
    }

    /** @brief add new SecMapping */
    void add_sec_map(SecMapping* s) {
        secmapvec.push_back(s);
    }

    /** @brief return section list mapping with given name */
    SecMapping* get_seclist_mapping(std::string name) {
        for (size_t i = 0; i < secmapvec.size(); i++) {
            if (name == secmapvec[i]->name)
                return secmapvec[i];
        }

        std::cout << "Warning: Section mapping list " << name << " doesn't exist! \n";
        return NULL;
    }

    /** @brief return segment count for specific section list with given name */
    size_t get_seclist_segment_count(std::string name) {
        SecMapping* s = get_seclist_mapping(name);
        size_t count = 0;
        if (s) {
            count = s->num_segments();
        }
        return count;
    }
    /** @brief return segment count for specific section list with given name */
    size_t get_seclist_section_count(std::string name) {
        SecMapping* s = get_seclist_mapping(name);
        size_t count = 0;
        if (s) {
            count = s->num_sections();
        }
        return count;
    }

    ~CellMapping() {
        for (size_t i = 0; i < secmapvec.size(); i++) {
            delete secmapvec[i];
        }
    }
};

/** @brief Compartment mapping information for NrnThread
 *
 * NrnThread could have more than one cell in cellgroup
 * and we store this in vector.
 */
struct NrnThreadMappingInfo {
    /** list of cells mapping */
    std::vector<CellMapping*> mappingvec;

    /** @brief number of cells */
    size_t size() {
        return mappingvec.size();
    }

    /** @brief memory cleanup */
    ~NrnThreadMappingInfo() {
        for (size_t i = 0; i < mappingvec.size(); i++) {
            delete mappingvec[i];
        }
    }

    /** @brief get cell mapping information for given gid
     *	if exist otherwise return NULL.
     */
    CellMapping* get_cell_mapping(int gid) {
        for (size_t i = 0; i < mappingvec.size(); i++) {
            if (mappingvec[i]->gid == gid) {
                return mappingvec[i];
            }
        }
        return NULL;
    }

    /** @brief add mapping information of new cell */
    void add_cell_mapping(CellMapping* c) {
        mappingvec.push_back(c);
    }
};

struct MyCellMapping 
{
    int gid;
    std::string cell_name;
    std::map<std::string, segvec_type> sec2segs;

    MyCellMapping(std::string s) : cell_name(s) {}

    size_t get_section_num()
    {
        return sec2segs.size();
    }

    size_t get_seg_num()
    {
        int nseg = 0;
        std::map<std::string, segvec_type>::iterator it;
        for (it = sec2segs.begin(); it != sec2segs.end(); it++)
        {
            nseg += it->second.size();
        }
        return nseg;
    }

    void new_pair(std::string name)
    {
        segvec_type vec_seg;
        sec2segs.insert(std::pair<std::string, segvec_type>(name, vec_seg));
    }

    void add_segment(std::string sec_name, int seg)
    {
        sec2segs[sec_name].push_back(seg);
    }

    bool has_section(std::string sec_name)
    {
        return sec2segs.find(sec_name) != sec2segs.end();
    }

    int get_segment(std::string secname, float pos)
    {
        if (sec2segs.find(secname) == sec2segs.end())
        {
            printf("section %s.%s not exists\n", cell_name.c_str(), secname.c_str());
            return -1;
        }
        segvec_type vec_segs = sec2segs[secname];
        int iseg;
        //for (int i = 0; i < vec_segs.size(); i++)
        //    printf("%d ", vec_segs[i]);
        //printf("\n");
        int actual_seg; 
        double interval, first_pos, fseg;
        bool is_soma = secname.find("soma") != secname.npos;
        if (pos == 1)
        {
            iseg = (int)vec_segs.size() - 1;
        }
        else if (pos == 0)
        {
            iseg = 0;
        }
        else
        {

            if (is_soma)
            {
                actual_seg = (int)vec_segs.size() - 2;
            }
            else
            {
                actual_seg = (int)vec_segs.size() - 1;
            }
            interval = 1.0 / actual_seg;
            first_pos = interval / 2;
            fseg = (pos * 1.0 - first_pos) / interval;
            if (is_soma)
            {
                iseg = fseg + 5e-5 + 1;
            }
            else
            {
                iseg = fseg + 5e-5;
            }
        }
        
        return vec_segs[iseg];
    }
};

struct ThreadMapping
{
    std::vector<MyCellMapping*> vec_cellmap;

    size_t size()
    {
        return vec_cellmap.size();
    }

    void add_cell_mapping(MyCellMapping* m)
    {
        vec_cellmap.push_back(m);
    }

    MyCellMapping* get_cell_map(std::string name)
    {
        size_t j = vec_cellmap.size();
        for (size_t i = 0; i < j; i++)
        {
            if (vec_cellmap[i]->cell_name == name)
                return vec_cellmap[i];
        }
        return NULL;
    }

    ~ThreadMapping()
    {
        for (size_t i = 0; i < vec_cellmap.size(); i++)
        {
            delete vec_cellmap[i];
        }
    }
};

}

#endif  // NRN_SECTION_MAPPING
