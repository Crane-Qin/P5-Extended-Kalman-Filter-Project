#include "kalman_filter.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

// Please note that the Eigen library does not initialize 
// VectorXd or MatrixXd objects with zeros upon creation.

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() {
  /**
  TODO:
    * predict the state
  */
    x_=F_*x_;
    P_=F_*P_*F_.transpose()+Q_;
    
}

void KalmanFilter::Update(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Kalman Filter equations
  */
    VectorXd y_ = z - H_ * x_;
    MatrixXd S_=H_*P_*H_.transpose()+R_;
    MatrixXd K_=P_*H_.transpose()*S_.inverse();
    
    x_=x_+K_*y_;
    //MatrixXd I = MatrixXd::Identity(2, 2);
    long x_size= x_.size();
    MatrixXd I = MatrixXd::Identity(x_size,x_size);
    
    P_=(I-K_*H_)*P_;
}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Extended Kalman Filter equations
  */
    //MatrixXd Hj_=Tools::CalculateJacobian(x_);

    double px = x_(0);
    double py = x_(1);
    double vx = x_(2);
    double vy = x_(3);
    
    double rho=sqrt(px*px+py*py);
    double phi=atan2(py,px);
    double rhodot=(px*vx+py*vy)/rho;
    VectorXd hx(3);
    
    hx << rho,phi,rhodot;
    
    VectorXd y_ = z - hx;
    // normalize y to -PI : PI
    while ( y_(1) > M_PI || y_(1) < -M_PI ) {
        if ( y_(1) > M_PI ) {
            y_(1) -= M_PI;
        } else {
            y_(1) += M_PI;
        }
    }
    
    MatrixXd S_=H_*P_*H_.transpose()+R_;
    MatrixXd K_=P_*H_.transpose()*S_.inverse();
    
    x_=x_+K_*y_;
    //MatrixXd I = MatrixXd::Identity(3, 3);
    long x_size= x_.size();
    MatrixXd I = MatrixXd::Identity(x_size,x_size);
    
    P_=(I-K_*H_)*P_;
}
