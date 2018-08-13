#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <math.h>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/math/distributions/normal.hpp> // for normal_distribution
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>

#include <boost/numeric/odeint.hpp>
#include <boost/math/constants/constants.hpp>
#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;
// Front wheel radius = 0.2 m
// Back wheels radius = 0.2 m
// Distance from front wheel to back axis (r) = 1m

constexpr double D_F2A = 1.0;
constexpr double R_FW = 0.2;
constexpr double R_BW = 0.2;
constexpr int EN_RES = 512;

using namespace std;

double randn(double mean, double sigma)
{
 typedef boost::normal_distribution<> NormalDistribution;
 typedef boost::mt19937 RandomGenerator;
 typedef boost::variate_generator<boost::mt19937, boost::normal_distribution<>> GaussianGenerator;

  /** Initiate Random Number generator with current time */
  static RandomGenerator rng(static_cast<float>(time(0)));

  /* Choose Normal Distribution */
  static NormalDistribution gaussian_dist(mean, sigma);

  /* Create a Gaussian Random Number generator
   *  by binding with previously defined
   *  normal distribution object
   */
  static GaussianGenerator generator(rng, gaussian_dist);

  // sample from the distribution
  return generator();
}

template <typename A, typename B, typename C>
std::function<C(A)> compose(std::function<C(B)> f, std::function<B(A)> g) {
  return [f,g](A x) { return f(g(x)); };
}

double steering(double t)
{
	double toReturn = M_PI/6.0;
	return toReturn;
}

double velocity(double t)
{
	return 0.5;
}

using state_type = std::vector<double>;

struct CarModelParams
{
    double m_d_f2a;
	double m_r_fw;
	double m_r_bw;
	int m_en_res;
};

struct Drive
{
	std::function<double(double)> steer;
	std::function<double(double)> vel;
};

Drive noiselessDrive()
{
	Drive toReturn;
	toReturn.steer = [=](double t) {return (steering(t));};
	toReturn.vel = [=](double t) {return (velocity(t));};
	return toReturn;
}

Drive noisyDrive()
{
	double mean = 0;
	double sigma = 0.1;
	Drive toReturn;
	toReturn.steer = [=](double t) { return (randn(mean, sigma) + steering(t));};
	toReturn.vel = [=](double t) { return (randn(mean, sigma) + velocity(t));};
	return toReturn;
}


CarModelParams defaultParams()
{
	CarModelParams carModelParams{D_F2A, R_FW, R_BW, EN_RES};
	return carModelParams;
}

//[ rhs_class
/* The rhs of x' = f(x) defined as a class */
class Car {

	CarModelParams mParams;

	Drive mDrive;

public:

    Car(CarModelParams params, Drive drive) : mParams(params), mDrive(drive)
		{ }

    void operator() ( const state_type& state , state_type& dState , const double t/* t */ )
    {
		double steer = mDrive.steer(t);
		double vel = mDrive.vel(t);
        dState[0] = vel*cos(steer)*cos(state[2]);
        dState[1] = vel*cos(steer)*sin(state[2]);
        dState[2] = vel*sin(steer)/mParams.m_d_f2a;

        // cout << " time : " << t << " steer : " << steer << " vel: " << vel << " x : " << dState[0] << " y : " << dState[1] << " theta : " << dState[2] << "\n";

    }

private:


};
//]


//[ integrate_observer
struct push_back_state_and_time
{
    std::vector< state_type >& m_states;
    std::vector< double >& m_times;

    push_back_state_and_time( std::vector< state_type > &states , std::vector< double > &times )
    : m_states( states ) , m_times( times ) { }

    void operator()( const state_type &x , double t )
    {
        m_states.push_back( x );
        m_times.push_back( t );
    }
};
//]

struct write_state
{
    void operator()( const state_type &x ) const
    {
        std::cout << x[0] << "\t" << x[1] << "\n";
    }
};


state_type estimate(double time, double steering_angle, double encoder_ticks, double angular_velocity)
{
	state_type toReturn ={0.0,0.0,0.0};
	return toReturn;
}


int main()
{
	using namespace std;
    using namespace boost::numeric::odeint;


    //[ state_initialization
    state_type x{0.0, 0.0, 0.0};
    //]
	// Declaring tuple./es

	//[ integration_class
    Car car(defaultParams(), noisyDrive());
    size_t steps = integrate( car ,
            x , 0.0 , 10.0 , 0.1 );
    //]

   //[ integrate_observ
    vector<state_type> x_vec;
    vector<double> times;

	runge_kutta4< state_type > stepper;

    steps = integrate_const( stepper , car ,
            x , 0.0 , 10.0 , 0.1 ,
            push_back_state_and_time( x_vec , times ) );

	const double dt = 0.1;
    vector<double> x_;
    vector<double> y_;
    vector<double> theta_;
    int i =0;
	for( double t=0.0 ; t<10.0 ; t+= dt )
    {
        stepper.do_step( car , x , t , dt );
        x_.push_back(x_vec[i][0]);
        y_.push_back(x_vec[i][1]);
        theta_.push_back(x_vec[i][2]);
        i++;
    }

    double x_av = accumulate( x_.begin(), x_.end(), 0.0)/x_.size();
    double y_av = accumulate( y_.begin(), y_.end(), 0.0)/y_.size();
    double theta_av = accumulate( theta_.begin(), theta_.end(), 0.0)/theta_.size();

    cout << "x_av = " << x_av << "\n";
    cout << "y_av = " << y_av << "\n";
    cout << "theta_av = " << theta_av << "\n";

    // Set the size of output image = 1200x780 pixels
    plt::figure_size(1200, 780);
    // Plot line from given x and y data. Color is selected automatically.
    // plt::plot(x_, y_);
    // Plot a red dashed line from given x and y data.
    // plt::plot(x, w,"r--");
    // Plot a line whose name will show up as "log(x)" in the legend.
    plt::named_plot("vehicle tra", x_, y_);

    // Set x-axis to interval [0,1000000]
    // plt::xlim(0, 1000*1000);
    // Enable legend.
    plt::legend();
    // Save the image (file format is determined by the extension)
    plt::save("./basic.png");

    plt::show();

	return 0;
}

// g++ -std=c++11 -O2 -Wall -pedantic  estimate.cpp -o estimate -I/usr/include/python2.7 -lpython2.7
