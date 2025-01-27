#ifndef NALUWIND_H
#define NALUWIND_H

#include <vector>
#include <string>

#include "Simulation.h"
#include "stk_util/parallel/Parallel.hpp"
#include "yaml-cpp/yaml.h"
#include "ExawindSolver.h"

namespace TIOGA {
class tioga;
}

namespace exawind {

class NaluWind : public ExawindSolver
{
private:
    YAML::Node m_doc;
    sierra::nalu::Simulation m_sim;
    std::vector<std::string> m_fnames;
    int m_ncomps;

public:
    static void initialize();
    static void finalize();
    NaluWind(
        stk::ParallelMachine comm,
        const std::string& inp_file,
        const std::vector<std::string>& fnames,
        TIOGA::tioga& tg);
    bool is_unstructured() override { return true; }
    bool is_amr() override { return false; }
    int overset_update_interval() override;
    int time_index() override;
    std::string identifier() override { return "Nalu-Wind"; }
    MPI_Comm comm() override { return m_comm; }
    int get_ncomps() { return m_ncomps; }

protected:
    void init_prolog(bool multi_solver_mode = true) override;
    void init_epilog() override;
    void prepare_solver_prolog() override;
    void prepare_solver_epilog() override;
    void pre_advance_stage1() override;
    void pre_advance_stage2() override;
    void advance_timestep() override;
    void additional_picard_iterations(const int) override;
    void post_advance() override;
    void pre_overset_conn_work() override;
    void post_overset_conn_work() override;
    void register_solution() override;
    void update_solution() override;
    MPI_Comm m_comm;
};

} // namespace exawind

#endif /* NALUWIND_H */
