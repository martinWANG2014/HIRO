#ifndef _H_HIRO_H
#define _H_HIRO_H

#include <vector>
#include "ilcplex/ilocplex.h"
#include "hirodefines.h"

namespace hiro
{
   class HIROsolution
   {
      public:
         // default constructor
         HIROsolution();

         // constructor with inputs
         HIROsolution(std::vector<double> x, double ub);

         // assignment operator
         //HIROsolution& operator=(const HIROsolution& rhs);

         // copy constructor
         //HIROsolution(const HIROsolution& rhs);

         // destructor
         virtual ~HIROsolution();

         // sets the values of the solution x
         void set_solution(std::vector<double> x);

         // sets the upper bound
         void set_upper_bound(double ub);

         // gets the values of the solution x
         std::vector<double> get_solution();

         // gets the upper bound
         double get_upper_bound();

      private:
         std::vector<double> _x;
         double _ub;
   };

   class HIRO
   {
      public:
         // default constructor
         HIRO();

         // default constructor with parameters
         HIRO(int _scenbudget, double _timelimit);

         // default destructor
         virtual ~HIRO();

         /// the virtual functions for the inner minimisation problem of the hard instance generator
         // function to solve an inner integer program of a robust problem.
         virtual const HIROsolution solve_ip(int numelements, int numscenarios,
            std::vector< std::vector<double> > scenariocosts) = 0;

         /// sets the parameters for the problem
         // sets the scenario budget
         void set_budget(int _scenbudget);

         // sets the time limit for the experiments
         void set_timelimit(double _timelimit);

         // algorithm type for the instance generation
         typedef enum
         {
            HIROALGTYPE_RANDOM      = 0,
            HIROALGTYPE_NORMAL      = 1,
            HIROALGTYPE_ITERATIVE   = 2,
            HIROALGTYPE_LAZY        = 3,
            HIROALGTYPE_MIDPOINT    = 4,
            HIROALGTYPE_MIDPOINTLP  = 5,
            HIROALGTYPE_ALT         = 6,
            HIROALGTYPE_ALTERNATE   = 7,
            HIROALGTYPE_LDR         = 8,
            HIROALGTYPE_COLGEN      = 9,
            HIROALGTYPE_NUMTYPE     = 10
         } algType;

         // sets the problem information
         void set_problem(int _type, int _n, int _N);
         void set_problem(int _type, int _n, int _N, int _scenbudget, double _timelimit);

         // sets the algorithm type
         void set_type(int _type);

         // sets the number of robust elements
         void set_num_elements(int _n);

         // sets the number of scenarios
         void set_num_scenarios(int _N);

         // generates a hard instance given the input parameters
         bool generate_hard_instance();

         /// functions to access member variables
         // returns the cost vector defining a hard instance
         std::vector<std::vector<double> > get_c();

         // returns the number of robust elements 'n' in the problem
         int get_num_elements();

         // returns the number of scenarios 'N' for the problem
         int get_num_scenarios();

      private:
         /// functions for generating random instances.
         // random instances sampled from a uniform distribution
         void generate_rand();

         // random instances sampled from a normal distribution
         void generate_normal();

         /// functions to generate the hard instances
         // hard instances generated by the exact iterative method or the relaxed column generation method
         void generate_hard(bool solvecompact = true);

         // ???
#if 0
         void generate_hard_lazy();
#endif

         // hard instances generated by using the alternating heuristic method
         void generate_hard_alternateheuristic();

         // hard instances generated by using linear decision rules
#if 0
         void generate_hard_LDR();
#endif

         // ???
         void generate_hard_alt();

         // hard instances generated using the midpoint method
#if 0
         void generate_midpoint();
#endif

         // hard instances generated using the LP of the midpoint integer program
         void generate_midpointlp();

         void gen_U();

         void print();

         int scenbudget;
         double timelimit;

         bool typeset;
         bool numcoeffsset;
         bool numscenariosset;
         bool problemsetup;
         algType type;
         int n;
         int N;
         std::vector<std::vector<double> > c;
         std::vector<std::vector<double> > nomc;

         std::vector<std::vector<double> > x;

         //bounds on c
         std::vector<std::vector<double> > cl;
         std::vector<std::vector<double> > cu;

         double solve_master(bool relax=false);
         double solve_master_decomp(double lowerbound, bool relax=false);
         double solve_master_alt();
         double solve_master_alternateheuristic();
         double solve_midlp_it();

         double build_decomp_sub(const IloEnv env, IloModel& model, IloCplex& cplex, IloObjective& objective,
            std::vector<std::vector<IloNumVar> >& cplexd, std::vector<IloNumVar>& cplexlambda, bool relax);
         double solve_decomp_sub(IloEnv& env, IloModel& model, IloCplex& cplex, IloObjective& objective,
            std::vector<std::vector<IloNumVar> > cplexd, std::vector<IloNumVar> cplexlambda, int j, double costdual,
            double convexdual, std::vector<std::vector<double> > scenduals, std::vector<double> exclude, double gap,
            bool enumerate, bool& add);
         double execute_column_generation(IloEnv& masterenv, IloModel& master, IloCplex& cpxmaster, IloEnv& env,
            IloModel& model, IloCplex& cplex, IloObjective& objective, std::vector<std::vector<IloNumVar> > cplexd,
            std::vector<IloNumVar>& cplexlambda, std::vector<std::vector<IloNumVar> >& cplexalpha,
            std::vector<IloRange> costcons, std::vector<IloRange> convexcons,
            std::vector<std::vector<std::vector<IloRange> > > scencons,
            std::vector<std::vector<std::vector<double> > >& columns, std::vector<int>& ncolumns, double lowerbound);
   };

} // namespace hiro
#endif
