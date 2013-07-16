/*
 * kmeans-adapter.cpp
 *
 *  Created on: May 31, 2013
 *      Author: jieshen
 */

#include "kmeans-adapter.hpp"
#include "utils.hpp"

#include <iostream>
using std::cerr;
using std::endl;

namespace jieshen
{
    const VlKMeansAlgorithm KMEANS_ADAPTER::DEFAULT_ALGORITHM = VlKMeansLloyd;
    const VlKMeansInitialization KMEANS_ADAPTER::DEFAULT_INIT = VlKMeansPlusPlus;
    const VlVectorComparisonType KMEANS_ADAPTER::DEFAULT_COMP = VlDistanceL2;

    KMEANS_ADAPTER::KMEANS_ADAPTER()
    {
        init();
    }

    KMEANS_ADAPTER::KMEANS_ADAPTER(float* data, const int dim,
                                   const int num_pt)
    {
        init();
        setData(data, dim, num_pt);
    }

    KMEANS_ADAPTER::~KMEANS_ADAPTER()
    {
        clear();
    }

    void KMEANS_ADAPTER::init()
    {
        init_model_parameters();
        m_kmeans_model = NULL;
        m_has_extracted = false;

        m_data_ref = NULL;
        m_dim = 0;
        m_num_point = 0;
        m_assignment = NULL;
    }

    void KMEANS_ADAPTER::init_model_parameters()
    {
        m_algorithm = DEFAULT_ALGORITHM;
        m_init_type = DEFAULT_INIT;
        m_comp_type = DEFAULT_COMP;
        m_num_rep = DEFAULT_NUM_REP_INVALID;
        m_num_iter = DEFAULT_NUM_ITER_INVALID;
        m_verbosity = DEFAULT_VERBOSITY_INVALID;
        m_num_center = DEFAULT_NUM_CENTER_INVALID;
    }

    void KMEANS_ADAPTER::clear()
    {
        clear_model_related_data();
        init_model_parameters();

        m_data_ref = NULL;
        m_dim = 0;
        m_num_point = 0;
        if (m_assignment)
            utils::myfree(&m_assignment);
    }

    void KMEANS_ADAPTER::clear_model_related_data()
    {
        if (m_kmeans_model)
        {
            vl_kmeans_delete(m_kmeans_model);
            m_kmeans_model = NULL;
        }

        m_has_extracted = false;
    }

    void KMEANS_ADAPTER::setAlgorithm(VlKMeansAlgorithm algo)
    {
        if (algo != VlKMeansLloyd && algo != VlKMeansElkan
                && algo != VlKMeansANN)
        {
            cerr << "ERROR Algorithm parameter!" << endl;
            exit(-1);
        }

        if (algo == m_algorithm)
            return;

        m_algorithm = algo;
        m_has_extracted = false;
    }

    void KMEANS_ADAPTER::setInitMethod(VlKMeansInitialization init_method)
    {
        if (init_method != VlKMeansPlusPlus
                && init_method != VlKMeansRandomSelection)
        {
            cerr << "ERROR Initialization Method!" << endl;
            exit(-1);
        }

        if (init_method == m_init_type)
            return;

        m_init_type = init_method;
        m_has_extracted = false;
    }

    void KMEANS_ADAPTER::setCompType(VlVectorComparisonType comp)
    {
        if (comp == m_comp_type)
            return;

        m_comp_type = comp;
        m_has_extracted = false;
    }

    void KMEANS_ADAPTER::setNumIterations(const vl_size num_iter)
    {
        if (num_iter <= 0)
        {
            cerr << "Number of iteration should > 0" << endl;
            exit(-1);
        }

        if (num_iter == m_num_iter)
            return;

        m_num_iter = num_iter;
        m_has_extracted = false;
    }

    void KMEANS_ADAPTER::setNumRepetitions(const vl_size num_rep)
    {
        if (num_rep <= 0)
        {
            cerr << "Number of repetition should > 0" << endl;
            exit(-1);
        }

        if (num_rep == m_num_rep)
            return;

        m_num_rep = num_rep;
        m_has_extracted = false;
    }

    void KMEANS_ADAPTER::setVerbosity(const int verbose)
    {
        if (verbose < 0)
        {
            cerr << "Verbosity should be non-negative" << endl;
            exit(-1);
        }

        if (verbose == m_verbosity)
            return;

        m_verbosity = verbose;
        m_has_extracted = false;
    }

    void KMEANS_ADAPTER::setNumCenter(const int num_c)
    {
        if (num_c <= 0)
        {
            cerr << "Number of centers should be positive" << endl;
            exit(-1);
        }

        if (num_c == m_num_center)
            return;

        m_num_center = num_c;
        m_has_extracted = false;
    }

    void KMEANS_ADAPTER::setData(float* data, const int dim,
                                 const int num_pt)
    {
        if (data == NULL || dim <= 0 || num_pt <= 0)
        {
            cerr << "Check the data" << endl;
            exit(-1);
        }

        if (data == m_data_ref && dim == m_dim && num_pt == m_num_point)
            return;

        m_data_ref = data;
        m_dim = dim;
        m_num_point = num_pt;

        m_has_extracted = false;
    }

    VlKMeansAlgorithm KMEANS_ADAPTER::getAlgorithm() const
    {
        if (m_has_extracted)
            return vl_kmeans_get_algorithm(m_kmeans_model);
        else
            return m_algorithm;
    }

    VlKMeansInitialization KMEANS_ADAPTER::getInitMethod() const
    {
        if (m_has_extracted)
            return vl_kmeans_get_initialization(m_kmeans_model);
        else
            return m_init_type;
    }

    VlVectorComparisonType KMEANS_ADAPTER::getCompType() const
    {
        if (m_has_extracted)
            return vl_kmeans_get_distance(m_kmeans_model);
        else
            return m_comp_type;
    }

    vl_size KMEANS_ADAPTER::getNumRepetitions() const
    {
        if (m_has_extracted)
            return vl_kmeans_get_num_repetitions(m_kmeans_model);
        else
        {
            if (m_num_rep == DEFAULT_NUM_REP_INVALID)
                return DEFAULT_NUM_REP;
            else
                return m_num_rep;
        }
    }

    vl_size KMEANS_ADAPTER::getNumIterations() const
    {
        if (m_has_extracted)
            return vl_kmeans_get_max_num_iterations(m_kmeans_model);
        else
        {
            if (m_num_iter == DEFAULT_NUM_ITER_INVALID)
                return DEFAULT_NUM_ITER;
            else
                return m_num_iter;
        }
    }

    int KMEANS_ADAPTER::getVerbosity() const
    {
        if (m_has_extracted)
            return vl_kmeans_get_verbosity(m_kmeans_model);
        else
        {
            if (m_verbosity == DEFAULT_VERBOSITY_INVALID)
                return DEFAULT_VERBOSITY;
            else
                return m_verbosity;
        }
    }

    int KMEANS_ADAPTER::getNumCenter() const
    {
        if (m_has_extracted)
            return vl_kmeans_get_num_centers(m_kmeans_model);
        else
        {
            if (m_num_center == DEFAULT_NUM_CENTER_INVALID)
                return DEFAULT_NUM_CENTER;
            else
                return m_num_center;
        }
    }

    const float* KMEANS_ADAPTER::getData() const
    {
        return m_data_ref;
    }

    const int KMEANS_ADAPTER::getDim() const
    {
        return m_dim;
    }

    const int KMEANS_ADAPTER::getNumPoint() const
    {
        return m_num_point;
    }

    void KMEANS_ADAPTER::set_kmeans_model()
    {
        clear_model_related_data();

        m_kmeans_model = vl_kmeans_new(VL_TYPE_FLOAT, getCompType());
        vl_kmeans_set_algorithm(m_kmeans_model, getAlgorithm());
        vl_kmeans_set_initialization(m_kmeans_model, getInitMethod());
        vl_kmeans_set_num_repetitions(m_kmeans_model, getNumRepetitions());
        vl_kmeans_set_max_num_iterations(m_kmeans_model, getNumIterations());
        vl_kmeans_set_verbosity(m_kmeans_model, getVerbosity());
    }

    void KMEANS_ADAPTER::clusterData()
    {
        if (m_has_extracted)
            return;

        if (getData() == NULL)
        {
            cerr << "Set the data first" << endl;
            exit(-1);
        }

        set_kmeans_model();

        double energy = vl_kmeans_cluster(m_kmeans_model, getData(), getDim(),
                                          getNumPoint(), getNumCenter());

        if (m_assignment)
            utils::myfree(&m_assignment);
        m_assignment = (vl_uint32*) utils::mymalloc(
                getNumPoint() * sizeof(vl_uint32));

        vl_kmeans_quantize(m_kmeans_model, m_assignment, NULL, getData(),
                           getNumPoint());

        m_has_extracted = true;
    }

    const float* KMEANS_ADAPTER::getCenters() const
    {
        if (!m_has_extracted)
        {
            cerr << "Call clusterData() first" << endl;
            return NULL;
        }

        return (float*)vl_kmeans_get_centers(m_kmeans_model);
    }

    const vl_uint32* KMEANS_ADAPTER::getAssignments() const
    {
        if (!m_has_extracted)
        {
            cerr << "Call clusterData() first" << endl;
            return NULL;
        }

        return m_assignment;
    }

    string KMEANS_ADAPTER::info() const
    {
        string info = "=====K-Means Settings=====\n";

        info += "Algorithm:      ";
        if(getAlgorithm() == VlKMeansLloyd)
            info += "Lloyd";
        else if(getAlgorithm() == VlKMeansElkan)
            info += "Elkan";
        else if(getAlgorithm() == VlKMeansANN)
            info += "ANN";
        else
            info += "UNKNOWN";
        info += "\n";

        info += "Initialization: ";
        if(getInitMethod() == VlKMeansPlusPlus)
            info += "PlusPlus";
        else if(getInitMethod() == VlKMeansRandomSelection)
            info += "Random";
        else
            info += "UNKNOWN";
        info += "\n";

        info += "Comparison:     ";
        if(getCompType() == VlDistanceL2)
            info += "L2";
        else
            info += "UNKNOWN";
        info += "\n";

        info += "Num Repetition: "  + utils::myitoa(getNumRepetitions()) + "\n";

        info += "Num Iteration:  " + utils::myitoa(getNumIterations()) + "\n";

        info += "Verbosity:      " + utils::myitoa(getVerbosity()) + "\n";

        info += "Num Center:     " + utils::myitoa(getNumCenter()) + "\n";

        info += "\n-----Data Info-----\n";
        info += "Dimension:      " + utils::myitoa(m_dim) + "\n";
        info += "Num Points:     " + utils::myitoa(m_num_point) + "\n";

        return info;
    }

}
