/*
 * kmeans-adapter.hpp
 *
 *  Created on: May 31, 2013
 *      Author: jieshen
 */

#ifndef __VLFEAT_ADAPTER_KMEANS_ADAPTER_HPP__
#define __VLFEAT_ADAPTER_KMEANS_ADAPTER_HPP__

#ifdef __cplusplus
extern "C"
{
#endif

#include <vl/kmeans.h>

#ifdef __cplusplus
}
#endif

#include <string>
using std::string;

namespace jieshen
{
    class KMEANS_ADAPTER
    {
        static const VlKMeansAlgorithm DEFAULT_ALGORITHM;
        static const VlKMeansInitialization DEFAULT_INIT;
        static const VlVectorComparisonType DEFAULT_COMP;
        enum{
            DEFAULT_NUM_REP = 1,
            DEFAULT_NUM_ITER = 100,
            DEFAULT_VERBOSITY = 0,
            DEFAULT_NUM_CENTER = 100,

            DEFAULT_NUM_REP_INVALID = -1,
            DEFAULT_NUM_ITER_INVALID = -1,
            DEFAULT_VERBOSITY_INVALID = -1,
            DEFAULT_NUM_CENTER_INVALID = -1
        };
    public:
        KMEANS_ADAPTER();
        KMEANS_ADAPTER(float* data, const int dim, const int num_pt);
        ~KMEANS_ADAPTER();
    public:
        void setAlgorithm(VlKMeansAlgorithm algo);
        void setInitMethod(VlKMeansInitialization init_method);
        void setCompType(VlVectorComparisonType comp);
        void setNumRepetitions(const vl_size num_rep);
        void setNumIterations(const vl_size num_iter);
        void setVerbosity(const int verbose);
        void setNumCenter(const int num_c);
        void setData(float* data, const int dim, const int num_pt);

        VlKMeansAlgorithm getAlgorithm() const;
        VlKMeansInitialization getInitMethod() const;
        VlVectorComparisonType getCompType() const;
        vl_size getNumRepetitions() const;
        vl_size getNumIterations() const;
        int getVerbosity() const;
        int getNumCenter() const;
        const float* getData() const;
        const int getDim() const;
        const int getNumPoint() const;

    public:
        void clusterData();
        const float* getCenters() const;
        const vl_uint32* getAssignments() const;

        string info() const;
    private:
        void init();
        void init_model_parameters();
        void clear();
        void clear_model_related_data();

        void set_kmeans_model();
    private:
        VlKMeans* m_kmeans_model;
        VlKMeansAlgorithm m_algorithm;
        VlKMeansInitialization m_init_type;
        VlVectorComparisonType m_comp_type;
        vl_size m_num_rep;
        vl_size m_num_iter;
        int m_verbosity;
        int m_num_center;
        bool m_has_extracted;

        float* m_data_ref;
        int m_dim;
        int m_num_point;
        vl_uint32* m_assignment;
    };
}


#endif /* __VLFEAT_ADAPTER_KMEANS_ADAPTER_HPP__ */
